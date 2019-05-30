#pragma once

#include "raytracing/texture/Texture.h"

#include <memory>

namespace rt
{

class Image;
class LatticeNoise;

class RampFBmTexture : public Texture
{
public:
    RampFBmTexture() {}
    RampFBmTexture(const std::shared_ptr<Image>& image, int num_octaves, float fbm_amount);

    virtual RGBColor GetColor(const ShadeRec& sr) const override;

    void SetNoise(const std::shared_ptr<LatticeNoise>& noise) {
        this->noise = noise;
    }

    void SetPerturbation(float p) { perturbation = p; }
    void SetNumOctaves(float num_octaves);
    void SetLacunarity(float lacunarity);
    void SetGain(float gain);

private:
    std::shared_ptr<Image>        ramp  = nullptr;			// this stores the ramp image
    std::shared_ptr<LatticeNoise> noise = nullptr;			// supplies the value_fbm noise function
    float perturbation = 0;		// the amount of fbm ("a" in the book)
    int	  hres = 0;				// horizontal ramp image resolution

}; // RampFBmTexture

}