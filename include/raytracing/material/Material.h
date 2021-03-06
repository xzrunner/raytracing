#pragma once

#include "raytracing/utilities/RGBColor.h"

namespace rt
{

class ShadeRec;

class Material
{
public:
	Material();
	Material(const Material& m);
	virtual ~Material() {}

    virtual RGBColor GetLe(const ShadeRec& sr) const = 0;
    virtual void SetCe(const RGBColor& col) {}

	virtual RGBColor Shade(const ShadeRec& sr) const = 0;
	virtual RGBColor AreaLightShade(const ShadeRec& sr) const = 0;
    virtual RGBColor PathShade(ShadeRec& sr) const { return RGBColor(1.0, 1.0, 1.0); }
    virtual RGBColor GlobalShade(ShadeRec& sr) const { return RGBColor(1.0, 1.0, 1.0); }

}; // Material

}
