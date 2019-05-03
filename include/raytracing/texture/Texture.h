#pragma once

#include "raytracing/utilities/RGBColor.h"
#include "raytracing/utilities/Object.h"

namespace rt
{

class ShadeRec;

class Texture : public Object
{
public:
	virtual ~Texture() {}

	virtual RGBColor GetColor(const ShadeRec& sr) const = 0;

}; // Texture

}
