#include "raytracing/bxdf/Lambertian.h"
#include "raytracing/texture/Texture.h"
#include "raytracing/sampler/Sampler.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

Lambertian::Lambertian()
	: m_kd(0)
	, m_cd_tex(nullptr)
{
}

RGBColor Lambertian::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const
{
	if (m_cd_tex)  {
		m_cd = m_cd_tex->GetColor(sr);
	}
	return m_kd * m_cd * (float)INV_PI;
}

RGBColor Lambertian::rho(const ShadeRec& sr, const Vector3D& wo) const
{
	if (m_cd_tex)  {
		m_cd = m_cd_tex->GetColor(sr);
	}
	return m_kd * m_cd;
}

RGBColor Lambertian::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const
{
    Vector3D w = sr.normal;
    Vector3D v = Vector3D(0.0034, 1, 0.0071) ^ w;
    v.Normalize();
    Vector3D u = v ^ w;

    Point3D sp = m_sampler->SampleHemisphere();
    wi = sp.x * u + sp.y * v + sp.z * w;
    wi.Normalize();

    pdf = sr.normal * wi * INV_PI;

    if (m_cd_tex) {
        m_cd = m_cd_tex->GetColor(sr);
    }
    return m_kd * m_cd * (float)INV_PI;
}

}