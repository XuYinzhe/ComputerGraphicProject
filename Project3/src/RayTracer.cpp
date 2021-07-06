// The main ray tracer.

#include <Fl/fl_ask.h>

#include "RayTracer.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/ray.h"
#include "fileio/read.h"
#include "fileio/parse.h"
#include "ui/TraceUI.h"
#include <iostream>
#include <random>
#include <time.h>

#define PI 3.14159265

default_random_engine e;

uniform_real_distribution<float> random(-0.1, 0.1);
uniform_real_distribution<float> random3(-0.05, 0.05);
normal_distribution<float> random2(0,0.03);

// Trace a top-level ray through normalized window coordinates (x,y)
// through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of 0.
vec3f RayTracer::trace( Scene *scene, double x, double y )
{
	e.seed(time(0)*x/y);
    ray r( vec3f(0,0,0), vec3f(0,0,0) );
    scene->getCamera()->rayThrough( x,y,r );
	std::stack<Material*> materials;
	materials.push(Material::getAir());

	// DOF
	vec3f dofContribution = vec3f(0, 0, 0);
	float dofRays = 0;

	double focusDis = ((TraceUI*)UI)->focusLength;
	if (((TraceUI*)UI)->m_DOF ) {		//depth of field
		vec3f focPoint = r.at(focusDis);
		dofRays = 10;// num of rays
		vec3f ranDir = r.at(1);
		for (int i = 0; i < dofRays; i++) {


			ranDir += random3(e) * ((TraceUI*)UI)->apa * scene->getCamera()->u;
			ranDir += random3(e) * ((TraceUI*)UI)->apa * scene->getCamera()->v;

			ray dofRay = ray(ranDir, (focPoint - ranDir).normalize());

			dofContribution += 1.0 / (dofRays + 1) * traceRay(scene, dofRay, scene->m_threshold, 0, materials);
		}
	}
	//DOF end

	return (dofContribution + 1.0 / (dofRays + 1) * traceRay(scene, r, scene->m_threshold, 0, materials)).clamp();


}

// Do recursive ray tracing!  You'll want to insert a lot of code here
// (or places called from here) to handle reflection, refraction, etc etc.
vec3f RayTracer::traceRay( Scene *scene, const ray& r, 
	const vec3f& thresh, int depth, std::stack<Material*> materials)
{
	isect i;

	int glossyLoop = ((TraceUI*)UI)->m_Glossy?25:1;
	

	if( scene->intersect( r, i ) ) {
		// YOUR CODE HERE

		// An intersection occured!  We've got work to do.  For now,
		// this code gets the material for the surface that was intersected,
		// and asks that material to provide a color for the ray.  

		// This is a great place to insert code for recursive ray tracing.
		// Instead of just returning the result of shade(), add some
		// more steps: add in the contributions from reflected and refracted
		// rays.
		vec3f reflection = vec3f(0, 0, 0);
		vec3f refraction = vec3f(0, 0, 0);




		const Material& m = i.getMaterial();




		if (depth < ((TraceUI*)UI)->getDepth()) {
			vec3f in = (r.at(i.t) - r.getPosition()).normalize();
			i.N = i.N.normalize();
			ray reflectionRay(r.at(i.t) , (in - 2*(in.dot(i.N))*i.N).normalize());

			reflection = 1.0/glossyLoop * traceRay(scene, reflectionRay, thresh, depth+1, materials).clamp();
			for (int j = 0; j < glossyLoop-1; j++) {
				vec3f ranDir = reflectionRay.getDirection() - reflectionRay.getPosition();
				vec3f vertComp = ranDir * sin(acos(in.dot(i.N)))*random(e);
				ranDir = (in - 2 * (in.dot(i.N)) * i.N).normalize();

				ranDir[0] += random(e);
				ranDir[1] += random(e);
				ranDir[2] += random(e);

				ray glossyRay(reflectionRay.getPosition(), ranDir.normalize());
				reflection += 1.0 / glossyLoop * traceRay(scene, glossyRay, thresh, depth + 1, materials).clamp();
			}

			if (m.kt[0] != 0 || m.kt[1] != 0 || m.kt[2] != 0) {// Obj is not completely solid
				//double inAngle = acos(i.N * in) * 180.0 / PI;
				//double test = (index == 1 ? 1 : m.index) * sin(inAngle) / (index == 1 ? m.index : 1);
				//
				//double refAngle = asin((index == 1 ? 1 : m.index) * sin(inAngle) / (index == 1 ? m.index : 1)) * 180.0 / PI;
				//if (1 - (pow((index == 1 ? 1 : m.index), 2) / pow((index == 1 ? 1 : m.index), 2) * (1 - pow(cos(inAngle), 2))) < 0) {
				//	printf("Total ref");
				//}
				//refAngle = acos(sqrt(1 - (pow((index == 1 ? 1 : m.index), 2) / pow((index == 1 ? 1 : m.index), 2) * (1 - pow(cos(inAngle), 2))))) * 180.0 / PI;
				//vec3f refOut = -abs(cos(refAngle)) * i.N + abs(sin(refAngle) / sin(inAngle)) * (in + abs(cos(inAngle)) * i.N);
				double ind = 1;
				vec3f normalDir;
				if (materials.top() == &m) {
					materials.pop();
					ind = m.index/ (materials.top())->index;
					normalDir = -i.N;
				}
				else {
					ind = (materials.top())->index / m.index;
					materials.push((Material*)&m);
					normalDir = i.N;
				}

				double cosIn = -in* normalDir;
				//if (cosIn < 0) {
				//	//cosIn = in * i.N;
				//}
				double cosRef = sqrt(1 - pow(ind, 2) * (1 - pow(cosIn, 2)));

				//double inAngle = acos(cosIn) * 180.0 / PI;

				//double refAngle = acos(cosRef) * 180.0 / PI;

				if (isnan(cosRef) ){//Total reflection, do nothing
					//printf("Total ref");
					//double test = sqrt(-(1 - pow(ind, 2) * (1 - pow(cosIn, 2))));
					if (-(1 - pow(ind, 2) * (1 - pow(cosIn, 2))) <= EPSILONS) {// handle epsilons, probabaly never happend
						cosRef = 0;
						goto canRef;
					}
				}
				else {
canRef:
					vec3f refOut = (-abs(cosRef) * normalDir + abs(ind) * (in + abs(cosIn) * normalDir)).normalize();
					//double dis = acos(testOut * refOut) * 180.0 / PI;
					//if (dis > 0.1) {
					//	//printf("diff");
					//}
					ray refractionRay(r.at(i.t), refOut);
					refraction = 1.0 / glossyLoop * traceRay(scene, refractionRay, thresh, depth + 1, materials).clamp();

					for (int j = 0; j < glossyLoop - 1; j++) {
						vec3f ranDir = refOut;
						ranDir[0] += random2(e);
						ranDir[1] += random2(e);
						ranDir[2] += random2(e);

						ray glossyRay(reflectionRay.getPosition(), (ranDir).normalize());
						refraction += 1.0 / glossyLoop * traceRay(scene, glossyRay, thresh, depth + 1, materials).clamp();
					}
				}


			}
			
		}		
		return (m.shade(scene, r, i,UI)  + prod(m.kr , reflection) + prod(m.kt, refraction)).clamp();
	
	} else {
		// No intersection.  This ray travels to infinity, so we color
		// it according to the background color, which in this (simple) case
		// is just black.
		if (m_backgroundData && scene->m_background == 1)
		{
			vec3f u = scene->getCamera()->u;
			vec3f v = scene->getCamera()->v;
			vec3f look = scene->getCamera()->look;
			unsigned char* pixel = backgroundPixel(u, v, look, r);
			return vec3f((float)pixel[0] / 255.0f, (float)pixel[1] / 255.0f, (float)pixel[2] / 255.0f).clamp();
		}

		return vec3f( 0.0, 0.0, 0.0 );
	}
}

