#pragma once

#include "raytracing/texture/Texture.h"

#include <memory>

namespace rt
{

class LatticeNoise;

class Wood : public Texture
{
public:
    Wood();
    Wood(const RGBColor light, const RGBColor dark);

    virtual RGBColor GetColor(const ShadeRec& sr) const override;

private:
    std::shared_ptr<LatticeNoise> noise = nullptr;
	RGBColor light_color;
	RGBColor dark_color;
	double ring_frequency           = 4.0;
	double ring_uneveness           = 0.25;
	double ring_noise               = 0.02;
	double ring_noise_frequency     = 1.0;
	double trunk_wobble             = 0.15;
	double trunk_wobble_frequency   = 0.025;
	double angular_wobble           = 0.5;
	double angular_wobble_frequency = 1.0;
	double grain_frequency          = 25.0;
	double grainy                   = 0.5;
	double ringy                    = 0.5;

}; // Wood

}
