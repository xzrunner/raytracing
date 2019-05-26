#pragma once

#include "raytracing/primitive/Compound.h"

namespace rt
{

class FishBowl : public Compound
{
public:
    FishBowl();
    FishBowl(float inner_radius, float glass_thickness, float water_depth,
        float meniscus_radius, float opening_angle);

    void SetGlassAirMaterial(const std::shared_ptr<Material>& material);
    void SetWaterAirMaterial(const std::shared_ptr<Material>& material);
    void SetWaterGlassMaterial(const std::shared_ptr<Material>& material);

private:
    void BuildComponents();

private:
    float inner_radius    = 1.0f;	// radius of the inside glass surface
    float glass_thickness = 0.1f;
    float water_depth     = 1.25f; 	// measured from the bottom of the water-glass boundary
    float meniscus_radius = 0.05f;
    float opening_angle   = 90;		// specifies how wide the opening is at the top (alpha in Figure 28.40(a))

}; // FishBowl

}