//debug use ONLY


RayTracer::RayTracer()
{
	buffer = NULL;
	buffer_width = buffer_height = 256;
	scene = NULL;

	m_bSceneLoaded = false;
}


RayTracer::~RayTracer()
{
	delete [] buffer;
	delete scene;
}

void RayTracer::getBuffer( unsigned char *&buf, int &w, int &h )
{
	buf = buffer;
	w = buffer_width;
	h = buffer_height;
}

double RayTracer::aspectRatio()
{
	return scene ? scene->getCamera()->getAspectRatio() : 1;
}

bool RayTracer::sceneLoaded()
{
	return m_bSceneLoaded;
}

bool RayTracer::loadScene( char* fn )
{
	try
	{
		scene = readScene( fn );
	}
	catch( ParseError pe )
	{
		fl_alert( "ParseError: %s\n", pe );
		return false;
	}

	if( !scene )
		return false;
	
	buffer_width = 256;
	buffer_height = (int)(buffer_width / scene->getCamera()->getAspectRatio() + 0.5);

	bufferSize = buffer_width * buffer_height * 3;
	buffer = new unsigned char[ bufferSize ];
	
	// separate objects into bounded and unbounded
	scene->initScene();
	
	// Add any specialized scene loading code here
	
	m_bSceneLoaded = true;

	return true;
}

void RayTracer::traceSetup( int w, int h )
{
	if( buffer_width != w || buffer_height != h )
	{
		buffer_width = w;
		buffer_height = h;

		bufferSize = buffer_width * buffer_height * 3;
		delete [] buffer;
		buffer = new unsigned char[ bufferSize ];
	}
	memset( buffer, 0, w*h*3 );
}

void RayTracer::traceLines( int start, int stop )
{
	vec3f col;
	if( !scene )
		return;

	if( stop > buffer_height )
		stop = buffer_height;

	for( int j = start; j < stop; ++j )
		for( int i = 0; i < buffer_width; ++i )
			tracePixel(i,j);
}

