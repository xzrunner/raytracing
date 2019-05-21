#pragma once

#include "raytracing/materials/Material.h"

#include <memory>

namespace rt
{

class Lambertian;
class GlossySpecular;

class Phong : public Material
{
public:
	Phong();
    virtual ~Phong();

    virtual RGBColor Shade(const ShadeRec& sr) const override;
    virtual RGBColor AreaLightShade(const ShadeRec& sr) const override;
    virtual RGBColor GetLe(const ShadeRec& sr) const override;

	void SetKa(float k);
	void SetKd(float k);
	void SetKs(float ks);
	void SetCd(const RGBColor& c);
	void SetCs(const RGBColor& c);
	void SetExp(float e);

protected:
    auto& GetDiffuseBRDF() const { return m_diffuse_brdf; }

protected:
	std::unique_ptr<Lambertian>		m_ambient_brdf = nullptr;
    std::unique_ptr<Lambertian>		m_diffuse_brdf = nullptr;
	std::unique_ptr<GlossySpecular>	m_specular_brdf = nullptr;

    bool m_shadows = true;

}; // Phong

}
