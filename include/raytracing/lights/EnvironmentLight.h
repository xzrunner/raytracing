#pragma once

#include "raytracing/lights/Light.h"

#include <memory>

namespace rt
{

class Sampler;
class Material;

class EnvironmentLight : public Light
{
public:
    EnvironmentLight() {}

    virtual RGBColor L(const ShadeRec& sr) const override;

    virtual Vector3D GetDirection(const ShadeRec& sr) const override;

    virtual bool InShadow(const Ray& ray, const ShadeRec& sr) const override;

    virtual float Pdf(const ShadeRec& sr) const override;

    void SetSampler(const std::shared_ptr<Sampler>& sampler);
    void SetMaterial(const std::shared_ptr<Material>& material) { m_material = material; }

private:
    std::shared_ptr<Sampler>  m_sampler = nullptr;
    std::shared_ptr<Material> m_material = nullptr;

    mutable Vector3D m_wi;

}; // EnvironmentLight

}