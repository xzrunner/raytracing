#pragma once

#include "raytracing/primitive/GeoPrimitive.h"
#include "raytracing/maths/Normal.h"

namespace rt
{

class Triangle : public GeoPrimitive
{
public:
	Triangle();
	Triangle(const Point3D& v0, const Point3D& v1, const Point3D& v2);

	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
	virtual bool ShadowHit(const Ray& ray, float& tmin) const;

	virtual AABB GetBoundingBox() const;

    void SetNormal(const Normal& normal) { m_normal = normal; }

private:
    void ComputeNormal();

private:
	Point3D	m_v0, m_v1, m_v2;
	Normal	m_normal;

}; // Triangle

}
