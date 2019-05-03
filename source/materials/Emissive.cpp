#include "raytracing/materials/Emissive.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

Emissive::Emissive()
	: m_radiance_scale_factor(40)
	, m_color(WHITE)
{
}

RGBColor Emissive::Shade(const ShadeRec& sr) const
{
	if (-sr.normal * sr.ray.dir > 0.0) {
		return m_radiance_scale_factor * m_color;
	} else {
		return BLACK;
	}
}

RGBColor Emissive::AreaLightShade(const ShadeRec& sr) const
{
	return Shade(sr);
}

RGBColor Emissive::GetLe(const ShadeRec& sr) const
{
	return m_radiance_scale_factor * m_color;
}

}