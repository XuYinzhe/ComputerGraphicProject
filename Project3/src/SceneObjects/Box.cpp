#include <cmath>
#include <assert.h>

#include "Box.h"

bool Box::intersectLocal( const ray& r, isect& i ) const
{
	// YOUR CODE HERE:
    // Add box intersection code here.
	// it currently ignores all boxes and just returns false.
	double t1, t2;
	if (!ComputeLocalBoundingBox().intersect(r, t1, t2))return false;
	if (t2 < RAY_EPSILON)return false;

	i.setT(t1);
	vec3f P = r.at(t1);
	vec3f N = vec3f(0, 0, 0);
	for (int i = 0; i < 3; i++)
		if (abs(P[i] - ComputeLocalBoundingBox().max[i]) < RAY_EPSILON)N[i] = 1;
		else if (abs(P[i] - ComputeLocalBoundingBox().min[i]) < RAY_EPSILON)N[i] = -1;
	if (N.dot(r.getDirection()) > 0)N = -N;
	i.setN(N);
	i.obj = this;

	return true;
}
