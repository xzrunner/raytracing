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

	virtual RGBColor Shade(const ShadeRec& sr) const;

	void SetKa(const float k);
	void SetKd(const float k);
	void SetKs(const float ks);
	void SetCd(const RGBColor& c);
	void SetCs(const RGBColor& c);
	void SetExp(const float e);

private:
	std::unique_ptr<Lambertian>		m_ambient_brdf = nullptr;
    std::unique_ptr<Lambertian>		m_diffuse_brdf = nullptr;
	std::unique_ptr<GlossySpecular>	m_specular_brdf = nullptr;

}; // Phong

}
