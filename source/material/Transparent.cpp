#include "raytracing/material/Transparent.h"
#include "raytracing/bxdf/PerfectSpecular.h"
#include "raytracing/bxdf/PerfectTransmitter.h"
#include "raytracing/bxdf/Lambertian.h"
#include "raytracing/bxdf/GlossySpecular.h"
#include "raytracing/light/Light.h"
#include "raytracing/world/World.h"
#include "raytracing/tracer/Tracer.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

Transparent::Transparent()
{
    m_reflective_brdf = std::make_unique<PerfectSpecular>();
    m_specular_btdf   = std::make_unique<PerfectTransmitter>();
}

RGBColor Transparent::Shade(const ShadeRec& sr) const
{
	RGBColor L(Phong::Shade(sr));

	Vector3D wo = -sr.ray.dir;
	Vector3D wi;
	RGBColor fr = m_reflective_brdf->sample_f(sr, wo, wi);
	Ray reflected_ray(sr.hit_point, wi);

	if(m_specular_btdf->Tir(sr))
		L += sr.w.GetTracer()->TraceRay(reflected_ray,sr.depth+1);
	else {
		Vector3D wt;
		RGBColor ft = m_specular_btdf->sample_f(sr, wo, wt);
		Ray transmitted_ray(sr.hit_point, wt);

		L += fr*sr.w.GetTracer()->TraceRay(reflected_ray,sr.depth+1)*fabs(sr.normal*wi);

		L += ft*sr.w.GetTracer()->TraceRay(transmitted_ray,sr.depth+1)*fabs(sr.normal*wt);		//this is very important for transparent rendering
	}

	return (L);
}

RGBColor Transparent::AreaLightShade(const ShadeRec& sr) const
{
	Vector3D 	wo 			= -sr.ray.dir;
	RGBColor 	L 			= m_ambient_brdf->rho(sr, wo) * sr.w.GetAmbient()->L(sr);
	int 		num_lights	= sr.w.GetLights().size();

	for (int j = 0; j < num_lights; j++)
    {
		Vector3D 	wi 		= sr.w.GetLights()[j]->GetDirection(sr);
		float 		ndotwi 	= sr.normal * wi;

		if (ndotwi > 0.0)
        {
			bool in_shadow = false;

			if (sr.w.GetLights()[j]->CastsShadows()) {
				Ray shadowRay(sr.hit_point, wi);
				in_shadow = sr.w.GetLights()[j]->InShadow(shadowRay, sr);
			}

			if (!in_shadow)
				L += (	m_diffuse_brdf->f(sr, wo, wi)
						  + m_specular_brdf->f(sr, wo, wi)) * sr.w.GetLights()[j]->L(sr) * ndotwi * sr.w.GetLights()[j]->G(sr) / sr.w.GetLights()[j]->Pdf(sr);
		}
	}
	return (L);
}

void Transparent::SetIor(float ior)
{
    m_specular_btdf->SetIor(ior);
}

void Transparent::SetKr(float kr)
{
    m_reflective_brdf->SetKr(kr);
}

void Transparent::SetKt(float kt)
{
    m_specular_btdf->SetKt(kt);
}

}