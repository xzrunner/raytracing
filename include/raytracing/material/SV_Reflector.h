#pragma once

#include "raytracing/material/Phong.h"

namespace rt
{

class SV_PerfectSpecular;
class Texture;

class SV_Reflector : public Phong
{
public:
    SV_Reflector();
    virtual ~SV_Reflector();

    virtual RGBColor Shade(const ShadeRec& sr) const override;
    virtual RGBColor AreaLightShade(const ShadeRec& sr) const override;
    virtual RGBColor PathShade(ShadeRec& sr) const override;
    virtual RGBColor GlobalShade(ShadeRec& sr) const override;

    void SetKr(float k);
    void SetCr(const std::shared_ptr<Texture>& texture);

private:
    std::unique_ptr<SV_PerfectSpecular> m_reflective_brdf = nullptr;

//    std::shared_ptr<Texture> m_texture = nullptr;

}; // SV_Reflector

}