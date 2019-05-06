#include "raytracing/lights/AreaLight.h"
#include "raytracing/materials/Material.h"
#include "raytracing/objects/GeometricObject.h"
#include "raytracing/world/World.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

AreaLight::AreaLight()
	: m_object(nullptr)
	, m_material(nullptr)
{
}

RGBColor AreaLight::L(const ShadeRec& sr) const
{
	float ndotd = -m_light_normal * m_wi;
	if (ndotd > 0.0f && m_material) {
		return m_material->GetLe(sr);
	} else {
		return BLACK;
	}
}

Vector3D AreaLight::GetDirection(const ShadeRec& sr) const
{
	m_sample_point = m_object->Sample();	// used in the G function
	m_light_normal = m_object->GetNormal(m_sample_point);
	m_wi = m_sample_point - sr.hit_point;		// used in the G function
	m_wi.Normalize();

	return m_wi;
}

bool AreaLight::InShadow(const Ray& ray, const ShadeRec& sr) const
{
	float t;
	float ts = (m_sample_point - ray.ori) * ray.dir;
	auto& objs = sr.w.GetObjects();
	for (int i=  0, n = objs.size(); i < n; ++i) {
		if (objs[i]->ShadowHit(ray, t) && t < ts) {
			return true;
		}
	}

	return false;
}

float AreaLight::G(const ShadeRec& sr) const
{
	float ndotd = -m_light_normal * m_wi;
	float d2 	= m_sample_point.DistanceSquared(sr.hit_point);

	return (ndotd / d2);
}

float AreaLight::Pdf(const ShadeRec& sr) const
{
	return (m_object->Pdf(sr));
}

void AreaLight::SetObject(const std::shared_ptr<GeometricObject>& object)
{
    m_object = object;
    m_material = object->GetMaterial();
}

}