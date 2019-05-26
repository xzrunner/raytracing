#pragma once

#include "raytracing/light/Light.h"

namespace rt
{

class Directional : public Light
{
public:
	Directional();

	virtual RGBColor L(const ShadeRec& sr) const override;

	virtual Vector3D GetDirection(const ShadeRec& sr) const override;

	virtual bool InShadow(const Ray& ray, const ShadeRec& sr) const override;

	void ScaleRadiance(const float b);
    void SetColor(const RGBColor& color) { m_color = color; }

	void SetDirection(const Vector3D& dir);

private:
	float    m_ls;
	RGBColor m_color;

	Vector3D m_dir;

}; // Directional

}
