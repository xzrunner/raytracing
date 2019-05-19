#pragma once

#include "raytracing/objects/Object.h"
#include "raytracing/maths/AABB.h"
#include "raytracing/maths/Point3D.h"
#include "raytracing/maths/Normal.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

class PartAnnulus : public Object
{
public:
    PartAnnulus();
    PartAnnulus(Point3D center, Normal normal, double inner_radius, double wall_thickness, double min_azimuth, double max_azimuth);

    virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const override;
    virtual bool ShadowHit(const Ray& ray, float& tmin) const override;

private:
    Point3D center;
    Normal normal         = Normal(0, 1, 0);
    double inner_radius   = 1;
    double wall_thickness = 1;

    double i_squared = 0;
    double w_squared = 4;

    double min_azimuth = 0;
    double max_azimuth = 2 * PI;

    AABB bbox;	// the bounding box

}; // PartAnnulus

}