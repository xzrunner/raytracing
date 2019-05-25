#pragma once

#include "raytracing/noise/LatticeNoise.h"

namespace rt
{

class LinearNoise : public LatticeNoise
{
public:
    LinearNoise() {}
    LinearNoise(int octaves, float lacunarity, float gain);

    virtual float value_noise(const Point3D& p) const override;

    virtual Vector3D vector_noise(const Point3D& p) const override;

}; // LinearNoise

// Templated lerp function that can be used for floats and vectors.
// This is not a class member function.
template<class T> T
lerp(const float f, const T& a, const T& b) {
    return (a + f * (b - a));
}

}