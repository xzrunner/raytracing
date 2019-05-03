#include "raytracing/materials/Phong.h"
#include "raytracing/maths/Vector3D.h"
#include "raytracing/brdfs/Lambertian.h"
#include "raytracing/brdfs/GlossySpecular.h"
#include "raytracing/world/World.h"
#include "raytracing/lights/Light.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

Phong::Phong()
	: m_ambient_brdf(new Lambertian)
	, m_diffuse_brdf(new Lambertian)
	, m_specular_brdf(new GlossySpecular)
{
}

Phong::~Phong()
{
	m_ambient_brdf->Release();
	m_diffuse_brdf->Release();
	m_specular_brdf->Release();
}

// this sets Phong::kd
// there is no Phong::ka data member because ambient reflection
// is diffuse reflection
void Phong::SetKa(const float k)
{
	m_ambient_brdf->SetKd(k);
}

// this also sets Phong::kd, but for a different Phong object
void Phong::SetKd(const float k)
{
	m_diffuse_brdf->SetKd(k);
}

void Phong::SetKs(const float ks)
{
	m_specular_brdf->SetKs(ks);
}

void Phong::SetCd(const RGBColor& c)
{
	m_ambient_brdf->SetCd(c);
	m_diffuse_brdf->SetCd(c);
	m_specular_brdf->SetCs(c);
}

void Phong::SetCs(const RGBColor& c)
{
	m_specular_brdf->SetCs(c);
}

void Phong::SetExp(const float e)
{
	m_specular_brdf->SetExp(e);
}

RGBColor Phong::Shade(const ShadeRec& sr) const
{
	Vector3D wo = -sr.ray.dir;
	RGBColor L  = m_ambient_brdf->rho(sr, wo) * sr.w.GetAmbient()->L(sr);
	const std::vector<Light*>& lights = sr.w.GetLights();
	for (int i = 0, n = lights.size(); i < n; i++)
	{
		Vector3D wi = lights[i]->GetDirection(sr);
		float ndotwi = sr.normal * wi;
		if (ndotwi > 0.0) {

			bool in_shadow = false;
			if (lights[i]->CastsShadows()) {
				Ray shadowRay(sr.hit_point, wi);
				in_shadow = lights[i]->InShadow(shadowRay, sr);
			}

			if (!in_shadow) {

				RGBColor z2 = m_diffuse_brdf->f(sr, wo, wi);
				RGBColor z3 = m_specular_brdf->f(sr, wo, wi);

				RGBColor z0 = m_diffuse_brdf->f(sr, wo, wi) + m_specular_brdf->f(sr, wo, wi);
				RGBColor z1 = lights[i]->L(sr);

				L += (m_diffuse_brdf->f(sr, wo, wi)
					+ m_specular_brdf->f(sr, wo, wi)) * lights[i]->L(sr) * ndotwi;
			}
		}
	}

	return L;
}

}