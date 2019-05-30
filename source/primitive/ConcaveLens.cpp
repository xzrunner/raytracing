#include "raytracing/primitive/ConcaveLens.h"
#include "raytracing/primitive/ConcavePartSphere.h"
#include "raytracing/primitive/ConvexOpenCylinder.h"

namespace rt
{

ConcaveLens::ConcaveLens(float radius, float thickness, float min_distance)
    : lens_radius(radius)
    , thickness(thickness)
    , min_distance(min_distance)
{
    // bounding box
    bbox.x0 = -lens_radius;
    bbox.x1 = lens_radius;
    bbox.y0 = -thickness / 2.0;
    bbox.y1 = thickness / 2.0;
    bbox.z0 = -lens_radius;
    bbox.z1 = lens_radius;

    BuildComponents();
}

bool ConcaveLens::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
    if (bbox.Hit(ray)) {
        return Compound::Hit(ray, tmin, sr);
    } else {
        return false;
    }
}

void ConcaveLens::BuildComponents()
{
	Point3D center;
	double theta_min, theta_max;

	double phi_min = 0.0;
	double phi_max = 360.0;

	// top concave part sphere

	double d = (thickness - min_distance) / 2.0;
	double sphere_radius = (d * d + lens_radius * lens_radius) / (2.0 * d);

	center = Point3D(0.0, sphere_radius + min_distance/ 2.0, 0.0);
	theta_min = 90.0 + acos(lens_radius / sphere_radius) * 180.0 / PI;  // in degrees
	theta_max = 180.0;
	m_parts.push_back(std::make_shared<ConcavePartSphere>(center, sphere_radius, phi_min, phi_max, theta_min, theta_max));

	// bottom concave part sphere

	center = Point3D(0.0, -(sphere_radius + min_distance / 2.0), 0.0);
	theta_min = 0.0;
	theta_max = 90.0 - acos(lens_radius / sphere_radius) * 180.0 / PI;  // in degrees
	m_parts.push_back(std::make_shared<ConcavePartSphere>(center, sphere_radius, phi_min, phi_max, theta_min, theta_max));

	// outer surface

	m_parts.push_back(std::make_shared<ConvexOpenCylinder>(-thickness / 2.0, thickness / 2.0, lens_radius));
}

}