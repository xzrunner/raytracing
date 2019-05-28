#include "raytracing/bxdf/Lambertian.h"
#include "raytracing/texture/Texture.h"

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

}