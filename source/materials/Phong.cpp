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
	: m_ambient_brdf(std::make_unique<Lambertian>())
	, m_diffuse_brdf(std::make_unique<Lambertian>())
	, m_specular_brdf(std::make_unique<GlossySpecular>())
{
}

Phong::~Phong()
{
}

// this sets Phong::kd
// there is no Phong::ka data member because ambient reflection
// is diffuse reflection
void Phong::SetKa(float k)
{
	m_ambient_brdf->SetKd(k);
}

// this also sets Phong::kd, but for a different Phong object
void Phong::SetKd(float k)
{
	m_diffuse_brdf->SetKd(k);
}

void Phong::SetKs(float ks)
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

void Phong::SetExp(float e)
{
	m_specular_brdf->SetExp(e);
}

RGBColor Phong::Shade(const ShadeRec& sr) const
{
	Vector3D wo = -sr.ray.dir;
	RGBColor L  = m_ambient_brdf->rho(sr, wo) * sr.w.GetAmbient()->L(sr);
	auto& lights = sr.w.GetLights();
	for (int i = 0, n = lights.size(); i < n; i++)
	{
		Vector3D wi = lights[i]->GetDirection(sr);
		float ndotwi = static_cast<float>(sr.normal * wi);
		if (ndotwi > 0.0)
        {
            if (m_shadows)
            {
			    bool in_shadow = false;
			    if (lights[i]->CastsShadows()) {
				    Ray shadowRay(sr.hit_point, wi);
				    in_shadow = lights[i]->InShadow(shadowRay, sr);
			    }
			    if (!in_shadow)
                {
				    L += (m_diffuse_brdf->f(sr, wo, wi)
					    + m_specular_brdf->f(sr, wo, wi)) * lights[i]->L(sr) * ndotwi;
			    }
            }
            else
            {
                L += (m_diffuse_brdf->f(sr, wo, wi)
                    + m_specular_brdf->f(sr, wo, wi)) * lights[i]->L(sr) * ndotwi;
            }
		}
	}

	return L;
}

RGBColor Phong::AreaLightShade(const ShadeRec& sr) const
{
	Vector3D wo = -sr.ray.dir;
	RGBColor L  = m_ambient_brdf->rho(sr, wo) * sr.w.GetAmbient()->L(sr);
	auto& lights = sr.w.GetLights();
	for (int i = 0, n = lights.size(); i < n; i++)
    {
        Vector3D wi = lights[i]->GetDirection(sr);
        float ndotwi = static_cast<float>(sr.normal * wi);
		if (ndotwi > 0.0)
        {
			if (m_shadows)
			{
				bool in_shadow = false;
                if (lights[i]->CastsShadows()) {
                    Ray shadowRay(sr.hit_point, wi);
                    in_shadow = lights[i]->InShadow(shadowRay, sr);
                }
                if (!in_shadow) {
                    L += (m_diffuse_brdf->f(sr, wo, wi)
                        + m_specular_brdf->f(sr, wo, wi)) * lights[i]->L(sr) * ndotwi * lights[i]->G(sr) / lights[i]->Pdf(sr);
                }
			}
			else
			{
                L += (m_diffuse_brdf->f(sr, wo, wi)
                    + m_specular_brdf->f(sr, wo, wi)) * lights[i]->L(sr) * ndotwi * lights[i]->G(sr) / lights[i]->Pdf(sr);
			}
		}
	}
	return (L);
}

RGBColor Phong::GetLe(const ShadeRec& sr) const
{
    return RGBColor(1.0f, 1.0f, 1.0f);
}

}