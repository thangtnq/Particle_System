// ----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Optimized C++
//----------------------------------------------------------------------------

#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Math\Matrix.h"
#include "Math\Vect4D.h"
#include "Particle.h"

#include <list>

class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();

	void SpawnParticle();
	void update();
	void draw();

	void addParticleToList(Particle *p);
	void removeParticleFromList(Particle *p);

	void Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc);

private:

	Vect4D	start_position;
	Vect4D	start_velocity;
	Vect4D	vel_variance;
	Vect4D	pos_variance;
	float	max_life;
	float	scale_variance;
	float	spawn_frequency;
	float	last_spawn;
	float	last_loop;


	// added for speed efficiency
	
	Particle *headParticle;
	int		max_particles;
	int		last_active_particle;
	
	

};

#endif // PARTICLEEMITTER_H
