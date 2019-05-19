#pragma once

#include "raytracing/objects/Compound.h"
#include "raytracing/maths/AABB.h"

namespace rt
{

class BeveledRing : public Compound
{
public:
    BeveledRing();
    BeveledRing(double bottom, double top, double i_radius, double o_radius, double b_radius);

    virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const override;
    virtual bool ShadowHit(const Ray& ray, float& t) const override;

    virtual AABB GetBoundingBox() const override { return bbox; }

    virtual void SetMaterial(const std::shared_ptr<Material>& material) const override;

    void SetBottomMaterial(const std::shared_ptr<Material>& material) {
        m_parts[0]->SetMaterial(material);
    }
    void SetTopMaterial(const std::shared_ptr<Material>& material) {
        m_parts[1]->SetMaterial(material);
    }
    void SetInnerWallMaterial(const std::shared_ptr<Material>& material) {
        m_parts[2]->SetMaterial(material);
    }
    void SetOuterWallMaterial(const std::shared_ptr<Material>& material) {
        m_parts[3]->SetMaterial(material);
    }

private:
    AABB bbox;	// the bounding box

}; // BeveledRing

}