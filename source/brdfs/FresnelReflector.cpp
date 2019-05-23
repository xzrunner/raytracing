#include "raytracing/brdfs/FresnelReflector.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

FresnelReflector::FresnelReflector()
    : cr(1.0f, 1.0f, 1.0f)
{
}

RGBColor FresnelReflector::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wr) const
{
    float ndotwo = static_cast<float>(sr.normal * wo);
    wr = -wo + 2.0 * sr.normal * ndotwo;
    return (Fresnel(sr) * WHITE / fabs(sr.normal * wr));
}

float FresnelReflector::Fresnel(const ShadeRec& sr) const
{
	Normal normal(sr.normal);
	float ndotd = static_cast<float>(-normal * sr.ray.dir);
	float eta;

	if (ndotd < 0.0) {
		normal = -normal;
		eta = eta_out / eta_in;
	}
	else
		eta = eta_in / eta_out;

	float cos_theta_i 		= static_cast<float>(-normal * sr.ray.dir);
	float temp 				= 1.0f - (1.0f - cos_theta_i * cos_theta_i) / (eta * eta);
	float cos_theta_t 		= sqrt (1.0f - (1.0f - cos_theta_i * cos_theta_i) / (eta * eta));
	float r_parallel 		= (eta * cos_theta_i - cos_theta_t) / (eta * cos_theta_i + cos_theta_t);
	float r_perpendicular 	= (cos_theta_i - eta * cos_theta_t) / (cos_theta_i + eta * cos_theta_t);
	float kr 				= 0.5f * (r_parallel * r_parallel + r_perpendicular * r_perpendicular);

	return (kr);
}

}