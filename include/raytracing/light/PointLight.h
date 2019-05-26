#pragma once

#include "raytracing/light/Light.h"
#include "raytracing/maths/Point3D.h"

namespace rt
{

class PointLight : public Light
{
public:
	PointLight();

	virtual RGBColor L(const ShadeRec& sr) const override;

	virtual Vector3D GetDirection(const ShadeRec& sr) const override;

	virtual bool InShadow(const Ray& ray, const ShadeRec& sr) const override;

	void SetLocation(const Point3D& location);

	void ScaleRadiance(const float b);

    void SetColor(const RGBColor& color) {
        m_color = color;
    }

private:
	// radiant scale factor
	float m_ls;
	RGBColor m_color;

	Point3D m_location;

}; // PointLight

}
