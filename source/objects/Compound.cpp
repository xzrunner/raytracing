#include "raytracing/objects/Compound.h"
#include "raytracing/maths/Normal.h"
#include "raytracing/utilities/ShadeRec.h"

#include <float.h>

namespace rt
{

Compound::~Compound()
{
	ClearObjects();
}

bool Compound::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double t;
	Normal normal;
	Point3D local_hit_point;
	bool hit = false;
	tmin = FLT_MAX;
	int num_objects	= m_parts.size();
	for (int j = 0; j < num_objects; j++) {
		if (m_parts[j]->Hit(ray, t, sr) && (t < tmin)) {
			hit				= true;
			tmin 			= t;
			SetMaterial(m_parts[j]->GetMaterial());
			normal			= sr.normal;
			local_hit_point	= sr.local_hit_point;
		}
	}

	if (hit) {
		sr.t				= tmin;
		sr.normal 			= normal;
		sr.local_hit_point 	= local_hit_point;
	}

	return hit;
}

bool Compound::ShadowHit(const Ray& ray, float& tmin) const
{
	float t;
	Normal normal;
	Point3D	local_hit_point;
	bool hit = false;
	tmin = FLT_MAX;
	int num_objects	= m_parts.size();
	for (int j = 0; j < num_objects; j++) {
		if (m_parts[j]->ShadowHit(ray, t) && (t < tmin)) {
			hit				= true;
			tmin 			= t;
			SetMaterial(m_parts[j]->GetMaterial());
		}
	}

	return hit;
}

void Compound::SetMaterial(const std::shared_ptr<Material>& material) const
{
    GeometricObject::SetMaterial(material);
    for (int i = 0, n = m_parts.size(); i < n; ++i) {
        m_parts[i]->SetMaterial(material);
    }
}

void Compound::ClearObjects()
{
	m_parts.clear();
}

}