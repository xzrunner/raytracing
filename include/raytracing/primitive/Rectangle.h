#pragma once

#include "raytracing/primitive/GeoPrimitive.h"
#include "raytracing/maths/Point3D.h"
#include "raytracing/maths/Normal.h"

namespace rt
{

class Sampler;

class Rectangle : public GeoPrimitive
{
public:
    Rectangle();
    Rectangle(const Point3D& p0, const Vector3D& a, const Vector3D& b);
	Rectangle(const Point3D& p0, const Vector3D& a, const Vector3D& b, const Normal& n);

	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
	virtual bool ShadowHit(const Ray& ray, float& tmin) const;

	virtual Point3D Sample() const override;
	virtual float Pdf(const ShadeRec& sr) const override;

	virtual Normal GetNormal(const Point3D& p) const;

    void SetSampler(const std::shared_ptr<Sampler>& sampler);

private:
	// corner vertex
	Point3D m_p0;
	// side
	Vector3D m_a, m_b;
	// square of the length of side
	double m_a_len_squared, m_b_len_squared;

	Normal m_normal;

	// for rectangular lights
	float m_area;
	float m_inv_area;
	std::shared_ptr<Sampler> m_sampler = nullptr;

}; // Rectangle

}
