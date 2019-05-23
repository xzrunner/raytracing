#include "raytracing/brdfs/FresnelTransmitter.h"
#include "raytracing/maths/Normal.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

RGBColor FresnelTransmitter::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt) const
{
	Normal n(sr.normal);
	float cos_thetai = static_cast<float>(n * wo);

	float eta = eta_in / eta_out;//ior;

	if (cos_thetai < 0.0) {			// transmitted ray is outside
		cos_thetai = -cos_thetai;
		n = -n;  					// reverse direction of normal
		eta = 1.0f / eta; 			// invert ior
	}

	float temp = 1.0f - (1.0f - cos_thetai * cos_thetai) / (eta * eta);
	float cos_theta2 = sqrt(temp);
	wt = -wo / eta - (cos_theta2 - cos_thetai / eta) * n;

	return (Fresnel(sr) / (eta * eta) * WHITE / fabs(sr.normal * wt));
}

bool FresnelTransmitter::Tir(const ShadeRec& sr) const
{
	Vector3D wo(-sr.ray.dir);
	float cos_thetai = static_cast<float>(sr.normal * wo);
	float eta = eta_in / eta_out;

	if (cos_thetai < 0.0f)
		eta = 1.0f / eta;

	return (1.0f - (1.0f - cos_thetai * cos_thetai) / (eta * eta) < 0.0f);
}

float FresnelTransmitter::Fresnel(const ShadeRec& sr) const
{
	Normal normal(sr.normal);
	float ndotd = static_cast<float>(-normal * sr.ray.dir);
	float eta;

	if (ndotd < 0.0f) {
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

	return (1-kr);
}

}