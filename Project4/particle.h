 // SAMPLE_SOLUTION
#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>

class Particle 
{
public:
	Particle(Vec3f p, Vec3f v, float m) :position(p), velocity(v), mass(m) {}

	float mass;
	Vec3f position, velocity, force;
};


#endif
