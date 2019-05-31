#pragma once

#include "raytracing/material/Material.h"

namespace rt
{

class Emissive : public Material
{
public:
	Emissive() {}

    virtual RGBColor GetLe(const ShadeRec& sr) const override;
    virtual void SetCe(const RGBColor& col) override { m_color = col; }

	virtual RGBColor Shade(const ShadeRec& sr) const override;
	virtual RGBColor AreaLightShade(const ShadeRec& sr) const override;
    virtual RGBColor PathShade(ShadeRec& sr) const override;
    virtual RGBColor GlobalShade(ShadeRec& sr) const override;

	void SetRadianceScaleFactor(float v) { m_radiance_scale_factor = v; }

private:
	float m_radiance_scale_factor = 1.0f;

	RGBColor m_color;

}; // Emissive

}
