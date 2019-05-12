#pragma once

#include "raytracing/objects/Object.h"
#include "raytracing/maths/Normal.h"

namespace rt
{

class Mesh;

class MeshTriangle : public Object
{
public:
    MeshTriangle() {}
    MeshTriangle(std::shared_ptr<Mesh> mesh, int i1, int i2, int i3);

    virtual bool ShadowHit(const Ray& ray, float& tmin) const override;

    virtual Normal GetNormal(void) const override { return normal; }
    virtual AABB GetBoundingBox() const override;

    void ComputeNormal(bool reverse_normal);

protected:
    std::shared_ptr<Mesh> mesh = nullptr;					// stores all the data
    int			index0 = 0, index1 = 0, index2 = 0;  	// indices into the vertices array in the mesh
    Normal		normal;
//    float		area;						// required for translucency

}; // MeshTriangle

}