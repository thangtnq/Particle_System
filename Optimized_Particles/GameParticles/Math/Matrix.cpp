// ----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Optimized C++
//----------------------------------------------------------------------------

#include <Math.h>
#include <assert.h>

#include "Vect4D.h"
#include "Matrix.h"

Matrix::Matrix()
{	// constructor for the matrix
	v0._m = _mm_set1_ps(0.0f);
	v1._m = _mm_set1_ps(0.0f);
	v2._m = _mm_set1_ps(0.0f);
	v3._m = _mm_set1_ps(0.0f);

}

Matrix::Matrix(const Matrix& t)
{ // copy constructor
	v0._m = _mm_set_ps(t.m3, t.m2, t.m1, t.m0);
	v1._m = _mm_set_ps(t.m7, t.m6, t.m5, t.m4);
	v2._m = _mm_set_ps(t.m11, t.m10, t.m9, t.m8);
	v3._m = _mm_set_ps(t.m15, t.m14, t.m13, t.m12);
}

Matrix::Matrix(const Vect4D &a, const Vect4D &b, const Vect4D &c, const Vect4D &d):
	v0(a), v1(b), v2(c),v3(d)
{}

Matrix::~Matrix()
{
	// nothign to delete
}

void Matrix::setIdentMatrix()
{ // initialize to the identity matrix
	v0._m = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);
	v1._m = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);
	v2._m = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
	v3._m = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
}

void Matrix::setTransMatrix(const Vect4D *t)
{ // set the translation matrix (note: we are row major)
	v0._m = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);
	v1._m = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);
	v2._m = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
	v3._m = _mm_set_ps(1.0f, t->z, t->y, t->x);
	
}

void Matrix::set(const MatrixRowEnum row, Vect4D const *t)
{
	// initialize the rows of the matrix
	switch (row)
	{
	case MATRIX_ROW_0:
		this->m0 = t->x;
		this->m1 = t->y;
		this->m2 = t->z;
		this->m3 = t->w;
		break;

	case MATRIX_ROW_1:
		this->m4 = t->x;
		this->m5 = t->y;
		this->m6 = t->z;
		this->m7 = t->w;
		break;

	case MATRIX_ROW_2:
		this->m8 = t->x;
		this->m9 = t->y;
		this->m10 = t->z;
		this->m11 = t->w;
		break;

	case MATRIX_ROW_3:
		this->m12 = t->x;
		this->m13 = t->y;
		this->m14 = t->z;
		this->m15 = t->w;
		break;

	default:
		// should never get here, if we are here something bad has happened
		assert(0);
	}
}

float &Matrix::operator[](INDEX_ENUM e)
{
	// get the individual elements
	switch (e)
	{
	case 0:
		return m0;
		break;
	case 1:
		return m1;
		break;
	case 2:
		return m2;
		break;
	case 3:
		return m3;
		break;
	case 4:
		return m4;
		break;
	case 5:
		return m5;
		break;
	case 6:
		return m6;
		break;
	case 7:
		return m7;
		break;
	case 8:
		return m8;
		break;
	case 9:
		return m9;
		break;
	case 10:
		return m10;
		break;
	case 11:
		return m11;
		break;
	case 12:
		return m12;
		break;
	case 13:
		return m13;
		break;
	case 14:
		return m14;
		break;
	case 15:
		return m15;
		break;
	default:
		assert(0);
		return m0;
		break;
	}
}

void Matrix::get(const MatrixRowEnum row, Vect4D *t) const
{ // get a row of the matrix
	switch (row)
	{
	case MATRIX_ROW_0:
		t->_m = _mm_set_ps(m3, m2, m1, m0);
		break;

	case MATRIX_ROW_1:
		t->_m = _mm_set_ps(m7, m6, m5, m4);
		break;

	case MATRIX_ROW_2:
		t->_m = _mm_set_ps(m11, m10, m9, m8);
		break;

	case MATRIX_ROW_3:
		t->_m = _mm_set_ps(m15, m14, m13, m12);
		break;

	default:
		assert(0);
	}
}

