#pragma once
#ifndef GameMath_h__
#define GameMath_h__

#include "glm.hpp"

using namespace glm;

inline int next_p2( int a )
{
	int rval = 1;
	while( rval < a ) rval <<= 1;
	return rval;
}

template < typename T >
inline T fade( T t )
{
	return t * t * t * ( t * ( t * 6 - 15 ) + 10 );
}

template < typename T >
inline T MAX(T a, T b){
	return a > b ? a : b;
}

template < typename T >
inline T MIN(T a, T b){
	return a < b ? a : b;
}

template < typename T >
inline bool isInRange(T some, T min, T max){
	return (some >= min && some <= max);
}

inline unsigned int next_p2( unsigned int a )
{
	unsigned int rval = 1;
	while( rval < a ) rval <<= 1;
	return rval;
}

struct iRect
{
	int x;
	int y;
	int w;
	int h;

	iRect()
	{
		x = 0; y = 0; w = 0; h = 0;
	}

	iRect(int x_, int y_, int w_, int h_)
	{
		x = x_;
		y = y_;
		w = w_;
		h = h_;
	}
};

template <typename T>
inline T sqr(T value) {
	return value*value;
}

/**
* Cubic Hermite spline.
*/
template <typename T>
inline T spline(T x, size_t nknots, T *knots)
{
	int span;
	int nspans = nknots - 3;
	T c0, c1, c2, c3; // coefficients of the cubic.

	assert(nspans >= 1);

	/* find the appropriate 4-point span of the spline. */
	x = glm::clamp(x, 0, 1) * nspans;
	span = (int) x;

	if (span >= nknots - 3) {
		span = nknots - 3;
	}

	x -= span;
	knots += span;

	/* Coefficients of basis matrix. */
#define CR00 -0.5
#define CR01 1.5
#define CR02 -1.5
#define CR03 0.5
#define CR10 1.0
#define CR11 -2.5
#define CR12 2.0
#define CR13 -0.5
#define CR20 -0.5
#define CR21 0.0
#define CR22 0.5
#define CR23 0.0
#define CR30 0.0
#define CR31 1.0
#define CR32 0.0
#define CR33 0.0

	/* Evaluate the span cubic at x using Horner's rule. */
	c3 = CR00*knots[0] + CR01*knots[1] + CR02*knots[2] + CR03*knots[3];
	c2 = CR10*knots[0] + CR11*knots[1] + CR12*knots[2] + CR13*knots[3];
	c1 = CR20*knots[0] + CR21*knots[1] + CR22*knots[2] + CR23*knots[3];
	c0 = CR30*knots[0] + CR31*knots[1] + CR32*knots[2] + CR33*knots[3];

	return ((c3*x + c2)*x + c1)*x + c0;
}

/**
* Extension to spline function to work with colors.
*/
template <typename T>
inline glm::vec3 spline(T param, size_t nknots, glm::vec3 *knots)
{
	float x[nknots];
	float y[nknots];
	float z[nknots];

	for (size_t i=0; i<nknots; ++i) {
		x[i] = knots[i].x;
		y[i] = knots[i].y;
		z[i] = knots[i].z;
	}

	return Vec3<T>(
		spline<T>(param, nknots, x),
		spline<T>(param, nknots, y),
		spline<T>(param, nknots, z));
}



#endif // GameMath_h__