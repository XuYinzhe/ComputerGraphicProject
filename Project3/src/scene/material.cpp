#include "ray.h"
#include "material.h"
#include "light.h"
#include <iostream>
#include "../ui/TraceUI.h"
static Material* Air = nullptr;

Material* Material::getAir()
{
	if (Air == nullptr) {
		Air = new Material();
	}
	return Air;
}

// Apply the phong model to this point on the surface of the object, returning
// the color of that point.
vec3f Material::shade(Scene* scene, const ray& r, const isect& i, void* UI) const
{
	// YOUR CODE HERE

	// For now, this method just returns the diffuse color of the object.
	// This gives a single matte color for every distinct surface in the
	// scene, and that's it.  Simple, but enough to get you started.
	// (It's also inconsistent with the phong model...)

	// Your mission is to fill in this method with the rest of the phong
	// shading model, including the contributions of all the light sources.
	// You will need to call both distanceAttenuation() and shadowAttenuation()
	// somewhere in your code in order to compute shadows and light falloff.

	vec3f N = i.N;
	vec3f V = r.getDirection();
	vec3f P = r.at(i.t);
	vec3f pointColor = ke + prod(prod(scene->m_ambient, ka), vec3f({ 1,1,1 }) - kt);
	//std::cout << scene->m_ambient[0]*100 << std::endl;
	typedef list<Light*>::const_iterator iter;
	iter j;
	for (j = scene->beginLights(); j != scene->endLights(); ++j)
	{

		vec3f L = (*j)->getDirection(P);
		vec3f R = (L - 2 * (N * L) * N).normalize();

		vec3f diffuse;
		vec3f uObj, vObj; //std::cout << i.obj->getObjUV(r,  uObj, vObj) << std::endl;
		if (scene->m_textureData && scene->m_texture == 1 && i.obj->getObjUV(r, uObj, vObj))
			if (uObj[0] > 0.000001 && vObj[0] > 0.000001)
			{
				//std::cout << "aaa" << std::endl;
				unsigned char* pixel = scene->m_textureData + (int(vObj[0] * scene->texture_height) * scene->texture_width + int(uObj[0] * scene->texture_width)) * 3;
				//printf("u:%f,v:%f \n", uObj[0], vObj[0]);
				diffuse = vec3f((float)pixel[0] / 255.0f, (float)pixel[1] / 255.0f, (float)pixel[2] / 255.0f).clamp();
			}
			else
				diffuse = vec3f(1, 1, 1);
		else
			diffuse = maximum(0, N * L) * kd;


	

		double theta = maximum(0, R * V);
		vec3f specular = pow(theta, shininess * 128) * ks;

		//Soft Shadow	
		bool isSoft = ((TraceUI*)UI)->m_SoftShadow;
		
		float displacement = isSoft?0.05:0;
		float samplePoints = isSoft?9:1;

		for (float dx = -displacement; dx <= displacement; dx += 2 * displacement / (sqrt(samplePoints)-1)) {

			for (float dy = -displacement; dy <= displacement; dy += 2 * displacement / (sqrt(samplePoints) - 1)) {
				vec3f reamSamplePoint = getDisplacePoint(P[0] + dx, P[1] + dy,N,P);
				pointColor += (1/ samplePoints)*
					(prod(
						prod((*j)->distanceAttenuation(reamSamplePoint) * (*j)->shadowAttenuation(reamSamplePoint), (*j)->getColor(reamSamplePoint)),
						prod(
							diffuse,
							vec3f(1, 1, 1) - kt
						)
						+
						specular
					));
			}
		}
		//Soft Shadow end

	}

	return pointColor.clamp();
}

vec3f getDisplacePoint(float x, float y, vec3f n, vec3f p) {

	float z = (-n[0] * (x - p[0]) - n[1] * (y - p[1])) / (n[2]) + p[2];
	return vec3f(x, y, isnan(z)?p[2]:z);
}