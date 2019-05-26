#pragma once

#include "raytracing/bxdf/BRDF.h"

namespace rt
{

class FresnelReflector : public BRDF
{
public:
    FresnelReflector();

	virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wr) const override;

    float Fresnel(const ShadeRec& sr) const;

    void SetEtaIn(float in) { eta_in = in; }
    void SetEtaOut(float out) { eta_out = out; }

private:
    float eta_in  = 0.0f;
    float eta_out = 0.0f;

    RGBColor cr;			// the reflection colour

}; // FresnelReflector

}