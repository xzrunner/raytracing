#pragma once

#include "raytracing/primitive/Compound.h"
#include "raytracing/maths/AABB.h"

namespace rt
{

class ThickRing : public Compound
{
public:
    ThickRing();
    ThickRing(double bottom, double top, double i_radius, double o_radius);

    virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const override;
    virtual bool ShadowHit(const Ray& ray, float& t) const override;

    virtual AABB GetBoundingBox() const override { return bbox; }

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

}; // ThickRing

}