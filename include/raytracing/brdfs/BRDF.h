#pragma once

#include "raytracing/utilities/RGBColor.h"
#include "raytracing/utilities/Constants.h"

#include <memory>

namespace rt
{

class ShadeRec;
class Vector3D;
class Sampler;

class BRDF
{
public:
	virtual ~BRDF() {}

	virtual RGBColor f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

	virtual RGBColor rho(const ShadeRec& sr, const Vector3D& wo) const;

	virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const;
	virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;

    void SetSampler(const std::shared_ptr<Sampler>& sampler) {
        m_sampler = sampler;
    }

protected:
    std::shared_ptr<Sampler> m_sampler = nullptr;

}; // BRDF

}
