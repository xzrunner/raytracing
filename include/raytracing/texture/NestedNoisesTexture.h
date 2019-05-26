#pragma once

#include "raytracing/texture/Texture.h"

#include <memory>

namespace rt
{

class LatticeNoise;

class NestedNoisesTexture : public Texture
{
public:
    NestedNoisesTexture() {}
    NestedNoisesTexture(float min_val, float max_val,
        float exp, float wrap_factor, const RGBColor& color);

    virtual RGBColor GetColor(const ShadeRec& sr) const override;

    void SetNoise(const std::shared_ptr<LatticeNoise>& noise) {
        m_noise = noise;
    }

    void SetTexture(const std::shared_ptr<Texture>& texture) {
        m_texture = texture;
    }

private:
    float m_min_value = 0;
    float m_max_value = 1;

    float m_expansion_number = 0;
    float m_wrap_factor      = 0;

    std::shared_ptr<LatticeNoise> m_noise = nullptr;

    RGBColor m_color;

    std::shared_ptr<Texture> m_texture = nullptr;

}; // NestedNoisesTexture

}