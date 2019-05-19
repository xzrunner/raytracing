#pragma once

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

int SolveQuadric(double c[3], double s[2]);
int SolveCubic(double c[4], double s[3]);
int SolveQuartic(double c[5], double s[4]);

}
