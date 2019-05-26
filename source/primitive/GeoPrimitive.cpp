#include "raytracing/primitive/GeoPrimitive.h"
#include "raytracing/material/Material.h"
#include "raytracing/maths/Normal.h"

namespace rt
{

GeoPrimitive::GeoPrimitive()
	: m_material(nullptr)
{
}

GeoPrimitive::GeoPrimitive(const GeoPrimitive& obj)
{
	if (obj.m_material) {

	}
}

Point3D GeoPrimitive::Sample() const
{
	return Point3D(0, 0, 0);
}

float GeoPrimitive::Pdf(const ShadeRec& sr) const
{
	return 0;
}

Normal GeoPrimitive::GetNormal(void) const
{
	return Normal();
}

Normal GeoPrimitive::GetNormal(const Point3D& p) const
{
	return Normal();
}

AABB GeoPrimitive::GetBoundingBox() const
{
	return AABB();
}

}