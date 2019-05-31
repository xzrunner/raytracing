#include "raytracing/bxdf/BRDF.h"
#include "raytracing/sampler/Sampler.h"

namespace rt
{

RGBColor BRDF::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const
{
	return BLACK;
}

RGBColor BRDF::rho(const ShadeRec& sr, const Vector3D& wo) const
{
	return BLACK;
}

RGBColor BRDF::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const
{
	return BLACK;
}

RGBColor BRDF::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const
{
	return BLACK;
}

void BRDF::SetSampler(const std::shared_ptr<Sampler>& sampler)
{
    m_sampler = sampler;
    m_sampler->MapSamplesToHemisphere(1);
}

}