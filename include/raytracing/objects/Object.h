#pragma once

#include "raytracing/maths/Point3D.h"
#include "raytracing/maths/AABB.h"

#include <memory>

#include <stdio.h>

namespace rt
{

class Ray;
class ShadeRec;
class Material;

class Object
{
public:
	Object();
	Object(const Object& obj);

	virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const = 0;
	virtual bool ShadowHit(const Ray& ray, float& t) const = 0;

	// The following two functions are only required for objects that are light sources, eg disks, rectangles, and spheres
	virtual Point3D Sample() const;
	virtual float Pdf(const ShadeRec& sr) const;

	// The following two functions allow us to simplify the code for smooth shaded triangle meshes
	virtual Normal GetNormal(void) const;
	virtual Normal GetNormal(const Point3D& p) const;

	virtual AABB GetBoundingBox() const;

	// required for compound objects
	virtual void AddObject(const std::shared_ptr<Object>& obj) {}

    auto& GetMaterial() const { return m_material; }
    virtual void SetMaterial(const std::shared_ptr<Material>& material) const {
        m_material = material;
    }

    void SetShadows(bool shadows) { m_shadows = shadows; }

protected:
	mutable std::shared_ptr<Material> m_material = nullptr;

    bool m_shadows = true;

}; // Object

}
