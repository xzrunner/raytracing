#include "raytracing/lights/AmbientOccluder.h"
#include "raytracing/samplers/Sampler.h"
#include "raytracing/world/World.h"
#include "raytracing/objects/Object.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

RGBColor AmbientOccluder::L(const ShadeRec& sr) const
{
    m_w = sr.normal;
    m_v = m_w ^ Vector3D(0.0072, 1.0, 0.0034); // jitter the up vector in case normal is vertical
    m_v.Normalize();			//there may be a spelling error used normalise
    m_u = m_v ^ m_w;

	Ray shadow_ray;
	shadow_ray.ori = sr.hit_point;
	shadow_ray.dir = GetDirection(sr);

    if (InShadow(shadow_ray, sr)) {
        return m_min_amount * m_ls * m_color;
    } else {
        return m_ls * m_color;
    }
}

Vector3D AmbientOccluder::GetDirection(const ShadeRec& s) const
{
    Point3D sp = m_sampler->SampleHemisphere();
    return (sp.x * m_u + sp.y * m_v + sp.z * m_w);
}

bool AmbientOccluder::InShadow(const Ray& ray, const ShadeRec& sr) const
{
	float t           = 10000;
    auto& objs        = sr.w.GetObjects();
	int   num_objects = objs.size();

    for (int j = 0; j < num_objects; j++) {
        if (objs[j]->ShadowHit(ray, t)) {
            return true;
        }
    }

	return false;
}

void AmbientOccluder::SetSampler(const std::shared_ptr<Sampler>& sampler)
{
    m_sampler = sampler;
    m_sampler->MapSamplesToHemisphere(1);
}

}