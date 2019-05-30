#pragma once

#include "raytracing/primitive/Compound.h"
#include "raytracing/maths/AABB.h"

namespace rt
{

class ProductJar : public Compound
{
public:
    ProductJar() {}
    ProductJar(double bottom, double body_top, double cap_top, double body_radius,
        double bottom_bevel_radius, double top_bevel_radius, double cap_bevel_radius);

    virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const override;
    virtual bool ShadowHit(const Ray& ray, float& t) const override;

    virtual AABB GetBoundingBox() const override { return bbox; }

    virtual void SetMaterial(const std::shared_ptr<Material>& material) const override;

    void SetBodyMaterial(const std::shared_ptr<Material>& material) const;

private:
    AABB bbox;	// the bounding box

    double body_top    = 0;
    double body_bottom = 0;

}; // ProductJar

}