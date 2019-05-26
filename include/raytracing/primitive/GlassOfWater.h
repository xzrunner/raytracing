#pragma once

#include "raytracing/primitive/Compound.h"

namespace rt
{

class GlassOfWater : public Compound
{
public:
    GlassOfWater();
    GlassOfWater(float height, float inner_radius, float wall_thickness,
        float base_thickness, float water_height, float meniscus_radius);

    void SetGlassAirMaterial(const std::shared_ptr<Material>& material);
    void SetWaterAirMaterial(const std::shared_ptr<Material>& material);
    void SetWaterGlassMaterial(const std::shared_ptr<Material>& material);

private:
    void BuildComponents();

private:
    float height          = 2.0f;   // total height
    float inner_radius    = 0.9f;	// inner radius of glass, outer radius of water
    float wall_thickness  = 0.1f;	// thickness of the glass wall
    float base_thickness  = 0.3f;	// thickness of the glass base
    float water_height    = 1.5f;	// height of water from bottom of glass base on (x, z) plane
    float meniscus_radius = 0.1f;

}; // GlassOfWater

}