vec3f RayTracer::adaptSupper(const double X, const double Y, const double deltaX, const double deltaY, int depth)
{
	if (depth < scene->m_nDepth)
	{
		double x[2] = { X - deltaX / 2.0, X + deltaX / 2.0 };//01 11  //1 2
		double y[2] = { Y - deltaY / 2.0, Y + deltaY / 2.0 };//00 10  //3 4 
		vec3f diffP[4] = { trace(scene,x[0],y[1]), trace(scene,x[1],y[1]),
						   trace(scene,x[0],y[0]), trace(scene,x[1],y[0]) };
		vec3f midP = trace(scene, X, Y);
		vec3f sum = midP;
		int count = 1;
		/*
		if ((diffP[0] - diffP[1]).length() + (diffP[0] - diffP[2]).length() + (diffP[0] - diffP[3]).length()
			+ (diffP[1] - diffP[2]).length() + (diffP[1] - diffP[3]).length() + (diffP[2] - diffP[3]).length() > scene->m_adaptDiff)
			sum = (adaptSupper(X - deltaX / 4.0, Y + deltaY / 4.0, deltaX / 2.0, deltaY / 2.0, depth + 1)
				+ adaptSupper(X + deltaX / 4.0, Y + deltaY / 4.0, deltaX / 2.0, deltaY / 2.0, depth + 1)
				+ adaptSupper(X - deltaX / 4.0, Y - deltaY / 4.0, deltaX / 2.0, deltaY / 2.0, depth + 1)
				+ adaptSupper(X + deltaX / 4.0, Y - deltaY / 4.0, deltaX / 2.0, deltaY / 2.0, depth + 1)) / 4;*/
		if ((midP - diffP[0]).length() > scene->m_adaptDiff)
		{
			sum += adaptSupper(X - deltaX / 4.0, Y + deltaY / 4.0, deltaX / 2.0, deltaY / 2.0, depth + 1);
			count = count + 1;
		}
		if ((midP - diffP[1]).length() > scene->m_adaptDiff)
		{
			sum += adaptSupper(X + deltaX / 4.0, Y + deltaY / 4.0, deltaX / 2.0, deltaY / 2.0, depth + 1);
			count = count + 1;
		}
		if ((midP - diffP[2]).length() > scene->m_adaptDiff)
		{
			sum += adaptSupper(X - deltaX / 4.0, Y - deltaY / 4.0, deltaX / 2.0, deltaY / 2.0, depth + 1);
			count = count + 1;
		}
		if ((midP - diffP[3]).length() > scene->m_adaptDiff)
		{
			sum += adaptSupper(X + deltaX / 4.0, Y - deltaY / 4.0, deltaX / 2.0, deltaY / 2.0, depth + 1);
			count = count + 1;
		}
		return sum / count;
	}
	else return trace(scene, X, Y);
}

void RayTracer::tracePixel( int i, int j )
{
	vec3f col;

	if( !scene )
		return;

	double x = double(i)/double(buffer_width);
	double y = double(j)/double(buffer_height);

	if (scene->m_adapt == 1 && scene->m_nDepth >= 2)
		col = adaptSupper(x, y, 1.0 / double(buffer_width), 1.0 / double(buffer_height), 1);
	else//adapt supper close, supper sample and jittering is available
		if (scene->m_supersampling == 1)
			if (getScene()->m_jittering == 0)
				col = trace(scene, x, y);
			else
				col = trace(scene,
					x + ((double)rand() / (double)RAND_MAX - 0.5) / double(buffer_width),
					y + ((double)rand() / (double)RAND_MAX - 0.5) / double(buffer_height));
		else
		{
			vec3f sum = vec3f(0, 0, 0);
			int numSup = getScene()->m_supersampling;
			double numSupD = double(getScene()->m_supersampling) - 1;
			double X = (double(i) - 0.5) / double(buffer_width);
			double Y = (double(j) - 0.5) / double(buffer_height);
			double deltaX = 1.0 / double(buffer_width) / numSupD;
			double deltaY = 1.0 / double(buffer_height) / numSupD;
			for (int m = 0; m < numSup; m++)
				for (int n = 0; n < numSup; n++)
					if (getScene()->m_jittering == 1)
						sum += trace(scene,
							X + m * deltaX + ((double)rand() / RAND_MAX - 0.5) / double(buffer_width),
							Y + n * deltaY + ((double)rand() / RAND_MAX - 0.5) / double(buffer_height));
					else 
						sum += trace(scene, X + m * deltaX, Y + n * deltaY);
			col = sum / (numSup * numSup);
		}

	unsigned char *pixel = buffer + ( i + j * buffer_width ) * 3;

	pixel[0] = (int)( 255.0 * col[0]);
	pixel[1] = (int)( 255.0 * col[1]);
	pixel[2] = (int)( 255.0 * col[2]);
}

unsigned char* RayTracer::backgroundPixel(const vec3f& u, const vec3f& v, const vec3f& look, const ray& r)
{
	vec3f d = r.getDirection();
	double aspectRatio = d * look / look.length_squared();
	double U = d * u / u.length_squared() / aspectRatio + 0.5;
	double V = d * v / v.length_squared() / aspectRatio + 0.5;

	return m_backgroundData + (int(bmp_width * U) + int(bmp_height * V) * bmp_width) * 3;
}

