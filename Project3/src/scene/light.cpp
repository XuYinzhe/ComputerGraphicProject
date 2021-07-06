#include <cmath>

#include "light.h"
#include <iostream>

#define PI 3.14159265

double DirectionalLight::distanceAttenuation( const vec3f& P ) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}


vec3f DirectionalLight::shadowAttenuation( const vec3f& P ) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
	isect i;
	return scene->intersect(ray(P, getDirection(P)), i) ? prod(getColor(P), i.getMaterial().kt) : getColor(P);
}

vec3f DirectionalLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f DirectionalLight::getDirection( const vec3f& P ) const
{
	return -orientation;
}

double PointLight::distanceAttenuation( const vec3f& P ) const
{
	// YOUR CODE HERE

	// You'll need to modify this method to attenuate the intensity 
	// of the light based on the distance between the source and the 
	// point P.  For now, I assume no attenuation and just return 1.0
	
	double distance = 1.0 / (getScene()->m_attConstant + getScene()->m_attLinear * (position - P).length() + getScene()->m_attQuatric * (position - P).length_squared());
	return minimum(1.0 / (getScene()->m_attConstant + getScene()->m_attLinear * (position - P).length() + getScene()->m_attQuatric * (position - P).length_squared()), 1.0);
	//return minimum(1.0 / (constant_attenuation_coeff + linear_attenuation_coeff * (position - P).length() + quadratic_attenuation_coeff * (position - P).length_squared()),1.0);
}

vec3f PointLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f PointLight::getDirection( const vec3f& P ) const
{
	return (position - P).normalize();
}


vec3f PointLight::shadowAttenuation(const vec3f& P) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
	isect i;
	return scene->intersect(ray(P, getDirection(P)), i) ? prod(getColor(P), i.getMaterial().kt) : getColor(P);
}

vec3f SpotLight::getColor(const vec3f& P) const
{
	//std::cout << acos(direction * (P-position).normalize()) << std::endl;
	if (acos(direction * (P - position).normalize()) < edgeplace[0] * (PI / 180.0))
		return color;
	else
		return vec3f(0, 0, 0);
}

vec3f WarnLight::getColor(const vec3f& P) const
{
	double R = radius[0];
	vec3f pointPosition = vec3f(P[0] - position[0], P[1] - position[1], 0);
	double pointDistance = pointPosition.length();
	double limitDistrance;
	double thetaX = sin(direction[0] * (PI / 180.0));
	double thetaY = cos(direction[0] * (PI / 180.0));
	double theta = acos(vec3f(thetaX, thetaY, 0) * pointPosition.normalize());
	if (type[0] - 1 < 0.000001)
		//std::cout << position[0] << ' ' << position[1] << ' ' << position[2] << std::endl; std::cout << P[0] << ' ' << P[1] << ' ' << P[2] << std::endl;
		if (theta < 2.0 * PI / 3.0)
			limitDistrance = sin(PI / 6.0) * R / sin(PI - PI / 6.0 - theta);
		else
		{
			theta = acos(vec3f(-thetaX, -thetaY, 0) * pointPosition.normalize());
			limitDistrance = sin(PI / 6.0) * R / sin(PI / 2.0 - theta);
		}
	else if (type[0] - 2 < 0.000001)
		if (theta < PI / 2.0)
			limitDistrance = sin(PI / 4.0) * R / sin(PI - PI / 4.0 - theta);
		else
		{
			theta = acos(vec3f(-thetaX, -thetaY, 0) * pointPosition.normalize());
			limitDistrance = sin(PI / 4.0) * R / sin(PI / 4.0 + theta);
		}
	else if (type[0] - 3 < 0.000001)
	{
		if (theta < PI / 3.0);
		else if (theta < 2.0 * PI / 3.0)theta -= PI / 3.0;
		else theta -= 2.0 * PI / 3.0;
		limitDistrance = sin(PI / 3.0) * R / sin(PI - PI / 3.0 - theta);
	}
	else limitDistrance = R;
	if (pointDistance < limitDistrance)return color; 
	else return vec3f(0, 0, 0);
}