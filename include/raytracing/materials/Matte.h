#pragma once

#include "raytracing/materials/Material.h"

#include <memory>

namespace rt
{

class Lambertian;
class ShadeRec;
class Vector3D;

class Matte : public Material
{
public:
	Matte();

    virtual RGBColor Shade(const ShadeRec& sr) const override;
    virtual RGBColor AreaLightShade(const ShadeRec& sr) const override;
    virtual RGBColor GetLe(const ShadeRec& sr) const override;

    virtual RGBColor PathShade(ShadeRec& sr) const override;
    virtual RGBColor GlobalShade(ShadeRec& sr) const override;

	void SetKa(const float k);
	void SetKd(const float k);
	void SetCd(const RGBColor c);

private:
	std::unique_ptr<Lambertian>	m_ambient_brdf = nullptr;
	std::unique_ptr<Lambertian>	m_diffuse_brdf = nullptr;

}; // Matte

}
