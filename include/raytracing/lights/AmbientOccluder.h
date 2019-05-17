#pragma once

#include "raytracing/lights/Light.h"

#include <memory>

namespace rt
{

class Sampler;

class AmbientOccluder : public Light
{
public:
    AmbientOccluder() {}

    virtual RGBColor L(const ShadeRec& sr) const override;

    virtual Vector3D GetDirection(const ShadeRec& sr) const override;

    virtual bool InShadow(const Ray& ray, const ShadeRec& sr) const override;

    void ScaleRadiance(float b) { m_ls = b; }

    void SetColor(const float c) {
        m_color.r = c;
        m_color.g = c;
        m_color.b = c;
    }
    void SetColor(const RGBColor& c) { m_color = c; }
    void SetColor(float r, float g, float b) {
        m_color.r = r;
        m_color.g = g;
        m_color.b = b;
    }

    void SetSampler(const std::shared_ptr<Sampler>& sampler);

    void SetMinAmount(float m) { m_min_amount = m; }

private:
    float    m_ls = 1.0f;
    RGBColor m_color = RGBColor(1.0f, 1.0f, 1.0f);
    float    m_min_amount = 0;
    mutable Vector3D m_u, m_v, m_w;
    std::shared_ptr<Sampler> m_sampler = nullptr;

}; // AmbientOccluder

}