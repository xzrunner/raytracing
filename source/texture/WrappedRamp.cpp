#include "raytracing/texture/WrappedRamp.h"
#include "raytracing/texture/Image.h"
#include "raytracing/noise/LatticeNoise.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

WrappedRamp::WrappedRamp(const std::shared_ptr<Image>& image, float perturbation, float wrap_number)
    : m_image(image)
    , m_perturbation(perturbation)
    , m_wrap_number(wrap_number)
{
    m_hres = image->GetHres();
}

RGBColor WrappedRamp::GetColor(const ShadeRec& sr) const
{
	float 	noise 	= m_wrap_number * m_noise->value_fbm(sr.local_hit_point);
	float 	y 		= sr.local_hit_point.y + m_perturbation * noise;
	float 	u 		= (1.0f + sin(y)) / 2.0f;
	int 	row 	= 0;    // use top row of ramp image
	int 	column 	= (int) (u * (m_hres - 1));

	float value = noise - floor(noise);
	value = m_min_value + (m_max_value - m_min_value) * value;

	return (value * m_image->GetColor(row, column));
}

}