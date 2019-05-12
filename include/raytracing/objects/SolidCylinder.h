#pragma once

#include "raytracing/objects/Compound.h"
#include "raytracing/maths/AABB.h"

namespace rt
{

class SolidCylinder : public Compound
{
public:
    SolidCylinder();
    SolidCylinder(float bottom, float top, float radius);

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
    void SetWallMaterial(const std::shared_ptr<Material>& material) {
        m_parts[2]->SetMaterial(material);
    }

private:
    AABB bbox;	// the bounding box

}; // SolidCylinder

}