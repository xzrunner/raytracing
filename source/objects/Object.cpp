#include "raytracing/objects/Object.h"
#include "raytracing/materials/Material.h"
#include "raytracing/maths/Normal.h"

namespace rt
{

Object::Object()
	: m_material(nullptr)
{
}

Object::Object(const Object& obj)
{
	if (obj.m_material) {

	}
}

Point3D Object::Sample() const
{
	return Point3D(0, 0, 0);
}

float Object::Pdf(const ShadeRec& sr) const
{
	return 0;
}

Normal Object::GetNormal(void) const
{
	return Normal();
}

Normal Object::GetNormal(const Point3D& p) const
{
	return Normal();
}

AABB Object::GetBoundingBox() const
{
	return AABB();
}

}