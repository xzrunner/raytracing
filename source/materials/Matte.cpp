#include "raytracing/materials/Matte.h"
#include "raytracing/maths/Vector3D.h"
#include "raytracing/brdfs/Lambertian.h"
#include "raytracing/world/World.h"
#include "raytracing/lights/Light.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

Matte::Matte()
	: m_ambient_brdf(new Lambertian)
	, m_diffuse_brdf(new Lambertian)
{
}

Matte::~Matte()
{
	delete m_ambient_brdf;
	delete m_diffuse_brdf;
}

RGBColor Matte::Shade(const ShadeRec& sr) const
{
	Vector3D wo = -sr.ray.dir;
	RGBColor L = m_ambient_brdf->rho(sr, wo) * sr.w.GetAmbient()->L(sr);
	const std::vector<Light*>& lights = sr.w.GetLights();
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

RGBColor Matte::AreaLightShade(const ShadeRec& sr) const
{
	Vector3D wo = -sr.ray.dir;
	RGBColor L = m_ambient_brdf->rho(sr, wo) * sr.w.GetAmbient()->L(sr);
	const std::vector<Light*>& lights = sr.w.GetLights();
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

void Matte::SetKa(const float k)
{
	m_ambient_brdf->SetKa(k);
}

void Matte::SetKd(const float k)
{
	m_diffuse_brdf->SetKd(k);
}

void Matte::SetCd(const RGBColor c)
{
	m_ambient_brdf->SetCd(c);
	m_diffuse_brdf->SetCd(c);
}

}