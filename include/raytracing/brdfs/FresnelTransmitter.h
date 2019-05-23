#pragma once

#include "raytracing/brdfs/BTDF.h"

namespace rt
{

class FresnelTransmitter : public BTDF
{
public:
	FresnelTransmitter() {}

    virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt) const override;

	bool Tir(const ShadeRec& sr) const;

    float Fresnel(const ShadeRec& sr) const;

    void SetEtaIn(float in) { eta_in = in; }
    void SetEtaOut(float out) { eta_out = out; }

private:
    float eta_in  = 0.0f;
    float eta_out = 1.0f;

}; // FresnelTransmitter

}