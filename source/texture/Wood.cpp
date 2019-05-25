#include "raytracing/texture/Wood.h"
#include "raytracing/noise/CubicNoise.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/maths/Point3D.h"
#include "raytracing/maths/Vector3D.h"
#include "raytracing/maths/maths.h"

namespace rt
{

Wood::Wood()
    : noise(std::make_shared<CubicNoise>(2, 4.0, 0.5))
    , light_color(WHITE)
    , dark_color(BLACK)
{
}

Wood::Wood(const RGBColor light, const RGBColor dark)
    : noise(std::make_shared<CubicNoise>(2, 4.0, 0.5))
    , light_color(light)
    , dark_color(dark)
{
}

RGBColor Wood::GetColor(const ShadeRec& sr) const
{
	Point3D hit_point = sr.local_hit_point;

	// perturb the hit point

	Vector3D offset = noise->vector_fbm(hit_point * ring_noise_frequency);

	Point3D ring_point = hit_point + ring_noise * offset;

	// perturb the trunk so that it's quite along the z axis

	Vector3D temp_vec = trunk_wobble * noise->vector_noise(Point3D(0, 0, hit_point.y * trunk_wobble_frequency));
	ring_point.x += temp_vec.x;
	ring_point.z += temp_vec.z;


	// distance from the y axis

	double r = sqrt(ring_point.x * ring_point.x + ring_point.z * ring_point.z) * ring_frequency;


	// perturb r so that the rings aren't quite round

	Point3D temp_vec2;
	temp_vec2.x = angular_wobble_frequency * ring_point.x;
	temp_vec2.y = angular_wobble_frequency * ring_point.y * 0.1;
	temp_vec2.z = angular_wobble_frequency * ring_point.z;

	double delta_r = angular_wobble * smooth_step(0.0, 5.0, r) * noise->value_noise(temp_vec2);
	r += delta_r;


	// add some noise so that the rings are not equally spaced and have different thicknesses

	r += ring_uneveness * noise->value_noise(Point3D(r, r, r));

	double temp = r;
	double in_ring = smooth_pulse_train(0.1, 0.55, 0.7, 0.95, 1.0, r);


	// the grain

	Point3D grain_point;
	grain_point.x = hit_point.x * grain_frequency;
	grain_point.y = hit_point.y * grain_frequency * 0.05;
	grain_point.z = hit_point.z * grain_frequency;

	double dpgrain 		= 0.2;
	double grain 		= 0.0;
	double amplitude 	= 1.0;

	for (int i = 0; i < 2; i++) {
		float grain_valid = 1.0 - smooth_step(0.2, 0.6, dpgrain);
		if (grain_valid > 0.0) {
			double g = grain_valid * noise->value_noise(grain_point);
			g *= (0.3 + 0.7 * in_ring);
			g = pow(clamp(0.8 - g, 0.0, 1.0), 2.0);
			g = grainy * smooth_step (0.5, 1.0, g);
			if (i == 0)
				in_ring *= (1.0 - 0.4 * grain_valid);
			grain = amplitude * std::max(grain, g);
		}
		grain_point = 	2.0 * grain_point;
		dpgrain 	*= 	2.0;
		amplitude 	*= 	0.5;
	}

	double final_value = mix_double(in_ring * ringy, 1.0, grain);

	return (mix_color(light_color, dark_color, final_value));
}

}