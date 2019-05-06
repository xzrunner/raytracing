#pragma once

#include "raytracing/utilities/RGBColor.h"

namespace rt
{

class ShadeRec;

class Texture
{
public:
	virtual ~Texture() {}

	virtual RGBColor GetColor(const ShadeRec& sr) const = 0;

}; // Texture

}
