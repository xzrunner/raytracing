#pragma once

#include "raytracing/objects/GeometricObject.h"

namespace rt
{

class Box : public GeometricObject
{
public:
	Box(const Point3D& min, const Point3D& max);

	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
	virtual bool ShadowHit(const Ray& ray, float& tmin) const;

	virtual AABB GetBoundingBox() const;

    void SetSize(const Point3D& min, const Point3D& max);

private:
	Normal GetNormal(const int face_hit) const;

private:
	float x0, y0, z0, x1, y1, z1;

}; // Box

}
