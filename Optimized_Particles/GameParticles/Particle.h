// ----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Optimized C++
//----------------------------------------------------------------------------

#ifndef PARTICLE_H
#define PARTICLE_H

// include
#include "Vect4D.h"

class Particle
{
public:
	friend class ParticleEmitter;

	Particle();
	Particle(Vect4D, Vect4D, Vect4D);
	~Particle();
	void Update(const float& time_elapsed);
	void CopyDataOnly(Particle *p);

	void *operator new(size_t size)
	{
		void *p = _aligned_malloc(size, 16);
		return p;
	}

	void *operator new[](size_t size)
	{
		void *p = _aligned_malloc(size, 16);
		return p;
	}

		void operator delete(void *p)
	{
		_aligned_free(p);
	}

	void operator delete[](void *p)
	{
		_aligned_free(p);
	}
	

	
private:
	
	

	Vect4D	position;
	Vect4D	velocity;
	Vect4D	scale;
	float	rotation;
	float	life;
	float	rotation_velocity;

	Particle *next;
	Particle *prev;

};


#endif //PARTICLE_H
