#pragma once

#include "raytracing/materials/Material.h"

namespace rt
{

class Emissive : public Material
{
public:
	Emissive();

	virtual RGBColor Shade(const ShadeRec& sr) const;
	virtual RGBColor AreaLightShade(const ShadeRec& sr) const;
	virtual RGBColor GetLe(const ShadeRec& sr) const;

	void SetRadianceScaleFactor(float v) { m_radiance_scale_factor = v; }
	void SetColor(const RGBColor& col) { m_color = col; }

private:
	float m_radiance_scale_factor;
	
	RGBColor m_color;

}; // Emissive

}
