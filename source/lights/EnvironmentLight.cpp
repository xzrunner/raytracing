#include "raytracing/lights/EnvironmentLight.h"
#include "raytracing/materials/Material.h"
#include "raytracing/samplers/Sampler.h"
#include "raytracing/world/World.h"
#include "raytracing/objects/Object.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/utilities/Constants.h"

#include <assert.h>

namespace rt
{

RGBColor EnvironmentLight::L(const ShadeRec& sr) const
{
    assert(m_material);
    return m_material->GetLe(sr);
}

Vector3D EnvironmentLight::GetDirection(const ShadeRec& sr) const
{
    Vector3D u, v, w;

    w = sr.normal;
    v = Vector3D(0.0034, 1, 0.0071) ^ w;
    v.Normalize();
    u = v ^ w;
    Point3D sp = m_sampler->SampleHemisphere();
    m_wi = sp.x * u + sp.y * v + sp.z * w;

    return m_wi;
}

bool EnvironmentLight::InShadow(const Ray& ray, const ShadeRec& sr) const
{
    float t = 10000;
    int num_objects = sr.w.GetObjects().size();
    double d = EPSILON;

    for (int j = 0; j < num_objects; j++)
        if (sr.w.GetObjects()[j]->ShadowHit(ray, t) && t > d)
            return (true);

    return (false);
}

float EnvironmentLight::Pdf(const ShadeRec& sr) const
{
    return static_cast<float>(sr.normal * m_wi * INV_PI);
}

void EnvironmentLight::SetSampler(const std::shared_ptr<Sampler>& sampler)
{
    m_sampler = sampler;
    m_sampler->MapSamplesToHemisphere(1);
}

}