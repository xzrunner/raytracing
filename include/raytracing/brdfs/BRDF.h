#pragma once

#include "raytracing/utilities/RGBColor.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

class ShadeRec;
class Vector3D;

class BRDF
{
public:
	virtual ~BRDF() {}

	virtual RGBColor f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

	virtual RGBColor rho(const ShadeRec& sr, const Vector3D& wo) const;

	virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const;
	virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;

}; // BRDF

}
