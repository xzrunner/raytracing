#pragma once

#include "raytracing/primitive/FlatMeshTriangle.h"

namespace rt
{

class FlatUVMeshTriangle : public FlatMeshTriangle
{
public:
    FlatUVMeshTriangle() {}
    FlatUVMeshTriangle(std::shared_ptr<Mesh> mesh, int i1, int i2, int i3);

    virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const override;

}; // FlatUVMeshTriangle

}