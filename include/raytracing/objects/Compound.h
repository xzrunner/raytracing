#pragma once

#include "raytracing/objects/GeometricObject.h"

#include <vector>

namespace rt
{

class Compound : public GeometricObject
{
public:
	virtual ~Compound();
	
	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
	virtual bool ShadowHit(const Ray& ray, float& tmin) const;

	virtual void AddObject(GeometricObject* obj);

	virtual void SetMaterial(const Material* m) const;

protected:
	void ClearObjects();

protected:
	std::vector<GeometricObject*> m_parts;

}; // Compound

}
