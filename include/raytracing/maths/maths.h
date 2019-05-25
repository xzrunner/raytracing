#pragma once

#include "raytracing/utilities/RGBColor.h"

#include <stdlib.h>

namespace rt
{

static const float INV_RAND_MAX = 1.0f / (float)RAND_MAX;

inline int rand_int() {
	return(rand());
}

inline float
rand_float(void) {
	return (float)(rand_int() * INV_RAND_MAX);
}

inline float
rand_float(float l, float h) {
	return rand_float() * (h - l) + l;
}

inline int
rand_int(int l, int h) {
	return (int) (rand_float(0.0f, h - l + 1.0f) + l);
}

inline void
set_rand_seed(int seed) {
	srand(seed);
}

inline double
clamp(const double x, const double min, const double max) {
	return x < min ? min : (x > max ? max : x);
}

inline double
smooth_step(double a, double b, double x) {
	if (x < a)
		return (0.0);

	if (x >= b)
		return (1.0);

	double y = (x - a) / ( b - a);  // normalise to [0, 1]

	return (y * y * (3.0 - 2.0 * y));
}

inline double
smooth_pulse(double e0, double e1, double e2, double e3, double x) {
	return (smooth_step (e0, e1, x) - smooth_step (e2, e3, x));
}

inline double
mod(double a, const double b) {
	int n = (int) (a / b);

	a -= n * b;
	if (a < 0.0)
		a += b;
return (a);
}

inline double
smooth_pulse_train(double e0, double e1, double e2, double e3, double period, double x) {
	return (smooth_pulse(e0, e1, e2, e3, mod(x, period)));
}
inline RGBColor
mix_color(const RGBColor& c0, const RGBColor& c1, const double f) {
    return (c0 * (1.0 - f) + c1 * f);
}

inline double
mix_double(const double a, const double b, const double f) {
    return ((1.0 - f) * a + f * b);
}

int SolveQuadric(double c[3], double s[2]);
int SolveCubic(double c[4], double s[3]);
int SolveQuartic(double c[5], double s[4]);

}
