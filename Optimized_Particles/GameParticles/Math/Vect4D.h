// ----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Optimized C++
//----------------------------------------------------------------------------

// This is a 4 dimensional Vect4D class
#ifndef Vect4D_H
#define Vect4D_H

// includes
#include "Enum.h"
#include<xmmintrin.h>
#include<smmintrin.h>


// Foward Declarations
class Matrix;

// class
class Vect4D
{
public:
	friend class Matrix;

	Vect4D();
	Vect4D(const float tx, const float ty, const float tz, const float tw = 1.0f);
	Vect4D(const __m128);
	~Vect4D();

	void norm(Vect4D &out);
	void set(const float &tx, const float &ty,const float &tz, const float &tw = 1.0f);

	Vect4D operator + (const Vect4D &t) const;
	Vect4D operator - (const Vect4D &t) const;
	const Vect4D operator * (const float scale) const;

	void Cross(const Vect4D &vin, Vect4D &vout);

	float &operator[](VECT_ENUM e);
	/*Vect4D &operator=(const Vect4D&);*/

private:
	union {
		__m128 _m;
		struct{
			float x;
			float y;
			float z;
			float w;
		};
	};
	
};

#endif  //Vect4D.h
