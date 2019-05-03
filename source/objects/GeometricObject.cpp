#include "raytracing/objects/GeometricObject.h"
#include "raytracing/materials/Material.h"
#include "raytracing/maths/Normal.h"

namespace rt
{

GeometricObject::GeometricObject()
	: m_material(NULL)
{
}

GeometricObject::GeometricObject(const GeometricObject& obj)
{
	if (obj.m_material) {

	}
}

GeometricObject::~GeometricObject()
{
	m_material->Release();
}

GeometricObject* GeometricObject::Clone() const
{
	return NULL;
}

Point3D GeometricObject::Sample() const
{
	return Point3D(0, 0, 0);
}

float GeometricObject::Pdf(const ShadeRec& sr) const
{
	return 0;
}

Normal GeometricObject::GetNormal(void) const
{
	return Normal();
}

Normal GeometricObject::GetNormal(const Point3D& p) const
{
	return Normal();
}

AABB GeometricObject::GetBoundingBox() const
{
	return AABB();
}

void GeometricObject::SetMaterial(const Material* m) const
{
	obj_assign((const Object*&)m_material, m);
}

}