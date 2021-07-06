#pragma warning(disable : 4786)

#include "particleSystem.h"


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <iostream>

#define COLOR_RED		1.0f, 0.0f, 0.0f
#define COLOR_GREEN		0.0f, 1.0f, 0.0f
#define COLOR_BLUE		0.0f, 0.0f, 1.0f

/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem() 
{
	// TODO
	bake_fps = 30;
	bake_start_time = -1;
	bake_end_time = -1;
	simulate = false;
	dirty = false;

	particleRadius = .03;
	systemForce.push_back(Vec3f(0, -9.8, 0));//gravity
	systemForce.push_back(Vec3f(1, 0, 0));//wind
}

void ParticleSystem::createParticle(Mat4f camM, Mat4f curM, Vec3f v, float size, int n, float m, float t)
{
	//Vec4f position4f = p * Vec4f(0, 0, 0, 1);
	//Vec3f position3f = Vec3f(p[12], p[13], p[14]);
	default_random_engine e(time(0));
	uniform_real_distribution<float> u1(-size / 2, size / 2);
	uniform_real_distribution<float> u2(-5, 5);
	uniform_real_distribution<float> u3(-m * 0.3, m * 0.3);
	//Mat4f CurrentModelViewMatrix = getModelViewMatrix();
	Mat4f WorldMatrix = camM.inverse() * curM;
	Vec4f WorldPoint = WorldMatrix * Vec4f(0, 0, 0, 1);
	//Vec4f direction = WorldMatrix * Vec4f(0, 1, 0, 0);
	//cout << isSimulate() << endl;
	if (isSimulate())
		if (bakeInfo.find(t + 1.0 / bake_fps) == bakeInfo.end())
		{
			for (int i = 0; i < n; i++)
			{
				/*
				Vec3f position = Vec3f(WorldPoint[0] + size / 2 - rand() % (int)size,
					WorldPoint[1] + size / 2 - rand() % (int)size,
					WorldPoint[2] + size / 2 - rand() % (int)size);*/
				Vec3f position = Vec3f(WorldPoint[0] + u1(e), WorldPoint[1] + u1(e), WorldPoint[2] + u1(e));
				Particle newParticle = Particle(position, v + Vec3f(u2(e), u2(e), u2(e)), m + u3(e));
				Vec3f force = Vec3f(0, 0, 0);
				for (vector<Vec3f>::iterator it = systemForce.begin(); it != systemForce.end(); it++)
					force += *it;
				newParticle.force = force;
				particleUnion.push_back(newParticle);
			}
		}
}



/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
	// TODO
	clearBaked();
}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
    
	// TODO
	bake_start_time = t;
	// These values are used by the UI ...
	// -ve bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
    
	// TODO
	bake_end_time = t;
	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
    
	// TODO
	clearBaked();
	particleUnion.clear();
	bake_start_time = -1;
	bake_end_time = -1;
	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{
	if(isSimulate())
		if (bakeInfo.find(t) == bakeInfo.end())
		{
			//detecting collisions
			for (vector<Particle>::iterator i = particleUnion.begin(); i != particleUnion.end(); i++)
				for (vector<Particle>::iterator j = particleUnion.begin(); j != particleUnion.end() && i != j; j++)
					if ((i->position - j->position).length() - (double)particleRadius * 2 <= 0)
					{
						Vec3f collisionDirectioni = j->position - i->position; collisionDirectioni.normalize();
						Vec3f collisionDirectionj = i->position - j->position; collisionDirectionj.normalize();
						Vec3f vi = i->velocity; Vec3f vj = j->velocity;
						Vec3f vis = vi * collisionDirectioni * collisionDirectioni;
						Vec3f vjs = vj * collisionDirectionj * collisionDirectionj;
						Vec3f vit = vi - vis; Vec3f vjt = vj - vjs;
						swap(vis, vjs);
						i->velocity = vis + vit;
						j->velocity = vjs + vjt;
					}
			for (vector<Particle>::iterator it = particleUnion.begin(); it != particleUnion.end(); it++)
			{
				it->position += it->velocity / bake_fps + it->force / it->mass / bake_fps / bake_fps / 2;//x+=vt+at^2/2
				it->velocity += it->force / it->mass / bake_fps;//v+=at
			}
			bakeParticles(t);
		}
	// TODO
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{
	if(isSimulate())
		if (bakeInfo.find(t) != bakeInfo.end()) 
		{
			vector<Particle> currentParticleUnion = bakeInfo.at(t);
			for (vector<Particle>::iterator it = currentParticleUnion.begin(); it != currentParticleUnion.end(); it++)
				if(it->position[1]>-8.0)
				{
					setDiffuseColor(COLOR_RED);
					glPushMatrix();
					glTranslated(it->position[0], it->position[1], it->position[2]);
					drawSphere(particleRadius);
					glPopMatrix();
				}

		}
	// TODO
}





/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{
	bakeInfo.insert(pair<float, vector<Particle>>(t, particleUnion));
	// TODO
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{
	bakeInfo.clear();
	// TODO
}





