#pragma once

#include "raytracing/objects/Object.h"
#include "raytracing/maths/Point3D.h"

namespace rt
{

class Sphere : public Object
{
public:
	Sphere();
	Sphere(const Point3D& m_center, float r);

	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
	virtual bool ShadowHit(const Ray& ray, float& tmin) const;

	void SetCenter(const Point3D& m_center);
	void SetRadius(float r);

private:
	Point3D m_center;
	float m_radius;

}; // Sphere

}
