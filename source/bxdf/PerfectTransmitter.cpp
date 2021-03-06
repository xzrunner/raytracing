#include "raytracing/bxdf/PerfectTransmitter.h"
#include "raytracing/maths/Normal.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

RGBColor PerfectTransmitter::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt) const
{
	Normal n(sr.normal);
	float cos_thetai = static_cast<float>(n * wo);
	float eta = ior;

	if (cos_thetai < 0.0) {			// transmitted ray is outside
		cos_thetai = -cos_thetai;
		n = -n;  					// reverse direction of normal
		eta = 1.0f / eta; 			// invert ior
	}

	float temp = 1.0f - (1.0f - cos_thetai * cos_thetai) / (eta * eta);
	float cos_theta2 = sqrt(temp);
	wt = -wo / eta - (cos_theta2 - cos_thetai / eta) * n;

	return (kt / (eta * eta) * WHITE / fabs(sr.normal * wt));
}

bool PerfectTransmitter::Tir(const ShadeRec& sr) const
{
	Vector3D wo(-sr.ray.dir);
	float cos_thetai = static_cast<float>(sr.normal * wo);
	float eta = ior;

    if (cos_thetai < 0.0) {
        eta = 1.0f / eta;
    }

	return (1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta) < 0.0);
}

}