const Matrix Matrix::operator*(const Matrix& mb) const
{ // matrix multiplications

	__m128 x1 = _mm_set1_ps(m0);
	__m128 y1 = _mm_set1_ps(m1);
	__m128 z1 = _mm_set1_ps(m2);
	__m128 w1 = _mm_set1_ps(m3);

	__m128 a0 = _mm_mul_ps(mb.v0._m, x1);
	__m128 a1 = _mm_mul_ps(mb.v1._m, y1);
	__m128 a2 = _mm_mul_ps(mb.v2._m, z1);
	__m128 a3 = _mm_mul_ps(mb.v3._m, w1);

	__m128 a_add_1 = _mm_add_ps(a0, a1);
	__m128 a_add_2 = _mm_add_ps(a_add_1, a2);

	//vect 1
	__m128 x2 = _mm_set1_ps(m4);
	__m128 y2 = _mm_set1_ps(m5);
	__m128 z2 = _mm_set1_ps(m6);
	__m128 w2 = _mm_set1_ps(m7);

	__m128 b0 = _mm_mul_ps(mb.v0._m, x2);
	__m128 b1 = _mm_mul_ps(mb.v1._m, y2);
	__m128 b2 = _mm_mul_ps(mb.v2._m, z2);
	__m128 b3 = _mm_mul_ps(mb.v3._m, w2);

	__m128 b_add_1 = _mm_add_ps(b0, b1);
	__m128 b_add_2 = _mm_add_ps(b_add_1, b2);

	//vect 2
	__m128 x3 = _mm_set1_ps(m8);
	__m128 y3 = _mm_set1_ps(m9);
	__m128 z3 = _mm_set1_ps(m10);
	__m128 w3 = _mm_set1_ps(m11);

	__m128 c0 = _mm_mul_ps(mb.v0._m, x3);
	__m128 c1 = _mm_mul_ps(mb.v1._m, y3);
	__m128 c2 = _mm_mul_ps(mb.v2._m, z3);
	__m128 c3 = _mm_mul_ps(mb.v3._m, w3);

	__m128 c_add_1 = _mm_add_ps(c0, c1);
	__m128 c_add_2 = _mm_add_ps(c_add_1, c2);
	//vect 3
	__m128 x4 = _mm_set1_ps(m12);
	__m128 y4 = _mm_set1_ps(m13);
	__m128 z4 = _mm_set1_ps(m14);
	__m128 w4 = _mm_set1_ps(m15);

	__m128 d0 = _mm_mul_ps(mb.v0._m, x4);
	__m128 d1 = _mm_mul_ps(mb.v1._m, y4);
	__m128 d2 = _mm_mul_ps(mb.v2._m, z4);
	__m128 d3 = _mm_mul_ps(mb.v3._m, w4);

	__m128 d_add_1 = _mm_add_ps(d0, d1);
	__m128 d_add_2 = _mm_add_ps(d_add_1, d2);

	return Matrix(Vect4D(_mm_add_ps(a_add_2, a3)), Vect4D(_mm_add_ps(b_add_2, b3)),
		Vect4D(_mm_add_ps(c_add_2, c3)), Vect4D(_mm_add_ps(d_add_2, d3)));
}

const Matrix& Matrix::operator/=(float rhs)
{
	// divide each element by a value
	// using inverse multiply trick, faster that individual divide
	float asd = 1 / rhs;
	__m128 a = _mm_set1_ps(asd);
	v0._m = _mm_div_ps(v0._m, a);
	v1._m = _mm_div_ps(v1._m, a);
	v2._m = _mm_div_ps(v2._m, a);
	v3._m = _mm_div_ps(v3._m, a);
	
	return *this;
}

float Matrix::Determinant()
{

	// ta = (lq - mp)
	float ta = (m10 * m15) - (m11 * m14);
	// tb = (kq - mo)
	float tb = (m9 * m15) - (m11 * m13);
	// tc = (kp - lo)
	float tc = (m9 * m14) - (m10 * m13);
	// td = (jq - mn)
	float td = (m8 * m15) - (m11 * m12);
	// te = (jo - kn)
	float te = (m8 * m13) - (m9 *  m12);
	// tf = (jp - ln)
	float tf = (m8 * m14) - (m10 * m12);

	return ((m0 * ((m5 * ta) - (m6 * tb) + (m7 * tc)))
			- (m1 * ((m4 * ta) - (m6 * td) + (m7 * tf)))
			+ (m2 * ((m4 * tb) - (m5 * td) + (m7 * te)))
			- (m3 * ((m4 * tc) - (m5 * tf) + (m6 * te))));

}

