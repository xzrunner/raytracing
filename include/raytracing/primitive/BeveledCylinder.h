#pragma once

#include "raytracing/primitive/Compound.h"
#include "raytracing/maths/AABB.h"

namespace rt
{

class BeveledCylinder : public Compound
{
public:
//    BeveledCylinder();
    BeveledCylinder(float bottom, float top, float radius, float bevel_radius);

    virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const override;
    virtual bool ShadowHit(const Ray& ray, float& t) const override;

    virtual AABB GetBoundingBox() const override { return bbox; }

    void SetBottomMaterial(const std::shared_ptr<Material>& material) {
        m_parts[0]->SetMaterial(material);
    }
    void SetTopMaterial(const std::shared_ptr<Material>& material) {
        m_parts[1]->SetMaterial(material);
    }
    void SetWallMaterial(const std::shared_ptr<Material>& material) {
        m_parts[2]->SetMaterial(material);
    }

private:
    AABB bbox;	// the bounding box

    float bottom       = 0.0f;
    float top          = 0.0f;
    float radius       = 0.0f;
    float bevel_radius = 0.0f;

}; // BeveledCylinder

}