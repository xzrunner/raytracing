#pragma once

#include "raytracing/utilities/RGBColor.h"

namespace rt
{

class ShadeRec;
class Vector3D;

class BTDF
{
public:
	virtual ~BTDF() {}

	virtual RGBColor f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

	virtual RGBColor rho(const ShadeRec& sr, const Vector3D& wo) const;

	virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt) const;

}; // BTDF

}
