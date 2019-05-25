#pragma once

#include "raytracing/noise/LatticeNoise.h"

namespace rt
{

class CubicNoise : public LatticeNoise
{
public:
    CubicNoise() {}
    CubicNoise(int octaves);
    CubicNoise(int octaves, float lacunarity, float gain);

    virtual float value_noise(const Point3D& p) const override;

    virtual Vector3D vector_noise(const Point3D& p) const override;

}; // CubicNoise

// Cubic interpolation using a Catmull-Rom four knot spline.
// This is templated so that we can use it to interpolate floats and Vector3Ds.
// This is not a class member function.
template<class T> T
four_knot_spline(const float x, const T knots[]) {
	T c3 = -0.5 * knots[0] + 1.5 * knots[1] - 1.5 * knots[2] + 0.5 * knots[3];
  	T c2 = knots[0] - 2.5 * knots[1] + 2.0 * knots[2] - 0.5 * knots[3];
  	T c1 = 0.5 * (-knots[0] + knots [2]);
  	T c0 = knots[1];

    return (T((c3*x + c2)*x + c1)*x + c0);
}

}