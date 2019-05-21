#include "raytracing/materials/Matte.h"
#include "raytracing/maths/Vector3D.h"
#include "raytracing/brdfs/Lambertian.h"
#include "raytracing/world/World.h"
#include "raytracing/lights/Light.h"
#include "raytracing/tracer/Tracer.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

Matte::Matte()
	: m_ambient_brdf(std::make_unique<Lambertian>())
	, m_diffuse_brdf(std::make_unique<Lambertian>())
{
}

RGBColor Matte::Shade(const ShadeRec& sr) const
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

RGBColor Matte::AreaLightShade(const ShadeRec& sr) const
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

RGBColor Matte::GetLe(const ShadeRec& sr) const
{
    return RGBColor(1.0f, 1.0f, 1.0f);
}

RGBColor Matte::PathShade(ShadeRec& sr) const
{
	Vector3D 	wo = -sr.ray.dir;
	Vector3D 	wi;
	float 		pdf;
	RGBColor 	f = m_diffuse_brdf->sample_f(sr, wo, wi, pdf);
	float 		ndotwi = static_cast<float>(sr.normal * wi);
	Ray 		reflected_ray(sr.hit_point, wi);

	return (f * sr.w.GetTracer()->TraceRay(reflected_ray, sr.depth + 1) * ndotwi / pdf);
}

RGBColor Matte::GlobalShade(ShadeRec& sr) const
{
	RGBColor L;

    if (sr.depth == 0) {
        L = AreaLightShade(sr);
    }

	Vector3D 	wi;
	Vector3D 	wo 		= -sr.ray.dir;
	float 		pdf;
	RGBColor 	f 		= m_diffuse_brdf->sample_f(sr, wo, wi, pdf);
	float 		ndotwi 	= static_cast<float>(sr.normal * wi);
	Ray 		reflected_ray(sr.hit_point, wi);

	L += f * sr.w.GetTracer()->TraceRay(reflected_ray, sr.depth + 1) * ndotwi / pdf;

	return (L);
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