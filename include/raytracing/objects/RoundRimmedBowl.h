#pragma once

#include "raytracing/objects/Compound.h"

namespace rt
{

class RoundRimmedBowl : public Compound
{
public:
    RoundRimmedBowl();
    RoundRimmedBowl(double inner_radius, double outer_radius, double openning_angle);

private:
    void BuildComponents();

private:
    double inner_radius = 0.9;		// radius of the inside glass surface
    double outer_radius = 1.0;
    double openning     = 90;

}; // RoundRimmedBowl

}