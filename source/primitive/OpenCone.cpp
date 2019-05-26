#include "raytracing/primitive/OpenCone.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

OpenCone::OpenCone(double h, double r)
    : m_h(h)
    , m_r(r)
{
}

bool OpenCone::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double t;
	double ox = ray.ori.x;
	double oy = ray.ori.y;
	double oz = ray.ori.z;
	double dx = ray.dir.x;
	double dy = ray.dir.y;
	double dz = ray.dir.z;

	double h2r2 = m_h*m_h/(m_r*m_r);
	double a = h2r2 * dx * dx + h2r2 * dz * dz - dy * dy;
	double b = 2.0 * ( h2r2 * ox * dx - oy * dy + m_h * dy + h2r2 * oz * dz);
	double c = h2r2 * ox * ox - oy * oy + 2 * m_h * oy - m_h * m_h + h2r2 * oz * oz;
	double disc = b * b - 4.0 * a * c ;

	if (disc < 0.0)
		return(false);
	else {
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > EPSILON) {

			Point3D hitpoint = ray.ori + t * ray.dir;

			if( hitpoint.y > 0 && hitpoint.y < m_h )
			{
				tmin = t;
				sr.normal = Normal( 2 * m_h * hitpoint.x / m_r, - 2 * ( hitpoint.y - m_h), 2 * m_h * hitpoint.z / m_r);

				sr.normal.Normalize();
				// test for hitting from inside
				if (-ray.dir * sr.normal < 0.0)
					sr.normal = -sr.normal;

				sr.local_hit_point = ray.ori + tmin * ray.dir;

				return (true);
			}
		}

		t = (-b + e) / denom;    // larger root

		if (t > EPSILON) {

			Point3D hitpoint = ray.ori + t * ray.dir;

			if( hitpoint.y > 0 && hitpoint.y < m_h )
			{
				tmin = t;
				sr.normal = Normal( 2 * m_h * hitpoint.x / m_r, - 2 * ( hitpoint.y - m_h), 2 * m_h * hitpoint.z / m_r);

				sr.normal.Normalize();
				// test for hitting inside surface
				if (-ray.dir * sr.normal < 0.0)
					sr.normal = -sr.normal;

				sr.local_hit_point = ray.ori + tmin * ray.dir;

				return (true);
			}
		}
	}

	return (false);
}

bool OpenCone::ShadowHit(const Ray& ray, float& tmin) const
{
    if (!m_shadows) {
        return false;
    }

	double t;
	double ox = ray.ori.x;
	double oy = ray.ori.y;
	double oz = ray.ori.z;
	double dx = ray.dir.x;
	double dy = ray.dir.y;
	double dz = ray.dir.z;

	double h2r2 = m_h*m_h/(m_r*m_r);
	double a = h2r2 * dx * dx + h2r2 * dz * dz - dy * dy;
	double b = 2.0 * ( h2r2 * ox * dx - oy * dy + 2 * m_h * dy + h2r2 * oz * dz);
	double c = h2r2 * ox * ox - oy * oy + 2 * m_h * oy - m_h * m_h + h2r2 * oz * oz;
	double disc = b * b - 4.0 * a * c ;


	if (disc < 0.0)
		return(false);
	else {
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > EPSILON) {
			double yhit = oy + t * dy;
			if( t < tmin && yhit < m_h && yhit > 0 )
			{
				tmin = static_cast<float>(t);
				return (true);
			}
		}

		t = (-b + e) / denom;    // larger root

		if (t > EPSILON) {
			double yhit = oy + t * dy;
			if( t < tmin && yhit < m_h && yhit > 0 )
			{
				tmin = static_cast<float>(t);		// this is very important
				return (true);
			}
		}
	}
	return (false);
}

}