#include "raytracing/brdfs/BTDF.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

RGBColor BTDF::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const
{
	return BLACK;
}

RGBColor BTDF::rho(const ShadeRec& sr, const Vector3D& wo) const
{
	return BLACK;
}

RGBColor BTDF::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const
{
	return BLACK;
}

}