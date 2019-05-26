#pragma once

#include "raytracing/material/Phong.h"

namespace rt
{

class FresnelReflector;
class FresnelTransmitter;

class Dielectric : public Phong
{
public:
	Dielectric();

	virtual RGBColor Shade(const ShadeRec& sr) const override;
	virtual RGBColor AreaLightShade(const ShadeRec& sr) const override;

    void SetEtaIn(float in);
    void SetEtaOut(float out);

    void SetCfIn(const RGBColor& in) { cf_in = in; }
    void SetCfOut(const RGBColor& out) { cf_out = out; }
    void SetShadows(bool b) { shadows = b; }

private:
    RGBColor cf_in;			// interior filter color
    RGBColor cf_out;		// exterior filter color

    std::unique_ptr<FresnelReflector>	fresnel_brdf = nullptr;
    std::unique_ptr<FresnelTransmitter>	fresnel_btdf = nullptr;

    bool shadows = true;

}; // Dielectric

}
