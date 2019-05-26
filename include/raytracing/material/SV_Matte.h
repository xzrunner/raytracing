#pragma once

#include "raytracing/material/Material.h"

#include <memory>

namespace rt
{

class Lambertian;
class ShadeRec;
class Texture;

class SV_Matte : public Material
{
public:
	SV_Matte();
	virtual ~SV_Matte();

    virtual RGBColor GetLe(const ShadeRec& sr) const override;

    virtual RGBColor Shade(const ShadeRec& sr) const override;
    virtual RGBColor AreaLightShade(const ShadeRec& sr) const override;
    virtual RGBColor PathShade(ShadeRec& sr) const override;
    virtual RGBColor GlobalShade(ShadeRec& sr) const override;

	void SetKa(const float k);
	void SetKd(const float k);
	void SetCd(const std::shared_ptr<Texture>& tex);

private:
	std::unique_ptr<Lambertian>	m_ambient_brdf;
    std::unique_ptr<Lambertian>	m_diffuse_brdf;

}; // SV_Matte

}
