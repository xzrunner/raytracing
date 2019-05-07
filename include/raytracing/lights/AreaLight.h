#pragma once

#include "raytracing/lights/Light.h"
#include "raytracing/maths/Point3D.h"
#include "raytracing/maths/Normal.h"
#include "raytracing/maths/Vector3D.h"

#include <memory>

namespace rt
{

class Object;
class Material;

class AreaLight : public Light
{
public:
	AreaLight();

	virtual RGBColor L(const ShadeRec& sr) const;

	virtual Vector3D GetDirection(const ShadeRec& sr) const;

	virtual bool InShadow(const Ray& ray, const ShadeRec& sr) const;

	virtual float G(const ShadeRec& sr) const;
	virtual float Pdf(const ShadeRec& sr) const;

	void SetObject(const std::shared_ptr<Object>& object);

private:
	std::shared_ptr<Object> m_object = nullptr;

	// will be an emissive material
	std::shared_ptr<Material> m_material = nullptr;

	mutable Point3D	m_sample_point;

	// assigned in get_direction - which therefore can't be const for any light
	mutable Normal m_light_normal;

	// unit direction from hit point being shaded to sample point on light surface
	mutable Vector3D m_wi;

}; // AreaLight

}
