#include "raytracing/material/GlossyReflector.h"
#include "raytracing/bxdf/GlossySpecular.h"
#include "raytracing/bxdf/Lambertian.h"
#include "raytracing/tracer/Tracer.h"
#include "raytracing/world/World.h"
#include "raytracing/light/Light.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

GlossyReflector::GlossyReflector()
	: m_glossy_specular_brdf(std::make_unique<GlossySpecular>())
{
}

GlossyReflector::~GlossyReflector()
{
}

RGBColor GlossyReflector::Shade(const ShadeRec& sr) const
{
	Vector3D wo     = -sr.ray.dir;
	RGBColor L 		= m_ambient_brdf->rho(sr, wo) * sr.w.GetAmbient()->L(sr);
    auto&    lights = sr.w.GetLights();

	for (int i = 0, n = lights.size(); i < n; i++)
    {
		Vector3D wi     = lights[i]->GetDirection(sr);
		float    ndotwi = static_cast<float>(sr.normal * wi);
		if (ndotwi > 0.0)
        {
			bool in_shadow = false;

			if (lights[i]->CastsShadows()) {
				Ray shadowRay(sr.hit_point, wi);
				in_shadow = lights[i]->InShadow(shadowRay, sr);
			}

            if (!in_shadow) {
                L += (m_diffuse_brdf->f(sr, wo, wi)
                   + m_specular_brdf->f(sr, wo, wi)) * lights[i]->L(sr) * ndotwi;
            }
		}
	}
	return (L);
}

RGBColor GlossyReflector::AreaLightShade(const ShadeRec& sr) const
{
	RGBColor 	L(Phong::AreaLightShade(sr));  // direct illumination
	Vector3D 	wo(-sr.ray.dir);
	Vector3D 	wi;
	float		pdf;
	RGBColor 	fr(m_glossy_specular_brdf->sample_f(sr, wo, wi, pdf));
	Ray 		reflected_ray(sr.hit_point, wi);

	L += fr * sr.w.GetTracer()->TraceRay(reflected_ray, sr.depth + 1) * (sr.normal * wi) / pdf;

	return (L);
}

void GlossyReflector::SetSamples(int num_samples, float exp)
{
    m_glossy_specular_brdf->SetSamples(num_samples, exp);
}

void GlossyReflector::SetSampler(const std::shared_ptr<Sampler>& sampler, float exp)
{
    m_glossy_specular_brdf->SetSampler(sampler, exp);
}

void GlossyReflector::SetKr(float k)
{
    m_glossy_specular_brdf->SetKs(k);
}

void GlossyReflector::SetCr(const RGBColor& c)
{
    m_glossy_specular_brdf->SetCs(c);
}

void GlossyReflector::SetExp(float e)
{
    Phong::SetExp(e);
    m_glossy_specular_brdf->SetExp(e);
}

}