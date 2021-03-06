#include "raytracing/light/Directional.h"
#include "raytracing/primitive/GeoPrimitive.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/world/World.h"

#include <vector>

namespace rt
{

Directional::Directional()
	: m_ls(1)
	, m_color(1, 1, 1)
	, m_dir(0, 1, 0)
{
}

RGBColor Directional::L(const ShadeRec& sr) const
{
	return m_ls * m_color;
}

Vector3D Directional::GetDirection(const ShadeRec& sr) const
{
	return m_dir;
}

bool Directional::InShadow(const Ray& ray, const ShadeRec& sr) const
{
	float t;
	auto& objs = sr.w.GetObjects();
	for (int i = 0, n = objs.size(); i < n; ++i) {
		if (objs[i]->ShadowHit(ray, t)) {
			return true;
		}
	}

	return false;
}

void Directional::ScaleRadiance(const float b)
{
	m_ls = b;
}

void Directional::SetDirection(const Vector3D& dir)
{
	m_dir = dir;
	m_dir.Normalize();
}

}