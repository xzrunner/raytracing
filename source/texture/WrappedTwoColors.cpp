#include "raytracing/texture/WrappedTwoColors.h"
#include "raytracing/noise/LatticeNoise.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

WrappedTwoColors::WrappedTwoColors(float min_val, float max_val, float exp)
    : m_min_value(min_val)
    , m_max_value(max_val)
    , m_expansion_number(exp)
{
}

RGBColor WrappedTwoColors::GetColor(const ShadeRec& sr) const
{
	float noise = m_expansion_number * m_noise->value_fbm(sr.local_hit_point);
	float value = noise - floor(noise);
	value = m_min_value + (m_max_value - m_min_value) * value;

	if (noise < 1.0)
		return (value * m_color1);
	else
		return (value * m_color2);
}

}