#include "raytracing/materials/SV_Matte.h"
#include "raytracing/brdfs/Lambertian.h"
#include "raytracing/maths/Vector3D.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/world/World.h"
#include "raytracing/lights/Light.h"

namespace rt
{

SV_Matte::SV_Matte()
	: m_ambient_brdf(std::make_unique<Lambertian>())
	, m_diffuse_brdf(std::make_unique<Lambertian>())
{
}

SV_Matte::~SV_Matte()
{
}

// same with Matte::Shade
RGBColor SV_Matte::Shade(const ShadeRec& sr) const
{
	Vector3D wo = -sr.ray.dir;
	RGBColor L = m_ambient_brdf->rho(sr, wo) * sr.w.GetAmbient()->L(sr);
	auto& lights = sr.w.GetLights();
	for (int i = 0, n = lights.size(); i < n; i++)
	{
		Vector3D wi = lights[i]->GetDirection(sr);
		float ndotwi = float(sr.normal * wi);

		if (ndotwi > 0.0f) {
			bool in_shadow = false;
			if (lights[i]->CastsShadows()) {
				Ray shadow_ray(sr.hit_point, wi);
				in_shadow = lights[i]->InShadow(shadow_ray, sr);
			}
			if (!in_shadow) {
				L += m_diffuse_brdf->f(sr, wo, wi) * lights[i]->L(sr) * ndotwi;
			}
		}
	}

	return L;
}

// same with Matte::Shade
RGBColor SV_Matte::AreaLightShade(const ShadeRec& sr) const
{
	Vector3D wo = -sr.ray.dir;
	RGBColor L = m_ambient_brdf->rho(sr, wo) * sr.w.GetAmbient()->L(sr);
	auto& lights = sr.w.GetLights();
	for (int i = 0, n = lights.size(); i < n; i++)
	{
		Vector3D wi = lights[i]->GetDirection(sr);
		float ndotwi = float(sr.normal * wi);

		if (ndotwi > 0.0) {
			bool in_shadow = false;
			if (lights[i]->CastsShadows()) {
				Ray shadow_ray(sr.hit_point, wi);
				in_shadow = lights[i]->InShadow(shadow_ray, sr);
			}
			if (!in_shadow) {
				L += m_diffuse_brdf->f(sr, wo, wi) * lights[i]->L(sr) * lights[i]->G(sr) * ndotwi / lights[i]->Pdf(sr);
			}
		}
	}

	return L;
}

void SV_Matte::SetKa(const float k)
{
	m_ambient_brdf->SetKa(k);
}

void SV_Matte::SetKd(const float k)
{
	m_diffuse_brdf->SetKd(k);
}

void SV_Matte::SetCd(const std::shared_ptr<Texture>& tex)
{
	m_ambient_brdf->SetCd(tex);
	m_diffuse_brdf->SetCd(tex);
}

}