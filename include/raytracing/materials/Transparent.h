#pragma once

#include "raytracing/materials/Phong.h"

namespace rt
{

class PerfectSpecular;
class PerfectTransmitter;

class Transparent : public Phong
{
public:
	Transparent();

	virtual RGBColor Shade(const ShadeRec& sr) const override;
	virtual RGBColor AreaLightShade(const ShadeRec& sr) const override;

    void SetIor(float ior);
	void SetKr(float kr);
    void SetKt(float kt);

private:
	std::unique_ptr<PerfectSpecular>    m_reflective_brdf = nullptr;
    std::unique_ptr<PerfectTransmitter> m_specular_btdf = nullptr;

}; // Transparent

}
