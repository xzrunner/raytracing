#pragma once

#include "raytracing/materials/Phong.h"

namespace rt
{

class PerfectSpecular;

class Reflective : public Phong
{
public:
	Reflective();

	virtual RGBColor Shade(const ShadeRec& sr) const;
	virtual RGBColor AreaLightShade(const ShadeRec& sr) const;

    virtual RGBColor PathShade(ShadeRec& sr) const override;
    virtual RGBColor GlobalShade(ShadeRec& sr) const override;

	void SetKr(const float k);
	void SetCr(const RGBColor& c);

private:
	std::unique_ptr<PerfectSpecular> m_reflective_brdf = nullptr;

}; // Reflective

}
