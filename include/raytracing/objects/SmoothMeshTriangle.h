#pragma once

#include "raytracing/objects/MeshTriangle.h"

namespace rt
{

class SmoothMeshTriangle : public MeshTriangle
{
public:
    SmoothMeshTriangle() {}
    SmoothMeshTriangle(std::shared_ptr<Mesh> mesh, int i1, int i2, int i3);

    virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const override;

private:
    Normal InterpolateNormal(double beta, double gamma) const;

}; // SmoothMeshTriangle

}