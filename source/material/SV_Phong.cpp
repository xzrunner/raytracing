#include "raytracing/material/SV_Phong.h"
#include "raytracing/bxdf/SV_Lambertian.h"
#include "raytracing/bxdf/SV_GlossySpecular.h"
#include "raytracing/world/World.h"
#include "raytracing/light/Light.h"
#include "raytracing/texture/Texture.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

SV_Phong::SV_Phong()
    : ambient_brdf(std::make_unique<SV_Lambertian>())
    , diffuse_brdf(std::make_unique<SV_Lambertian>())
    , specular_brdf(std::make_unique<SV_GlossySpecular>())
{
}

SV_Phong::~SV_Phong()
{
}

RGBColor SV_Phong::GetLe(const ShadeRec& sr) const
{
    return RGBColor(1.0f, 1.0f, 1.0f);
}

void SV_Phong::SetCe(const RGBColor& col)
{
    //ce = RGBColor(r, g, b);
    return;
}

RGBColor SV_Phong::Shade(const ShadeRec& sr) const
{
	/*Vector3D 	wo 			= -sr.ray.dir;
	RGBColor 	L 			= ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);
	int 		num_lights	= sr.w.lights.size();

	for (int j = 0; j < num_lights; j++) {
		Vector3D wi = sr.w.lights[j]->get_direction(sr);
		float ndotwi = sr.normal * wi;

		if (ndotwi > 0.0)
			L += (	diffuse_brdf->f(sr, wo, wi) +
					specular_brdf->f(sr, wo, wi)) * sr.w.lights[j]->L(sr) * ndotwi;
	}
	return (L);*/

	Vector3D 	wo 			= -sr.ray.dir;
	RGBColor 	L 			= ambient_brdf->rho(sr, wo) * sr.w.GetAmbient()->L(sr);
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
				L += (	diffuse_brdf->f(sr, wo, wi)
						  + specular_brdf->f(sr, wo, wi)) * sr.w.GetLights()[j]->L(sr) * ndotwi;
		}
	}
	return (L);
}

RGBColor SV_Phong::AreaLightShade(const ShadeRec& sr) const
{
	Vector3D 	wo 			= -sr.ray.dir;
	RGBColor 	L 			= ambient_brdf->rho(sr, wo) * sr.w.GetAmbient()->L(sr);
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
				L += (	diffuse_brdf->f(sr, wo, wi)
						  + specular_brdf->f(sr, wo, wi)) * sr.w.GetLights()[j]->L(sr) * ndotwi * sr.w.GetLights()[j]->G(sr) / sr.w.GetLights()[j]->Pdf(sr);
		}
	}
	return (L);
}

void SV_Phong::SetKa(float k)
{
    ambient_brdf->SetKd(k);
}

void SV_Phong::SetKd(float k)
{
    diffuse_brdf->SetKd(k);
}

void SV_Phong::SetCd(const std::shared_ptr<Texture>& c)
{
    ambient_brdf->SetCd(c);
    diffuse_brdf->SetCd(c);
}

void SV_Phong::SetExp(float c)
{
    specular_brdf->SetExp(c);
}

void SV_Phong::SetKs(float c)
{
    specular_brdf->SetKs(c);
}

void SV_Phong::SetCs(const std::shared_ptr<Texture>& c)
{
    specular_brdf->SetCs(c);
}

}