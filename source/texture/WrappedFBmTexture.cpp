#include "raytracing/texture/WrappedFBmTexture.h"
#include "raytracing/noise/LatticeNoise.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

WrappedFBmTexture::WrappedFBmTexture(float min_val, float max_val, float exp)
    : m_min_value(min_val)
    , m_max_value(max_val)
    , m_expansion_number(exp)
{
}

RGBColor WrappedFBmTexture::GetColor(const ShadeRec& sr) const
{
    float noise_val = m_expansion_number * m_noise->value_fbm(sr.local_hit_point);
    float value = noise_val - floor(noise_val);
    value = m_min_value + (m_max_value - m_min_value) * value;

    return (value * m_color);
}

}