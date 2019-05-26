#include "raytracing/primitive/FishBowl.h"
#include "raytracing/primitive/Instance.h"
#include "raytracing/primitive/Disk.h"
#include "raytracing/primitive/ConvexPartSphere.h"
#include "raytracing/primitive/ConcavePartSphere.h"
#include "raytracing/primitive/ConvexPartTorus.h"
#include "raytracing/primitive/ConcavePartTorus.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/maths/Point3D.h"

namespace rt
{

FishBowl::FishBowl()
{
    BuildComponents();
}

FishBowl::FishBowl(float inner_radius, float glass_thickness, float water_depth,
                    float meniscus_radius, float opening_angle)
    : inner_radius(inner_radius)
    , glass_thickness(glass_thickness)
    , water_depth(water_depth)
    , meniscus_radius(meniscus_radius)
    , opening_angle(opening_angle)
{
    BuildComponents();
}

void FishBowl::SetGlassAirMaterial(const std::shared_ptr<Material>& material)
{
    for (int j = 0; j < 3; j++) {
        m_parts[j]->SetMaterial(material);
    }
}

void FishBowl::SetWaterAirMaterial(const std::shared_ptr<Material>& material)
{
    m_parts[3]->SetMaterial(material);
    m_parts[4]->SetMaterial(material);
    m_parts[5]->SetMaterial(material);
}

void FishBowl::SetWaterGlassMaterial(const std::shared_ptr<Material>& material)
{
    m_parts[6]->SetMaterial(material);
}

void FishBowl::BuildComponents()
{
	double angle_radians = (opening_angle / 2.0) * PI_ON_180; // half the opening angle in radians

	// meniscus calculations - required here because they affect the inner surface of the glass-air boundary

	// torus tube center coordinates

	double h = water_depth - inner_radius;
	double yc = h + meniscus_radius;
	double xc = sqrt(inner_radius * (inner_radius - 2.0 * meniscus_radius) - h * (h + 2.0 * meniscus_radius));
	double beta = atan2(yc, xc) * 180.0 / PI;   // in degrees


	// outer glass-air boundary

	m_parts.push_back(std::make_shared<ConvexPartSphere>(
        Point3D(0, 0, 0),
        inner_radius + glass_thickness,
        0, 360,  				// azimuth angle range - full circle
        opening_angle / 2.0,  	// minimum polar angle measured from top
        180));                 	// maximum polar angle measured from top

	// inner glass-air boundary
	// the inner surface of the glass only goes down to the top of the meniscus

	m_parts.push_back(std::make_shared<ConcavePartSphere>(
        Point3D(0, 0, 0),
        inner_radius,
        0, 360, 				// azimuth angle - full circle
        opening_angle / 2.0,    // mimimum polar angle measured from top
        90 - beta));   			// maximum polar angle measured from top

	// round rim - need an instance for this as it's a half torus

	double theta_min = opening_angle / 2.0;  	// measured counter-clockwise from (x, z) plane
	double theta_max = theta_min + 180;			// measured counter-clockwise from (x, z) plane

	auto rim_ptr = std::make_shared<Instance>(std::make_shared<ConvexPartTorus>(
        (inner_radius + glass_thickness / 2.0) * sin(angle_radians), // a
        glass_thickness / 2.0, 										 // b
        0, 360,
        theta_min,
        theta_max));

	rim_ptr->Translate(rt::Vector3D(0, (inner_radius + glass_thickness / 2.0) * cos(angle_radians), 0));
	m_parts.push_back(rim_ptr);


	// meniscus - if water_depth > 1, we need two part tori

	auto torus_ptr1 = std::make_shared<Instance>(std::make_shared<ConcavePartTorus>(
        xc,
        meniscus_radius,
        0, 360,
        270, 360));
	torus_ptr1->Translate(rt::Vector3D(0, yc, 0));
	m_parts.push_back(torus_ptr1);


	auto torus_ptr2 = std::make_shared<Instance>(std::make_shared<ConcavePartTorus>(
        xc,
        meniscus_radius,
        0, 360,
        0, beta));
	torus_ptr2->Translate(rt::Vector3D(0, yc, 0));
	m_parts.push_back(torus_ptr2);


	// water-air boundary

	m_parts.push_back(std::make_shared<Disk>(
        Point3D(0, h, 0),
        Normal(0, 1, 0),
        xc));				// the disk just touches the bottom of the meniscus


	// water-glass boundary

	m_parts.push_back(std::make_shared<ConvexPartSphere>(
        Point3D(0, 0, 0),
        inner_radius,
        0, 360,
        90 - beta,		// mimimum polar angle measured from top
        180));			// maximum polar angle measured from top
}

}