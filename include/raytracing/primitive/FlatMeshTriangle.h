#pragma once

#include "raytracing/primitive/MeshTriangle.h"

namespace rt
{

class FlatMeshTriangle : public MeshTriangle
{
public:
    FlatMeshTriangle() {}
    FlatMeshTriangle(std::shared_ptr<Mesh> mesh, int i1, int i2, int i3);

    virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const override;

}; // FlatMeshTriangle

}