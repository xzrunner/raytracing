#pragma once

#include "raytracing/primitive/GeoPrimitive.h"
#include "raytracing/maths/Point3D.h"

namespace rt
{

class Sampler;

class Sphere : public GeoPrimitive
{
public:
	Sphere();
	Sphere(const Point3D& center, float r);

	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
	virtual bool ShadowHit(const Ray& ray, float& tmin) const;

    virtual Point3D Sample() const override;
    virtual float Pdf(const ShadeRec& sr) const override;

	void SetCenter(const Point3D& center);
	void SetRadius(float r);

    void SetSampler(const std::shared_ptr<Sampler>& sampler);

private:
	Point3D m_center;
	float m_radius;

    std::shared_ptr<Sampler> m_sampler = nullptr;
    double m_inv_area = 0;

}; // Sphere

}
