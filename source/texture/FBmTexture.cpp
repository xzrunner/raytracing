#include "raytracing/texture/FBmTexture.h"
#include "raytracing/noise/LatticeNoise.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

RGBColor FBmTexture::GetColor(const ShadeRec& sr) const
{
    float value = noise->value_fbm(sr.local_hit_point); // in the range (0, 1)
    value = min_value + (max_value - min_value) * value;	// in the range (min_value, max_value)

    return (value * color);
}

}