#include "raytracing/texture/NestedNoisesTexture.h"
#include "raytracing/noise/LatticeNoise.h"
#include "raytracing/texture/Texture.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

NestedNoisesTexture::NestedNoisesTexture(float min_val, float max_val,
                                         float exp, float wrap_factor, const RGBColor& color)
    : m_min_value(min_val)
    , m_max_value(max_val)
    , m_expansion_number(exp)
    , m_wrap_factor(wrap_factor)
    , m_color(color)
{
}

RGBColor NestedNoisesTexture::GetColor(const ShadeRec& sr) const
{
	/*
	float noise = expansion_number * noise_ptr->value_fbm(sr.local_hit_point);
	float value = noise - floor(noise);
	value = min_value + (max_value - min_value) * value;

	if (noise < 1.0)
		return (value * color1);
	else
		return (value * color2);*/
	float noise = m_wrap_factor * m_noise->value_fbm(sr.local_hit_point);
	float value = noise - floor(noise);
	value = (m_max_value - m_min_value) * value + m_min_value;

	if (noise < 1.0)
		return (value * m_color);
	else
		return (value * m_texture->GetColor(sr));
}

}