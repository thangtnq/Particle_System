// ----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Optimized C++
//----------------------------------------------------------------------------

#include "Particle.h"
#include "Matrix.h"

Particle::Particle()
{
	// construtor
	this->life = 0.0f;
	this->position.set(0.0f, 0.0f, 0.0f);
	this->velocity.set(0.0f, 0.0f, 0.0f);
	this->scale.set(1.0f, 1.0f, 1.0f);
	this->rotation = 0.0f;
	this->rotation_velocity = -0.5f;
}

Particle::Particle(Vect4D v, Vect4D p, Vect4D s) {
	this->life = 0.0f;
	this->position = p;
	this->velocity = v;
	this->scale = s;
	this->rotation = 0.0f;
	this->rotation_velocity = -0.5f;
}

Particle::~Particle()
{
	// nothing to do
}

void Particle::CopyDataOnly(Particle *p)
{
	// copy the data only
	// this way of copying data is more efficient that element by element
	this->position = p->position;
	this->velocity = p->velocity;
	this->scale = p->scale;
	this->rotation = p->rotation;
	this->rotation_velocity = p->rotation_velocity;
	this->life = p->life;
}

void Particle::Update(const float& time_elapsed)
{
	life += time_elapsed;
	position = position + (velocity * time_elapsed);
	Vect4D z_axis(0.0f, -0.25f, 1.0f);
	Vect4D v(3, 4, 0);
	position.Cross(z_axis, v);
	v.norm(v);
	position = position + v * 0.05 * life;

	rotation = rotation + rotation_velocity * time_elapsed *2.01;
}


// End of file


