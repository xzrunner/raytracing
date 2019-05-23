#include "raytracing/materials/Dielectric.h"
#include "raytracing/brdfs/FresnelReflector.h"
#include "raytracing/brdfs/FresnelTransmitter.h"
#include "raytracing/brdfs/Lambertian.h"
#include "raytracing/brdfs/GlossySpecular.h"
#include "raytracing/world/World.h"
#include "raytracing/tracer/Tracer.h"
#include "raytracing/lights/Light.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

Dielectric::Dielectric()
{
    fresnel_brdf = std::make_unique<FresnelReflector>();
    fresnel_btdf = std::make_unique<FresnelTransmitter>();
}

RGBColor Dielectric::Shade(const ShadeRec& sr) const
{
	RGBColor L(Phong::Shade(sr));

	Vector3D 	wi;
	Vector3D 	wo(-sr.ray.dir);
	RGBColor 	fr = fresnel_brdf->sample_f(sr, wo, wi);  	// computes wi
	Ray 		reflected_ray(sr.hit_point, wi);
	double 		t;
	RGBColor 	Lr, Lt;
	float 		ndotwi = static_cast<float>(sr.normal * wi);

    // total internal reflection
	if (fresnel_btdf->Tir(sr))
    {
		if (ndotwi < 0.0) {
			// reflected ray is inside

			Lr = sr.w.GetTracer()->TraceRay(reflected_ray, t, sr.depth + 1);
			L += cf_in.Powc(static_cast<float>(t)) * Lr;   						// inside filter color
		}
		else {
			// reflected ray is outside

			Lr = sr.w.GetTracer()->TraceRay(reflected_ray, t, sr.depth + 1);   // kr = 1
			L += cf_out.Powc(static_cast<float>(t)) * Lr;   					// outside filter color
		}
	}
	else { 													// no total internal reflection
		Vector3D wt;
		RGBColor ft = fresnel_btdf->sample_f(sr, wo, wt);  	// computes wt
		Ray transmitted_ray(sr.hit_point, wt);
		float ndotwt = static_cast<float>(sr.normal * wt);

		if (ndotwi < 0.0) {
			// reflected ray is inside

			Lr = fr * sr.w.GetTracer()->TraceRay(reflected_ray, t, sr.depth + 1) * fabs(ndotwi);
			L += cf_in.Powc(static_cast<float>(t)) * Lr;     					// inside filter color

			// transmitted ray is outside

			Lt = ft * sr.w.GetTracer()->TraceRay(transmitted_ray, t, sr.depth + 1) * fabs(ndotwt);
			L += cf_out.Powc(static_cast<float>(t)) * Lt;   					// outside filter color
		}
		else {
			// reflected ray is outside

			Lr = fr * sr.w.GetTracer()->TraceRay(reflected_ray, t, sr.depth + 1) * fabs(ndotwi);
			L += cf_out.Powc(static_cast<float>(t)) * Lr;						// outside filter color

			// transmitted ray is inside

			Lt = ft * sr.w.GetTracer()->TraceRay(transmitted_ray, t, sr.depth + 1) * fabs(ndotwt);
			L += cf_in.Powc(static_cast<float>(t)) * Lt; 						// inside filter color
		}
	}

	return (L);
}

RGBColor Dielectric::AreaLightShade(const ShadeRec& sr) const
{
	Vector3D 	wo 			= -sr.ray.dir;
	RGBColor 	L 			= m_ambient_brdf->rho(sr, wo) * sr.w.GetAmbient()->L(sr);
	int 		num_lights	= sr.w.GetLights().size();

	for (int j = 0; j < num_lights; j++) {
		Vector3D 	wi 		= sr.w.GetLights()[j]->GetDirection(sr);
		float 		ndotwi 	= static_cast<float>(sr.normal * wi);

		if (ndotwi > 0.0) {
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

void Dielectric::SetEtaIn(float in)
{
    fresnel_brdf->SetEtaIn(in);
    fresnel_btdf->SetEtaIn(in);
}

void Dielectric::SetEtaOut(float out)
{
    fresnel_brdf->SetEtaOut(out);
    fresnel_btdf->SetEtaOut(out);
}

}