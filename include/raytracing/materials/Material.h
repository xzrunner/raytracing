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

	virtual RGBColor Shade(const ShadeRec& sr) const = 0;
	virtual RGBColor AreaLightShade(const ShadeRec& sr) const = 0;
	virtual RGBColor GetLe(const ShadeRec& sr) const = 0;

}; // Material

}
