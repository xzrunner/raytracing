#include "raytracing/brdfs/SV_GlossySpecular.h"
#include "raytracing/samplers/Sampler.h"
#include "raytracing/texture/Texture.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

RGBColor SV_GlossySpecular::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const
{
	RGBColor 	L;
	float 		ndotwi = static_cast<float>(sr.normal * wi);
	Vector3D 	r(-wi + 2.0 * sr.normal * ndotwi); // mirror reflection direction
	float 		rdotwo = static_cast<float>(r * wo);

	if (rdotwo > 0.0)
		L = ks * cs->GetColor(sr) * pow(rdotwo, exp);

	return (L);
}

RGBColor SV_GlossySpecular::rho(const ShadeRec& sr, const Vector3D& wo) const
{
    return BLACK;
}

RGBColor SV_GlossySpecular::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const
{
    float ndotwo = static_cast<float>(sr.normal * wo);
    Vector3D r = -wo + 2.0 * sr.normal * ndotwo;     // direction of mirror reflection

    Vector3D w = r;
    Vector3D u = Vector3D(0.00424, 1, 0.00764) ^ w;
    u.Normalize();
    Vector3D v = u ^ w;

    Point3D sp = m_sampler->SampleHemisphere();
    wi = sp.x * u + sp.y * v + sp.z * w;			// reflected ray direction

    if (sr.normal * wi < 0.0) 						// reflected ray is below tangent plane
	    wi = -sp.x * u - sp.y * v + sp.z * w;

    float phong_lobe = pow((float)(r * wi), exp);	// here may cause a mistake if use pow(r*wi,exp) only
    pdf = static_cast<float>(phong_lobe * (sr.normal * wi));

    return (ks * cs->GetColor(sr) * phong_lobe);
}

}