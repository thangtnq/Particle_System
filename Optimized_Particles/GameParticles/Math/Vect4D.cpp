// ----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Optimized C++
//----------------------------------------------------------------------------

#include <math.h>
#include <assert.h>

#include "Vect4D.h"

Vect4D::Vect4D()
{
	_m = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
}

Vect4D::Vect4D(float tx, float ty, float tz, float tw)
{
	_m = _mm_set_ps(tw,tz,ty,tx);
}

Vect4D::Vect4D(__m128 a):
	_m(a)
{}

Vect4D::~Vect4D()
{
	// nothing to delete
}

void Vect4D::norm(Vect4D& out)
{
	__m128 mag = _mm_sqrt_ps(_mm_dp_ps(_m, _m, 0X77));

	if (0.0f < _mm_cvtss_f32(mag))
	{
		out._m = _mm_div_ps(_m, mag);
		out.w = 1.0f;
	}
}

//Vect4D &Vect4D::operator=(const Vect4D& a) {
//	Vect4D sad;
//	sad._m = _mm_set_ps(a.w, a.z, a.y, a.x);
//	return sad;
//}

Vect4D Vect4D::operator + (const Vect4D &t) const
{
	Vect4D out;

	out._m = _mm_add_ps(_m, t._m);
	out.w = 1.0f;

	return out;
}

Vect4D Vect4D::operator - (const Vect4D &t) const
{
	Vect4D out;

	out._m = _mm_sub_ps(_m, t._m);
	out.w = 1.0f;

	return out;
}

const Vect4D Vect4D::operator *(const float scale) const
{
	Vect4D tmp;

	tmp.x = this->x * scale;
	tmp.y = this->y * scale;
	tmp.z = this->z * scale;

	return tmp;

}

float& Vect4D::operator[](VECT_ENUM e)
{
	switch (e)
	{
	case 0:
		return x;
		break;
	case 1:
		return y;
		break;
	case 2:
		return z;
		break;
	case 3:
		return w;
		break;
	default:
		assert(0);
		return x;
	}
}

void Vect4D::Cross(const Vect4D& vin, Vect4D& vout)
{
	vout._m = _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(_m, _m, _MM_SHUFFLE(3, 0, 2, 1)),
									_mm_shuffle_ps(vin._m, vin._m, _MM_SHUFFLE(3, 1, 0, 2))),
									_mm_mul_ps(_mm_shuffle_ps(_m, _m, _MM_SHUFFLE(3, 1, 0, 2)),
									_mm_shuffle_ps(vin._m, vin._m, _MM_SHUFFLE(3, 0, 2, 1))));
	
}

void Vect4D::set(const float &tx, const float &ty, const float &tz, const float &tw)
{
	_m = _mm_set_ps(tw, tz, ty, tx);
}

// End of file