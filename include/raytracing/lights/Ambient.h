#pragma once

#include "raytracing/lights/Light.h"
#include "raytracing/utilities/RGBColor.h"

namespace rt
{

class Ambient : public Light
{
public:
	Ambient();

	virtual RGBColor L(const ShadeRec& sr) const;

	virtual Vector3D GetDirection(const ShadeRec& sr) const;

	virtual bool InShadow(const Ray& ray, const ShadeRec& sr) const;

	virtual float G(const ShadeRec& sr) const;
	virtual float Pdf(const ShadeRec& sr) const;

	void ScaleRadiance(const float b);

    void SetColor(const RGBColor& color) {
        m_color = color;
    }

private:
	float m_ls;
	RGBColor m_color;

}; // Ambient

}
