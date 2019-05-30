#include "raytracing/primitive/ConvexOpenCylinder.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/maths/Normal.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

ConvexOpenCylinder::ConvexOpenCylinder(double bottom, double top, double radius)
    : y0(bottom)
    , y1(top)
    , radius(radius)
    , inv_radius(1.0 / radius)
{
}

bool ConvexOpenCylinder::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
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
	double c = ox * ox + oz * oz - radius * radius;
	double disc = b * b - 4.0 * a * c ;


	if (disc < 0.0)
		return(false);
	else {
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > EPSILON) {
			double yhit = oy + t * dy;

			if (yhit > y0 && yhit < y1) {
				tmin = t;
				sr.normal = Normal((ox + t * dx) * inv_radius, 0.0, (oz + t * dz) * inv_radius);
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
			double yhit = oy + t * dy;

			if (yhit > y0 && yhit < y1) {
				tmin = t;
				sr.normal = Normal((ox + t * dx) * inv_radius, 0.0, (oz + t * dz) * inv_radius);
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

bool ConvexOpenCylinder::ShadowHit(const Ray& ray, float& tmin) const
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
	double c = ox * ox + oz * oz - radius * radius;
	double disc = b * b - 4.0 * a * c ;


	if (disc < 0.0)
		return(false);
	else {
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > EPSILON) {
			double yhit = oy + t * dy;

			if (yhit > y0 && yhit < y1) {
				if( t < tmin )
				{
					tmin = static_cast<float>(t);
					return (true);
				}
			}
		}

		t = (-b + e) / denom;    // larger root

		if (t > EPSILON) {
			double yhit = oy + t * dy;

			if (yhit > y0 && yhit < y1) {
				if( t < tmin )
				{
					tmin = static_cast<float>(t);		// this is very important
					return (true);
				}
			}
		}
	}
	return (false);
}

}