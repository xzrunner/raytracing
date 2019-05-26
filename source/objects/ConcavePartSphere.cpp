#include "raytracing/objects/ConcavePartSphere.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/maths/Normal.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

ConcavePartSphere::ConcavePartSphere(const Point3D& c, double r, double azimuth_min,
                                   double azimuth_max, double polar_min, double polar_max)
    : m_center(c)
    , m_radius(r)
    , m_phi_min(azimuth_min * PI_ON_180)
    , m_phi_max(azimuth_max * PI_ON_180)
    , m_theta_min(polar_min * PI_ON_180)
    , m_theta_max(polar_max * PI_ON_180)
    , m_cos_theta_min(cos(m_theta_min))
    , m_cos_theta_max(cos(m_theta_max))
{
}

bool ConcavePartSphere::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double 		t;
	Vector3D  	temp 	= ray.ori - m_center;
	double 		a 		= ray.dir * ray.dir;
	double 		b 		= 2.0 * temp * ray.dir;
	double 		c 		= temp * temp - m_radius * m_radius;
	double 		disc 	= b * b - 4.0 * a * c;

	if (disc < 0.0)
		return(false);
	else {
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > EPSILON) {
			Vector3D hit = ray.ori + t * ray.dir - m_center;

			double phi = atan2(hit.x, hit.z);
			if (phi < 0.0)
				phi += TWO_PI;

			if (hit.y <= m_radius * m_cos_theta_min &&
				hit.y >= m_radius * m_cos_theta_max &&
				phi >= m_phi_min && phi <= m_phi_max) {

				tmin = t;
				sr.normal = - (temp + t * ray.dir) / m_radius;   // points inwards
				sr.local_hit_point = ray.ori + tmin * ray.dir;
				return (true);
			}
		}

		t = (-b + e) / denom;    // larger root

		if (t > EPSILON) {
			Vector3D hit = ray.ori + t * ray.dir - m_center;

			double phi = atan2(hit.x, hit.z);
			if (phi < 0.0)
				phi += TWO_PI;

			if (hit.y <= m_radius * m_cos_theta_min &&
				hit.y >= m_radius * m_cos_theta_max &&
				phi >= m_phi_min && phi <= m_phi_max) {

				tmin = t;
				sr.normal = - (temp + t * ray.dir) / m_radius;   // points inwards
				sr.local_hit_point = ray.ori + tmin * ray.dir;
				return (true);
			}
		}
	}

	return (false);
}

bool ConcavePartSphere::ShadowHit(const Ray& ray, float& tmin) const
{
    if (!m_shadows) {
        return false;
    }

    double 		t;
    Vector3D  	temp = ray.ori - m_center;
    double 		a = ray.dir * ray.dir;
    double 		b = 2.0 * temp * ray.dir;
    double 		c = temp * temp - m_radius * m_radius;
    double 		disc = b * b - 4.0 * a * c;

    if (disc < 0.0)
        return(false);
    else {
        double e = sqrt(disc);
        double denom = 2.0 * a;
        t = (-b - e) / denom;    // smaller root

        if (t > EPSILON) {
            Vector3D hit = ray.ori + t * ray.dir - m_center;

            double phi = atan2(hit.x, hit.z);
            if (phi < 0.0)
                phi += TWO_PI;

            if (hit.y <= m_radius * m_cos_theta_min &&
                hit.y >= m_radius * m_cos_theta_max &&
                phi >= m_phi_min && phi <= m_phi_max) {

                tmin = static_cast<float>(t);
                return (true);
            }
        }

        t = (-b + e) / denom;    // larger root

        if (t > EPSILON) {
            Vector3D hit = ray.ori + t * ray.dir - m_center;

            double phi = atan2(hit.x, hit.z);
            if (phi < 0.0)
                phi += TWO_PI;

            if (hit.y <= m_radius * m_cos_theta_min &&
                hit.y >= m_radius * m_cos_theta_max &&
                phi >= m_phi_min && phi <= m_phi_max) {

                tmin = static_cast<float>(t);
                return (true);
            }
        }
    }

    return (false);
}

}