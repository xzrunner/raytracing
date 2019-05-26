#pragma once

#include "raytracing/texture/Texture.h"

#include <memory>

namespace rt
{

class LatticeNoise;

class TurbulenceTexture : public Texture
{
public:
    TurbulenceTexture() {}
    TurbulenceTexture(const RGBColor& color, float min_val, float max_val);

    virtual RGBColor GetColor(const ShadeRec& sr) const override;

    void SetNoise(const std::shared_ptr<LatticeNoise>& noise) {
        this->noise = noise;
    }

private:
    std::shared_ptr<LatticeNoise> noise = nullptr;
    RGBColor color;
    float    min_value = 0, max_value = 1;	// scaling factors

}; // TurbulenceTexture

}