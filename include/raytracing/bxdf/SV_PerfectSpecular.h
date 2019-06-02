#pragma once

#include "raytracing/bxdf/BRDF.h"

#include <memory>

namespace rt
{

class Texture;

class SV_PerfectSpecular : public BRDF
{
public:
    SV_PerfectSpecular() {}

    virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const override;
    virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const override;

    void SetKr(float k) { kr = k; }
    void SetCr(const std::shared_ptr<Texture>& c) { cr = c; }

private:
    float kr = 0.0f;

    std::shared_ptr<Texture> cr = nullptr;

}; // SV_PerfectSpecular

}