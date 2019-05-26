#pragma once

#include "raytracing/texture/Texture.h"

#include <memory>

namespace rt
{

class LatticeNoise;

class WrappedTwoColors : public Texture
{
public:
    WrappedTwoColors() {}
    WrappedTwoColors(float min_val, float max_val, float exp);

    virtual RGBColor GetColor(const ShadeRec& sr) const override;

    void SetMinValue(float v) { m_min_value = v; }
    void SetMaxValue(float v) { m_max_value = v; }

    void SetExpansionNumber(float exp) { m_expansion_number = exp; }

    void SetNoise(const std::shared_ptr<LatticeNoise>& noise) {
        m_noise = noise;
    }

    void SetColor1(RGBColor c) { m_color1 = c; }
    void SetColor2(RGBColor c) { m_color2 = c; }

private:
    float m_min_value = 0;
    float m_max_value = 1;

    float m_expansion_number = 0;

    std::shared_ptr<LatticeNoise> m_noise = nullptr;

    RGBColor m_color1;
    RGBColor m_color2;

}; // WrappedTwoColors

}