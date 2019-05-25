#include "raytracing/brdfs/SV_Lambertian.h"
#include "raytracing/texture/Texture.h"
#include "raytracing/samplers/Sampler.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

RGBColor SV_Lambertian::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const
{
    return (kd * cd->GetColor(sr) * INV_PI);
}

RGBColor SV_Lambertian::rho(const ShadeRec& sr, const Vector3D& wo) const
{
    return (kd * cd->GetColor(sr));
}

RGBColor SV_Lambertian::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const
{
	Vector3D w = sr.normal;
	Vector3D v = Vector3D(0.0034, 1, 0.0071) ^ w;
	v.Normalize();
	Vector3D u = v ^ w;

	Point3D sp = m_sampler->SampleHemisphere();
	wi = sp.x * u + sp.y * v + sp.z * w;
	wi.Normalize();

	return (kd * cd->GetColor(sr) * INV_PI);
}

}