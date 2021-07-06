#include <cmath>

#include "Square.h"

bool Square::intersectLocal( const ray& r, isect& i ) const
{
	vec3f p = r.getPosition();
	vec3f d = r.getDirection();

	if( d[2] == 0.0 ) {
		return false;
	}

	double t = -p[2]/d[2];

	if( t <= RAY_EPSILON ) {
		return false;
	}

	vec3f P = r.at( t );

	if( P[0] < -0.5 || P[0] > 0.5 ) {	
		return false;
	}

	if( P[1] < -0.5 || P[1] > 0.5 ) {	
		return false;
	}

	i.obj = this;
	i.t = t;
	if( d[2] > 0.0 ) {
		i.N = vec3f( 0.0, 0.0, -1.0 );
	} else {
		i.N = vec3f( 0.0, 0.0, 1.0 );
	}

	return true;
}

bool Square::getObjUV(const ray& r, vec3f& u, vec3f& v) const
{
	//std::cout << "aaa" << std::endl;
	vec3f rP = r.getPosition();vec3f rD = r.getDirection();
	vec3f p = transform->globalToLocalCoords(rP);
	vec3f d = -p + transform->globalToLocalCoords(rP + rD);
	d /= d.length();
	isect i;
	if (intersectLocal(ray(p,d), i))
	{
		vec3f P = p + i.t * d;
		u = vec3f((P[0] + 0.5) , 0, 1).clamp();
		v = vec3f((P[1] + 0.5) , 0, 1).clamp();
		return true;
	}
	else return false;
	/*
	ray objRay(p, d);
	double t = -p[2] / d[2];
	if (t <= RAY_EPSILON)return false;

	vec3f P = r.at(t);
	if (P[0] < -0.5 || P[0] > 0.5)return false;
	if (P[1] < -0.5 || P[1] > 0.5)return false;

	u = vec3f(P[0] + 0.5, 0, 1).clamp();
	v = vec3f(P[1] + 0.5, 0, 1).clamp();
	*/
	
}
