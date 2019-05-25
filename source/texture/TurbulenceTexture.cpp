#include "raytracing/texture/TurbulenceTexture.h"
#include "raytracing/noise/LatticeNoise.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

TurbulenceTexture::TurbulenceTexture(const RGBColor& color, float min_val, float max_val)
    : color(color)
    , min_value(min_val)
    , max_value(max_val)
{
}

RGBColor TurbulenceTexture::GetColor(const ShadeRec& sr) const
{
    float value = noise->value_fbm(sr.local_hit_point);     // in the range (0, 1)
    value = min_value + (max_value - min_value) * value;	// in the range (min_value, max_value)

    return (value * color);
}

}