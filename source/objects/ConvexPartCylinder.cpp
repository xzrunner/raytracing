#include "raytracing/objects/ConvexPartCylinder.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/maths/Normal.h"
#include "raytracing/utilities/ShadeRec.h"

namespace
{

double kEpsilon = 0.001;

}

namespace rt
{

ConvexPartCylinder::ConvexPartCylinder(double bottom, double top, double radius,
                                       double polar_min, double polar_max)
    : m_y0(bottom)
    , m_y1(top)
    , m_radius(radius)
    , m_inv_radius(1.0 / radius)
    , m_polar_min(polar_min * PI_ON_180)
    , m_polar_max(polar_max * PI_ON_180)
{
}

ConvexPartCylinder::ConvexPartCylinder(double bottom, double top, double radius)
    : m_y0(bottom)
    , m_y1(top)
    , m_radius(radius)
    , m_inv_radius(1.0 / radius)
    , m_polar_min(0)
    , m_polar_max(TWO_PI)
{
}

bool ConvexPartCylinder::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double t;
	double ox = ray.ori.x;
	double oy = ray.ori.y;
	double oz = ray.ori.z;
	double dx = ray.dir.x;
	double dy = ray.dir.y;
	double dz = ray.dir.z;

	double a = dx * dx + dz * dz;
	double b = 2.0 * (ox * dx + oz * dz);
	double c = ox * ox + oz * oz - m_radius * m_radius;
	double disc = b * b - 4.0 * a * c ;


	if (disc < 0.0)
		return(false);
	else {
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > kEpsilon) {
			double yhit = oy + t * dy;

			if (yhit > m_y0 && yhit < m_y1) {

				Point3D hit = ray.ori + t * ray.dir;

				double phi = atan2(hit.x, hit.z);

				if (phi < 0.0)
					phi += TWO_PI;

				if(  phi >= m_polar_min && phi <= m_polar_max  )
				{
					tmin = t;
					sr.normal = Normal((ox + t * dx) * m_inv_radius, 0.0, (oz + t * dz) * m_inv_radius);
					// test for hitting from inside
					if (-ray.dir * sr.normal < 0.0)
						sr.normal = -sr.normal;
					sr.local_hit_point = ray.ori + tmin * ray.dir;
					return true;
				}
			}
		}

		t = (-b + e) / denom;    // larger root

		if (t > kEpsilon) {
			double yhit = oy + t * dy;

			if (yhit > m_y0 && yhit < m_y1) {

				Point3D hit = ray.ori + t * ray.dir;

				double phi = atan2(hit.x, hit.z);

				if (phi < 0.0)
					phi += TWO_PI;

				if(  phi >= m_polar_min && phi <= m_polar_max  )
				{
					tmin = t;
					sr.normal = Normal((ox + t * dx) * m_inv_radius, 0.0, (oz + t * dz) * m_inv_radius);
					// test for hitting from inside
					if (-ray.dir * sr.normal < 0.0)
						sr.normal = -sr.normal;
					sr.local_hit_point = ray.ori + tmin * ray.dir;
					return true;
				}
			}
		}
	}
	return false;
}

bool ConvexPartCylinder::ShadowHit(const Ray& ray, float& tmin) const
{
	double t;
	double ox = ray.ori.x;
	double oy = ray.ori.y;
	double oz = ray.ori.z;
	double dx = ray.dir.x;
	double dy = ray.dir.y;
	double dz = ray.dir.z;

	double a = dx * dx + dz * dz;
	double b = 2.0 * (ox * dx + oz * dz);
	double c = ox * ox + oz * oz - m_radius * m_radius;
	double disc = b * b - 4.0 * a * c ;


	if (disc < 0.0)
		return(false);
	else {
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > kEpsilon) {
			double yhit = oy + t * dy;

			if (yhit > m_y0 && yhit < m_y1) {

				Point3D hit = ray.ori + t * ray.dir;

				double phi = atan2(hit.x, hit.z);

				if (phi < 0.0)
					phi += TWO_PI;

				if(  phi >= m_polar_min && phi <= m_polar_max  )
				{
					if( t < tmin )
					{
						tmin = static_cast<float>(t);
						return (true);
					}
					Vector3D normal = - Normal((ox + t * dx) * m_inv_radius, 0.0, (oz + t * dz) * m_inv_radius);
					if( ray.dir * normal < 0.0 )
					{
						tmin = 10;
						return true;
					}
				}
			}
		}

		t = (-b + e) / denom;    // larger root

		if (t > kEpsilon) {
			double yhit = oy + t * dy;

			if (yhit > m_y0 && yhit < m_y1) {

				Point3D hit = ray.ori + t * ray.dir;

				double phi = atan2(hit.x, hit.z);

				if (phi < 0.0)
					phi += TWO_PI;

				if(  phi >= m_polar_min && phi <= m_polar_max  )
				{
					if( t < tmin )
					{
						tmin = static_cast<float>(t);
						return true;
					}
					Vector3D normal = - Normal((ox + t * dx) * m_inv_radius, 0.0, (oz + t * dz) * m_inv_radius);
					if( ray.dir * normal < 0.0 )
					{
						tmin = 10;
						return true;
					}
				}
			}
		}
	}
	return false;
}

}