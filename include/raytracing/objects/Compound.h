#pragma once

#include "raytracing/objects/Object.h"

#include <vector>

namespace rt
{

class Compound : public Object
{
public:
	virtual ~Compound();

	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const override;
	virtual bool ShadowHit(const Ray& ray, float& tmin) const override;

    virtual void AddObject(const std::shared_ptr<Object>& obj) override {
        m_parts.push_back(obj);
    }

	virtual void SetMaterial(const std::shared_ptr<Material>& material) const override;

protected:
	void ClearObjects();

protected:
	std::vector<std::shared_ptr<Object>> m_parts;

}; // Compound

}
