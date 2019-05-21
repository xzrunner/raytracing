#pragma once

#include "raytracing/materials/Phong.h"

namespace rt
{

class GlossySpecular;
class Lambertian;

class GlossyReflector : public Phong
{
public:
    GlossyReflector();
    virtual ~GlossyReflector();

    virtual RGBColor Shade(const ShadeRec& sr) const;
    virtual RGBColor AreaLightShade(const ShadeRec& sr) const;

    void SetSamples(int num_samples, float exp);

    void SetKr(float k);
    void SetCr(const RGBColor& c);
    void SetExp(float e);

private:
    std::unique_ptr<GlossySpecular> m_glossy_specular_brdf = nullptr;

}; // GlossyReflector

}