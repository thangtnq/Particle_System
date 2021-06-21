// ----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Optimized C++
//-----------------------------------------------------------------------------

#include "DO_NOT_MODIFY\Timer.h"
#include "DO_NOT_MODIFY\GlobalTimer.h"
#include "DO_NOT_MODIFY\OpenGLInterface.h"

#include <assert.h>

#define UNUSED_VAR(v) ((void *)v)
#include "ParticleEmitter.h"
#include "Settings.h"

static const unsigned char squareColors[] =
{
	200,  200,  0,  255,
	200,  200,  0,  255,
	200,  200,  0,  255,
	200,  200,  0,  255,
};

static const float squareVertices[] =
{
	-0.015,  -0.015, 0.0,
	 0.015,  -0.015, 0.0,
	-0.015,   0.015, 0.0,
	 0.015,   0.015, 0.0,
};

ParticleEmitter::ParticleEmitter()
	: start_position(0.0, 0.0, 0.0),
	start_velocity(0.0, 1.0, 0.0),
	max_life(MAX_LIFE),
	max_particles(NUM_PARTICLES),
	spawn_frequency(0.0000001),
	last_spawn(globalTimer::getTimerInSec()),
	last_loop(globalTimer::getTimerInSec()),
	last_active_particle(-1),
	vel_variance(1.0, 4.0, 0.4),
	pos_variance(1.0, 1.0, 1.0),
	scale_variance(2.5),
	headParticle(0)
{
	// nothing to do
}

ParticleEmitter::~ParticleEmitter()
{
	// do nothing
}

void ParticleEmitter::SpawnParticle()
{
	Particle* newParticle = new Particle(start_velocity, start_position, Vect4D(2.0, 2.0, 2.0, 1.0));

	this->Execute(newParticle->position, newParticle->velocity, newParticle->scale);
	last_active_particle++;
	this->addParticleToList(newParticle);
}

void ParticleEmitter::update()
{
	float current_time = globalTimer::getTimerInSec();
	float time_elapsed = current_time - last_spawn;

	int size = max_particles - 1;

	while (last_active_particle < size) {
		this->SpawnParticle();
	}

	last_spawn = current_time;
	time_elapsed = current_time - last_loop;

	Particle *p = this->headParticle;
	
	while (p != 0)
	{
		p->Update(time_elapsed);

		if ((last_active_particle > 0) && (p->life > max_life))
		{
			Particle *s = p;
			p = p->next;
			this->removeParticleFromList(s);
			last_active_particle--;
		}
		else
		{
			p = p->next;
		}
	}

	last_loop = current_time;
}

void ParticleEmitter::addParticleToList(Particle *p)
{
	assert(p);
	if (this->headParticle == 0)
	{ // first on list
		this->headParticle = p;
		p->next = 0;
		p->prev = 0;
	}
	else
	{ // add to front of list
		headParticle->prev = p;
		p->next = headParticle;
		p->prev = 0;
		headParticle = p;
	}

}

void ParticleEmitter::removeParticleFromList(Particle *p)
{
	// make sure we are not screwed with a null pointer
	assert(p);

	if (p->prev == 0 && p->next == 0)
	{ // only one on the list
		this->headParticle = 0;
	}
	else if (p->prev == 0 && p->next != 0)
	{ // first on the list
		p->next->prev = 0;
		this->headParticle = p->next;
	}
	else if (p->prev != 0 && p->next == 0)
	{ // last on the list 
		p->prev->next = 0;
	}
	else//( p->next != 0  && p->prev !=0 )
	{ // middle of the list
		p->prev->next = p->next;
		p->next->prev = p->prev;
	}

	// bye bye
	delete p;
}


void ParticleEmitter::draw()
{
	Matrix cameraMatrix;

	glGetFloatv(GL_MODELVIEW_MATRIX, reinterpret_cast<float*>(&cameraMatrix));

	glVertexPointer(3, GL_FLOAT, 0, squareVertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, squareColors);
	glEnableClientState(GL_COLOR_ARRAY);

	Matrix rotParticle, transParticle;
	rotParticle.setRotZMatrix(this->headParticle->rotation);

	Vect4D camPosVect;
	cameraMatrix.get(Matrix::MATRIX_ROW_3, &camPosVect);

	Matrix transCamera;
	transCamera.setTransMatrix(&camPosVect);

	Particle* it = this->headParticle;

	
	while( it != nullptr)
	{
		transParticle.setTransMatrix(&it->position);

		Matrix scaleMatrix;
		scaleMatrix.setScaleMatrix(&it->scale);
	
		glLoadMatrixf(reinterpret_cast<float*>(&(Matrix(scaleMatrix * transCamera * transParticle * rotParticle* scaleMatrix))));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		it = it->next;
	
	}	
}

void ParticleEmitter::Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc)
{
	// Add some randomness...

	// x - variance
	
	float var = static_cast<float>(rand() % 1000) * 0.001f;
	float sign = static_cast<float>(rand() % 2);
	float *t_pos = reinterpret_cast<float*>(&pos);
	float *t_var = &pos_variance[x];
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	// y - variance
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	// z - variance
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);

	// x  - add velocity
	t_pos = &vel[x];
	t_var = &vel_variance[x];
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	// y - add velocity
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	// z - add velocity
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	// correct the sign
	var = 2.0f * static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);

	if (sign == 0)
	{
		var *= -1.0;
	}
	
	sc = sc * var;
}


// End of file