#pragma once

#include "raytracing/brdfs/BRDF.h"

#include <memory>

namespace rt
{

class Sampler;

class GlossySpecular : public BRDF
{
public:
	GlossySpecular();

	virtual RGBColor f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

	virtual RGBColor rho(const ShadeRec& sr, const Vector3D& wo) const;

	virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;

	void SetKs(float ks);
	void SetExp(float e);
	void SetCs(const RGBColor& c);

    void SetSamples(int num_samples, float exp);

private:
	// specular reflection coefficient
	float m_ks = 0;

	// specular exponent
	float m_exp = 1.0f;

	// specular color
	RGBColor m_cs;

	// for use in sample_f
	std::shared_ptr<Sampler> m_sampler = nullptr;

}; // GlossySpecular

}
