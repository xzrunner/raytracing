#pragma once

#include "raytracing/brdfs/BTDF.h"

namespace rt
{

class PerfectTransmitter : public BTDF
{
public:
	PerfectTransmitter() {}

    virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt) const override;

    void SetKt(float k) { kt = k; }
    void SetIor(float eta) { ior = eta; }

	bool Tir(const ShadeRec& sr) const;

private:
	float kt  = 0.0f;		// transmission coefficient
	float ior = 1.0f;		// index of refraction
};

}