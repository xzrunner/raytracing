#pragma once

#include "raytracing/objects/Compound.h"

namespace rt
{

class FlatRimmedBowl : public Compound
{
public:
    FlatRimmedBowl();
    FlatRimmedBowl(double inner_radius, double outer_radius);

private:
    void BuildComponents();

private:
    double inner_radius = 0.9;		// radius of the inside glass surface
    double outer_radius = 1.0;

}; // FlatRimmedBowl

}