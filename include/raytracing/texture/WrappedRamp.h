#pragma once

#include "raytracing/texture/Texture.h"

#include <memory>

namespace rt
{

class LatticeNoise;
class Image;

class WrappedRamp : public Texture
{
public:
    WrappedRamp() {}
    WrappedRamp(const std::shared_ptr<Image>& image,
        float perturbation, float wrap_number);

    virtual RGBColor GetColor(const ShadeRec& sr) const override;

    void SetMinValue(float v) { m_min_value = v; }
    void SetMaxValue(float v) { m_max_value = v; }

    void SetExpansionNumber(float exp) { m_expansion_number = exp; }

    void SetNoise(const std::shared_ptr<LatticeNoise>& noise) {
        m_noise = noise;
    }

    void SetColor(RGBColor c) { m_color = c; }

private:
    float m_min_value = 0;
    float m_max_value = 1;

    float m_expansion_number = 0;

    std::shared_ptr<LatticeNoise> m_noise = nullptr;

    RGBColor m_color;

    std::shared_ptr<Image> m_image = nullptr;

    float m_perturbation = 0;
    float m_wrap_number  = 0;
    int   m_hres         = 0;

}; // WrappedRamp

}