Matrix Matrix::GetAdjugate()
{
	Matrix tmp;

	// load		ABC		(3)		ABC--
	float t1 = (m10*m15) - (m11*m14);
	float t2 = (m9*m15) - (m11*m13);
	float t3 = (m9*m14) - (m10*m13);

	// a = f(ta) - g(tb) + h(tc)
	tmp.m0 = (m5*t1) - (m6*t2) + (m7*t3);
	// b = -( b(ta) - c(tb) + d(tc))
	tmp.m1 = -((m1*t1) - (m2*t2) + (m3*t3));

	// load		JK		(5)		ABCJK
	float t4 = (m8*m15) - (m11*m12);
	float t5 = (m8*m14) - (m10*m12);
	// e = - ( e(ta) - g(tj) + h(tk))
	tmp.m4 = -((m4*t1) - (m6*t4) + (m7*t5));
	// f = a(ta) - c(tj) + d(tk)
	tmp.m5 = (m0*t1) - (m2*t4) + (m3*t5);

	// unload	AJ		(3)		-BC-K
	// load		P		(4)		PBC-K
	t1 = (m8*m13) - (m9*m12);
	// n = -( e(tc) - f(tk) + g(tp))
	tmp.m12 = -((m4*t3) - (m5*t5) + (m6*t1));
	// o = a(tc) - b(tk) + c(tp)
	tmp.m13 = (m0*t3) - (m1*t5) + (m2*t1);

	// unload	KC		(2)		PB---
	// load		J		(3)		PBJ--
	t3 = (m8*m15) - (m11*m12);

	// j = e(tb) - f(tj) + h(tp)
	tmp.m8 = (m4*t2) - (m5*t3) + (m7*t1);
	// k = - ( a(tb) - b(tj) + d(tp))
	tmp.m9 = -((m0*t2) - (m1*t3) + (m3*t1));

	// unload	BPJ		(0)		-----
	// load		DLM		(3)		DLM--
	t1 = (m6*m15) - (m7*m14);
	t2 = (m4*m15) - (m7*m12);
	t3 = (m4*m14) - (m6*m12);

	// g = - ( a(td) - c(tl) + d(tm))
	tmp.m6 = -((m0*t1) - (m2*t2) + (m3*t3));

	// load		FR		(5)		DLMFR
	t4 = (m5*m14) - (m6*m13);
	t5 = (m4*m13) - (m5*m12);

	// p = - ( a(tf) - b(tm) + c(tr))
	tmp.m14 = -((m0*t4) - (m1*t3) + (m3*t5));

	// unload	M		(4)		DL-FR
	// load		E		(5)		DLEFR
	t3 = (m5*m15) - (m7*m13);

	// l = a(te) - b(tl) + d(tr)
	tmp.m10 = (m0*t3) - (m1*t2) + (m3*t5);

	// unload	LR		(3)		D-EF-
	// c = b(td) - c(te) + d(tf)
	tmp.m2 = (m1*t1) - (m2*t3) + (m3*t4);

	// unload	DEF		(0)		-----
	// load		GHI		(3)		GHI--
	t1 = (m6*m11) - (m7*m10);
	t2 = (m5*m11) - (m7*m9);
	t3 = (m5*m10) - (m6*m9);

	// d = -( b(tg) - c(th) + d(ti) )
	tmp.m3 = -((m1*t1) - (m2*t2) + (m3*t3));

	// load		NO		(5)		GHINO
	t4 = (m4*m11) - (m7*m8);
	t5 = (m4*m10) - (m6*m8);

	// h = a(tg) - c(tn) + d(to)
	tmp.m7 = (m0*t1) - (m2*t4) + (m3*t5);

	// unload	G		(4)		-HINO
	// load		Q		(5)		QHINO
	t1 = (m4*m9) - (m5*m8);

	// m = -( a(th) - b(tn) + d(tq))
	tmp.m11 = -((m0*t2) - (m1*t4) + (m3*t1));

	// unload	HN		(3)		Q-I-O
	// q = a(ti) - b(to) + c(tq)
	tmp.m15 = (m0*t3) - (m1*t5) + (m2*t1);

	return tmp;
}

void Matrix::Inverse(Matrix &out)
{
	Matrix tmp;
	float det = Determinant();
	if (fabs(det) < 0.0001)
	{
		// do nothing, Matrix is not invertable
	}
	else
	{
		tmp = GetAdjugate();
		tmp /= det;
	}

	out = tmp;
}

void Matrix::setScaleMatrix(const Vect4D *scale)
{
	//	{	sx		0		0		0	}
	//	{	0		sy		0		0	}
	//	{	0		0		sz		0	}
	//	{	0		0		0		1	}

	v0._m = _mm_set_ps(0.0f, 0.0f, 0.0f, scale->x);
	v1._m = _mm_set_ps(0.0f, 0.0f, scale->y, 0.0f);
	v2._m = _mm_set_ps(0.0f, scale->z, 0.0f, 0.0f);
	v3._m = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
	
}

void Matrix::setRotZMatrix(float az)
{
	//	{	cos		-sin	0		0	}
	//	{	sin		cos		0		0	}
	//	{	0		0		1		0	}
	//	{	0		0		0		1	}

	v0._m = _mm_set_ps(0.0f, 0.0f, -sin(az), cos(az));
	v1._m = _mm_set_ps(0.0f, 0.0f, cos(az), sin(az));
	v2._m = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
	v3._m = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);

}

// End of file