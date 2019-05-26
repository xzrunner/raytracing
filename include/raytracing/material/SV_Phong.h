#pragma once

#include "raytracing/material/Material.h"

#include <memory>

namespace rt
{

class SV_Lambertian;
class SV_GlossySpecular;
class Texture;

class SV_Phong : public Material
{
public:
    SV_Phong();
    virtual ~SV_Phong();

    virtual RGBColor GetLe(const ShadeRec& sr) const;
    virtual void SetCe(const RGBColor& col);

	virtual RGBColor Shade(const ShadeRec& sr) const;
	virtual RGBColor AreaLightShade(const ShadeRec& sr) const;

    void SetKa(float k);
    void SetKd(float k);
    void SetCd(const std::shared_ptr<Texture>& c);
    void SetExp(float c);
    void SetKs(float c);
    void SetCs(const std::shared_ptr<Texture>& c);

private:
    std::unique_ptr<SV_Lambertian>     ambient_brdf = nullptr;
    std::unique_ptr<SV_Lambertian>     diffuse_brdf = nullptr;
    std::unique_ptr<SV_GlossySpecular> specular_brdf = nullptr;

}; // SV_Phong

}