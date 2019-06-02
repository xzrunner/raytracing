#include "raytracing/material/SV_Reflector.h"
#include "raytracing/bxdf/SV_PerfectSpecular.h"
#include "raytracing/tracer/Tracer.h"
#include "raytracing/world/World.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

SV_Reflector::SV_Reflector()
    : m_reflective_brdf(std::make_unique<SV_PerfectSpecular>())
{
}

SV_Reflector::~SV_Reflector()
{
}

RGBColor SV_Reflector::Shade(const ShadeRec& sr) const
{
	RGBColor L(Phong::Shade(sr));  // direct illumination

	Vector3D wo = -sr.ray.dir;
	Vector3D wi;
	RGBColor fr = m_reflective_brdf->sample_f(sr, wo, wi);
	Ray reflected_ray(sr.hit_point, wi);
	reflected_ray.depth = sr.depth + 1;

	L += fr * sr.w.GetTracer()->TraceRay(reflected_ray, sr.depth + 1) * (sr.normal * wi);

	return (L);
}

RGBColor SV_Reflector::AreaLightShade(const ShadeRec& sr) const
{
	RGBColor L(Phong::Shade(sr));  // direct illumination

	Vector3D wo = -sr.ray.dir;
	Vector3D wi;
	RGBColor fr = m_reflective_brdf->sample_f(sr, wo, wi);
	Ray reflected_ray(sr.hit_point, wi);
	reflected_ray.depth = sr.depth + 1;

	L += fr * sr.w.GetTracer()->TraceRay(reflected_ray, sr.depth + 1) * (sr.normal * wi);
	//L += diffuse_brdf->f(sr, wo, wi) * sr.w.lights[j]->L(sr) * sr.w.lights[j]->G(sr) * ndotwi / sr.w.lights[j]->pdf(sr);

	return (L);
}

RGBColor SV_Reflector::PathShade(ShadeRec& sr) const
{
	Vector3D 	wo = -sr.ray.dir;
	Vector3D 	wi;
	float 		pdf;
	RGBColor 	fr = m_reflective_brdf->sample_f(sr, wo, wi, pdf);
	Ray 		reflected_ray(sr.hit_point, wi);

	if (sr.depth == 0)
		return (fr * sr.w.GetTracer()->TraceRay(reflected_ray, sr.depth + 2) * (sr.normal * wi) / pdf);
	else
		return (fr * sr.w.GetTracer()->TraceRay(reflected_ray, sr.depth + 1) * (sr.normal * wi) / pdf);
}

RGBColor SV_Reflector::GlobalShade(ShadeRec& sr) const
{
	Vector3D 	wo = -sr.ray.dir;
	Vector3D 	wi;
	float 		pdf;
	RGBColor 	fr = m_reflective_brdf->sample_f(sr, wo, wi, pdf);
	Ray 		reflected_ray(sr.hit_point, wi);

	return (fr * sr.w.GetTracer()->TraceRay(reflected_ray, sr.depth + 1) * (sr.normal * wi) / pdf);
}

void SV_Reflector::SetKr(float k)
{
    m_reflective_brdf->SetKr(k);
}

void SV_Reflector::SetCr(const std::shared_ptr<Texture>& texture)
{
    m_reflective_brdf->SetCr(texture);
}

}