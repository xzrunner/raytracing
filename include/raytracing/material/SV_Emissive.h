#pragma once

#include "raytracing/material/Material.h"

#include <memory>

namespace rt
{

class Texture;

class SV_Emissive : public Material
{
public:
    SV_Emissive() {}

    virtual RGBColor GetLe(const ShadeRec& sr) const override;
    virtual void SetCe(const RGBColor& col) override { m_ce = col; }

    virtual RGBColor Shade(const ShadeRec& sr) const override;
    virtual RGBColor AreaLightShade(const ShadeRec& sr) const override;
    virtual RGBColor PathShade(ShadeRec& sr) const override;
    virtual RGBColor GlobalShade(ShadeRec& sr) const override;

    void SetRadianceScaleFactor(float v) { m_ls = v; }

    void SetTexture(const std::shared_ptr<Texture>& tex) { m_tex = tex; }

private:
    float    m_ls = 1.0f;	// radiance scaling factor
    RGBColor m_ce;		    // color

    std::shared_ptr<Texture> m_tex = nullptr;

}; // SV_Emissive

}