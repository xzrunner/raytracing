#pragma once

#include "raytracing/bxdf/BRDF.h"
#include "raytracing/utilities/Constants.h"

#include <memory>

namespace rt
{

class Texture;

class Lambertian : public BRDF
{
public:
	Lambertian();

	virtual RGBColor f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

	virtual RGBColor rho(const ShadeRec& sr, const Vector3D& wo) const;

	void SetKa(const float k);

	void SetKd(const float k);

	void SetCd(const RGBColor c);
    void SetCd(const std::shared_ptr<Texture>& tex) {
        m_cd_tex = tex;
    }

private:
	// diffuse reflection coefficient
	float m_kd;
	// diffuse color
	mutable RGBColor m_cd;
	std::shared_ptr<Texture> m_cd_tex = nullptr;

}; // Lambertian

inline void Lambertian::SetKa(const float k) {
	m_kd = k;
}

inline void Lambertian::SetKd(const float k) {
	m_kd = k;
}

inline void Lambertian::SetCd(const RGBColor c) {
	m_cd = c;
}

}
