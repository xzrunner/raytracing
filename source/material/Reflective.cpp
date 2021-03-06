#include "raytracing/material/Reflective.h"
#include "raytracing/maths/Vector3D.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/bxdf/PerfectSpecular.h"
#include "raytracing/world/World.h"
#include "raytracing/tracer/Tracer.h"
#include "raytracing/bxdf/Lambertian.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

Reflective::Reflective()
    : m_reflective_brdf(std::make_unique<PerfectSpecular>())
{
}

RGBColor Reflective::Shade(const ShadeRec& sr) const
{
	RGBColor L(Phong::Shade(sr));  // direct illumination

	Vector3D wo = -sr.ray.dir;
	Vector3D wi;
	RGBColor fr = m_reflective_brdf->sample_f(sr, wo, wi);
	Ray reflected_ray(sr.hit_point, wi);
	reflected_ray.depth = sr.depth + 1;

	L += fr * sr.w.GetTracer()->TraceRay(reflected_ray, sr.depth + 1) * static_cast<float>(sr.normal * wi);

	return L;
}

RGBColor Reflective::AreaLightShade(const ShadeRec& sr) const
{
    RGBColor L(Phong::Shade(sr));  // direct illumination

    Vector3D wo = -sr.ray.dir;
    Vector3D wi;
    RGBColor fr = m_reflective_brdf->sample_f(sr, wo, wi);
    Ray reflected_ray(sr.hit_point, wi);
    reflected_ray.depth = sr.depth + 1;

    L += fr * sr.w.GetTracer()->TraceRay(reflected_ray, sr.depth + 1) * static_cast<float>(sr.normal * wi);

    //auto& lights = sr.w.GetLights();
    //L += GetDiffuseBRDF()->f(sr, wo, wi) * lights[j]->L(sr) * lights[j]->G(sr) * ndotwi / lights[j]->pdf(sr);

    return L;
}

RGBColor Reflective::PathShade(ShadeRec& sr) const
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

RGBColor Reflective::GlobalShade(ShadeRec& sr) const
{
	Vector3D 	wo = -sr.ray.dir;
	Vector3D 	wi;
	float 		pdf;
	RGBColor 	fr = m_reflective_brdf->sample_f(sr, wo, wi, pdf);
	Ray 		reflected_ray(sr.hit_point, wi);

	return (fr * sr.w.GetTracer()->TraceRay(reflected_ray, sr.depth + 1) * (sr.normal * wi) / pdf);
}

void Reflective::SetKr(const float k)
{
	m_reflective_brdf->SetKr(k);
}

void Reflective::SetCr(const RGBColor& c)
{
	m_reflective_brdf->SetCr(c);
}

}