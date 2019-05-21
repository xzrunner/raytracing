#include "raytracing/brdfs/GlossySpecular.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/samplers/MultiJittered.h"

namespace rt
{

GlossySpecular::GlossySpecular()
	: m_cs(1, 1, 1)
{
}

// ----------------------------------------------------------------------------------- f
// no sampling here: just use the Phong formula
// this is used for direct illumination only
// explained on page 284
RGBColor GlossySpecular::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const
{
	RGBColor	L;
	float		ndotwi = static_cast<float>(sr.normal * wi);
	Vector3D 	r(-wi + 2.0f * sr.normal * ndotwi); // mirror reflection direction
	float		rdotwo = static_cast<float>(r * wo);

	if (rdotwo > 0.0)
		L = m_ks * m_cs * pow(rdotwo, m_exp);

	return (L);
}

RGBColor GlossySpecular::rho(const ShadeRec& sr, const Vector3D& wo) const
{
	return BLACK;
}

RGBColor GlossySpecular::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const
{
	float ndotwo = static_cast<float>(sr.normal * wo);
	Vector3D r = -wo + 2.0f * sr.normal * ndotwo;	// direction of mirror reflection

	Vector3D w = r;
	Vector3D u = Vector3D(0.00424, 1, 0.00764) ^ w;
	u.Normalize();
	Vector3D v = u ^ w;

	Point3D sp = m_sampler->SampleHemisphere();
	wi = sp.x * u + sp.y * v + sp.z * w;			// reflected ray direction

	if (sr.normal * wi < 0.0) 						// reflected ray is below tangent plane
		wi = -sp.x * u - sp.y * v + sp.z * w;

	float phong_lobe = pow((float)(r * wi), (float)m_exp);
	pdf = static_cast<float>(phong_lobe * (sr.normal * wi));

	return (m_ks * m_cs * phong_lobe);
}

void GlossySpecular::SetKs(float ks)
{
	m_ks = ks;
}

void GlossySpecular::SetExp(float e)
{
	m_exp = e;
}

void GlossySpecular::SetCs(const RGBColor& c)
{
	m_cs = c;
}

void GlossySpecular::SetSamples(int num_samples, float exp)
{
    m_sampler = std::make_shared<MultiJittered>(num_samples);
    m_sampler->MapSamplesToHemisphere(exp);
}

}