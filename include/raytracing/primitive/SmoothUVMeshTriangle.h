#pragma once

#include "raytracing/primitive/SmoothMeshTriangle.h"

namespace rt
{

class SmoothUVMeshTriangle : public SmoothMeshTriangle
{
public:
    SmoothUVMeshTriangle() {}
    SmoothUVMeshTriangle(std::shared_ptr<Mesh> mesh, int i1, int i2, int i3);

    virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const override;

}; // SmoothUVMeshTriangle

}