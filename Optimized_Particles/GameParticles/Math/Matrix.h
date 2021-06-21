// ----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Optimized C++
//----------------------------------------------------------------------------

#ifndef MATRIX_H
#define MATRIX_H

// includes
#include "Enum.h"
#include<xmmintrin.h>
#include<smmintrin.h>
#include"Vect4D.h"

// forward declare
class Vect4D;

// class
class Matrix
{
public:

	// local enumerations
	enum MatrixRowEnum
	{
		MATRIX_ROW_0,
		MATRIX_ROW_1,
		MATRIX_ROW_2,
		MATRIX_ROW_3
	};

	Matrix();
	Matrix(const Matrix& t);
	Matrix(const Vect4D &a, const Vect4D &b, const Vect4D &c, const Vect4D &d);
	~Matrix();

	void set(const MatrixRowEnum row, const Vect4D *t);
	void get(const MatrixRowEnum row, Vect4D *t) const;

	void setIdentMatrix();
	void setTransMatrix(const Vect4D *const);
	void setScaleMatrix(const Vect4D *const);
	void setRotZMatrix(const float Z_Radians);

	float &operator[](INDEX_ENUM e);

	const Matrix operator*(const Matrix &t) const;

	float Determinant();

	Matrix GetAdjugate();
	const Matrix& Matrix::operator/=(const float t);

	void Matrix::Inverse(Matrix &out);

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
	union {
		struct {
			Vect4D v0;
			Vect4D v1;
			Vect4D v2;
			Vect4D v3;
		};
		struct {
			// ROW 0
			float m0;
			float m1;
			float m2;
			float m3;

			// ROW 1
			float m4;
			float m5;
			float m6;
			float m7;

			// ROW 2
			float m8;
			float m9;
			float m10;
			float m11;

			// ROW 3
			float m12;
			float m13;
			float m14;
			float m15;
		};
	};
};

#endif  // Matrix.h
