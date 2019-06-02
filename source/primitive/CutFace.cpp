#include "raytracing/primitive/CutFace.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

CutFace::CutFace(double size, double radius)
    : m_size(size)
    , m_radius(radius)
{
}

bool CutFace::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double t = -ray.ori.y / ray.dir.y;

	if (t > EPSILON)
    {
		double xi = ray.ori.x + t * ray.dir.x;
		double zi = ray.ori.z + t * ray.dir.z;
		double d = xi * xi + zi * zi;
		double size_on_two = 0.5 * m_size;

		if ((-size_on_two <= xi && xi <= size_on_two) && (-size_on_two <= zi && zi <= size_on_two)  // inside square
				&& 	d >= m_radius * m_radius)															// outside circle
		{
			tmin 				= t;
			sr.normal 			= Normal(0.0, 1.0, 0.0);
			sr.local_hit_point 	= ray.ori + t * ray.dir;

			return (true);
		}
		else
			return (false);
	}

	return (false);
}

bool CutFace::ShadowHit(const Ray& ray, float& tmin) const
{
	double t = -ray.ori.y / ray.dir.y;

	if (t > EPSILON)
    {
		double xi = ray.ori.x + t * ray.dir.x;
		double zi = ray.ori.z + t * ray.dir.z;
		double d = xi * xi + zi * zi;
		double size_on_two = 0.5 * m_size;

		if ((-size_on_two <= xi && xi <= size_on_two) && (-size_on_two <= zi && zi <= size_on_two)  // inside square
				&& 	d >= m_radius * m_radius)															// outside circle
		{
			tmin 				= static_cast<float>(t);
			return (true);
		}
		else
			return (false);
	}
	return (false);
}

}