#pragma once

#include "raytracing/brdfs/BRDF.h"

#include <memory>

namespace rt
{

class Texture;

class SV_Lambertian : public BRDF
{
public:
    SV_Lambertian() {}

    virtual RGBColor f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

    virtual RGBColor rho(const ShadeRec& sr, const Vector3D& wo) const;

    virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const;

    void SetKd(float k) { kd = k; }
    void SetCd(const std::shared_ptr<Texture>& c) { cd = c; }

private:
    float kd = 0.0f;

    std::shared_ptr<Texture> cd = nullptr;

}; // SV_Lambertian

}