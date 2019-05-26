#pragma once

#include "raytracing/light/Light.h"
#include "raytracing/maths/Point3D.h"

namespace rt
{

class FakeSphericalLight : public Light
{
public:
    FakeSphericalLight() {}

    virtual RGBColor L(const ShadeRec& sr) const override;

    virtual Vector3D GetDirection(const ShadeRec& sr) const override;

    virtual bool InShadow(const Ray& ray, const ShadeRec& sr) const override;

    void ScaleRadiance(float b) { m_ls = b; }

    void SetJitterAmount(float jitter_amount) { m_jitter_amount = jitter_amount; }
    void SetDistanceAttenuation(bool distance_attenuation) {
        m_distance_attenuation = distance_attenuation;
    }

private:
    float    m_ls = 1.0f;
    RGBColor m_color = RGBColor(1.0f, 1.0f, 1.0f);
    Point3D  m_location;

    float m_jitter_amount = 0;
    bool  m_distance_attenuation = false;

}; // FakeSphericalLight

}