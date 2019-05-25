#include "raytracing/texture/RampFBmTexture.h"
#include "raytracing/noise/CubicNoise.h"
#include "raytracing/texture/Image.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

RampFBmTexture::RampFBmTexture(const std::shared_ptr<Image>& image, int num_octaves, float fbm_amount)
    : ramp(image)
    , noise(std::make_shared<CubicNoise>(num_octaves))
    , perturbation(fbm_amount)
{
    hres = ramp->GetHres();
}

RGBColor RampFBmTexture::GetColor(const ShadeRec& sr) const
{
	/*
	float 	y 		= sr.local_hit_point.y;
	float 	u 		= (1.0 + sin(y)) / 2.0;
	int 	row 	= 0;    // use top row of ramp image
	int 	column 	= (int) (u * ( hres - 1));

	return (ramp_ptr->get_color(row, column));
	*/
	// new for list
	float 	noise 	= this->noise->value_fbm(sr.local_hit_point);
	float 	y 		= sr.local_hit_point.y + perturbation * noise;
	float 	u 		= (1.0f + sin(y)) / 2.0f;
	int 	row 	= 0;    // use top row of ramp image
	int 	column 	= (int) (u * (hres - 1));

	return ramp->GetColor(column, row);
}

}