#pragma once

#include "raytracing/material/Material.h"

#include <memory>

namespace rt
{

class Lambertian;
class ShadeRec;
class Vector3D;
class Sampler;

class Matte : public Material
{
public:
	Matte();

    virtual RGBColor GetLe(const ShadeRec& sr) const override;

    virtual RGBColor Shade(const ShadeRec& sr) const override;
    virtual RGBColor AreaLightShade(const ShadeRec& sr) const override;
    virtual RGBColor PathShade(ShadeRec& sr) const override;
    virtual RGBColor GlobalShade(ShadeRec& sr) const override;

	void SetKa(const float k);
	void SetKd(const float k);
	void SetCd(const RGBColor c);

    void SetSampler(const std::shared_ptr<Sampler>& sampler);

private:
	std::unique_ptr<Lambertian>	m_ambient_brdf = nullptr;
	std::unique_ptr<Lambertian>	m_diffuse_brdf = nullptr;

}; // Matte

}
