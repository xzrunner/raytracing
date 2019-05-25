#pragma once

#include "raytracing/brdfs/BRDF.h"

#include <memory>

namespace rt
{

class Texture;

class SV_GlossySpecular : public BRDF
{
public:
    SV_GlossySpecular() {}

	virtual RGBColor f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const override;

	virtual RGBColor rho(const ShadeRec& sr, const Vector3D& wo) const override;

	virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const override;

    void SetKs(float c) { ks = c; }
    void SetCs(const std::shared_ptr<Texture>& c) { cs = c; }
    void SetExp(float c) { exp = c; }

private:
    float ks = 0;
    std::shared_ptr<Texture> cs = nullptr;
    float exp = 1; 		// specular exponent

}; // SV_GlossySpecular

}