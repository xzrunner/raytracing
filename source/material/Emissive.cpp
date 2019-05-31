#include "raytracing/material/Emissive.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

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

RGBColor Emissive::PathShade(ShadeRec& sr) const
{
    return sr.depth == 1 ? BLACK : Shade(sr);
}

RGBColor Emissive::GlobalShade(ShadeRec& sr) const
{
    return Shade(sr);
}

}