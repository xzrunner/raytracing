#pragma once

#include "raytracing/texture/Texture.h"

#include <memory>

namespace rt
{

class LatticeNoise;

class FBmTexture : public Texture
{
public:
    FBmTexture() {}
    FBmTexture(const std::shared_ptr<LatticeNoise>& noise) : noise(noise) {}

    virtual RGBColor GetColor(const ShadeRec& sr) const override;

    void SetColor(const RGBColor& c) { color = c; }

    void SetMinValue(float v) { min_value = v; }
    void SetMaxValue(float v) { max_value = v; }

private:
    std::shared_ptr<LatticeNoise> noise = nullptr;
    RGBColor color;
    float    min_value = 0, max_value = 0;	// scaling factors

}; // FBmTexture

}