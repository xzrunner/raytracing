#include "raytracing/bxdf/SV_PerfectSpecular.h"
#include "raytracing/texture/Texture.h"
#include "raytracing/sampler/Sampler.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

RGBColor SV_PerfectSpecular::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const
{
    float ndotwo = sr.normal * wo;
    wi = -wo + 2.0 * sr.normal * ndotwo;
    return (kr * cr->GetColor(sr) / fabs(sr.normal * wi));
}

RGBColor SV_PerfectSpecular::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const
{
    float ndotwo = sr.normal * wo;
    wi = -wo + 2.0 * sr.normal * ndotwo;
    pdf = fabs(sr.normal * wi);
    return (kr * cr->GetColor(sr));
}

}