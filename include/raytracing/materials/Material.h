#pragma once

#include "raytracing/utilities/RGBColor.h"
#include "raytracing/utilities/Object.h"

namespace rt
{

class ShadeRec;

class Material : public Object
{
public:
	Material();
	Material(const Material& m);
	virtual ~Material() {}

	virtual Material* Clone() const;

	virtual RGBColor Shade(const ShadeRec& sr) const = 0;
	virtual RGBColor AreaLightShade(const ShadeRec& sr) const;
	virtual RGBColor GetLe(const ShadeRec& sr) const;

}; // Material

}
