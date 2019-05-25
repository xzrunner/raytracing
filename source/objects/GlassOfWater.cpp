#include "raytracing/objects/GlassOfWater.h"
#include "raytracing/objects/Annulus.h"
#include "raytracing/objects/Disk.h"
#include "raytracing/objects/ConvexPartCylinder.h"
#include "raytracing/objects/ConcavePartCylinder.h"
#include "raytracing/objects/ConcavePartTorus.h"
#include "raytracing/objects/Instance.h"

namespace rt
{

GlassOfWater::GlassOfWater()
{
    BuildComponents();
}

GlassOfWater::GlassOfWater(float height, float inner_radius, float wall_thickness,
                           float base_thickness, float water_height, float meniscus_radius)
    : height(height)
    , inner_radius(inner_radius)
    , wall_thickness(wall_thickness)
    , base_thickness(base_thickness)
    , water_height(water_height)
    , meniscus_radius(meniscus_radius)
{
    BuildComponents();
}

void GlassOfWater::SetGlassAirMaterial(const std::shared_ptr<Material>& material)
{
    for (int j = 0; j < 4; j++)
        m_parts[j]->SetMaterial(material);
}

void GlassOfWater::SetWaterAirMaterial(const std::shared_ptr<Material>& material)
{
    m_parts[4]->SetMaterial(material);	// top of water
    m_parts[7]->SetMaterial(material);   	// meniscus
}

void GlassOfWater::SetWaterGlassMaterial(const std::shared_ptr<Material>& material)
{
    for (int j = 5; j < 7; j++)
        m_parts[j]->SetMaterial(material);
}

void GlassOfWater::BuildComponents()
{
	// build the glass parts

	m_parts.push_back(std::make_shared<Annulus>(
        Point3D(0, height, 0), 						// rim at top
        Normal(0, 1, 0),
        inner_radius,
        inner_radius + wall_thickness));

	m_parts.push_back(std::make_shared<Disk>(
        Point3D(0, 0, 0), 									// bottom of glass
        Normal(0, -1, 0),
        inner_radius + wall_thickness));

	m_parts.push_back(std::make_shared<ConcavePartCylinder>(
        water_height + meniscus_radius, // inner curved surface of glass
        height,
        inner_radius ));

	m_parts.push_back(std::make_shared<ConvexPartCylinder>(
        0, 								// outer curved surface of glass
        height,
        inner_radius + wall_thickness));


	// build the water parts

	m_parts.push_back(std::make_shared<Disk>(
        Point3D(0, water_height, 0), 					// top of water
        Normal(0, 1, 0),
        inner_radius - meniscus_radius));


	m_parts.push_back(std::make_shared<Disk>(
        Point3D(0, base_thickness, 0), 					// bottom of water
        Normal(0, -1, 0),
        inner_radius));


	m_parts.push_back(std::make_shared<ConvexPartCylinder>(
        base_thickness,					// curved surface of water
        water_height + meniscus_radius,
        inner_radius ));


	// build the meniscus: we need an instance for this

	auto meniscus_ptr = std::make_shared<Instance>(std::make_shared<ConcavePartTorus>(
        inner_radius - meniscus_radius,
        meniscus_radius,
        0, 360,			// azimuth angle - phi - range
        270, 360));		// polar angle - theta - range
	meniscus_ptr->Translate(rt::Vector3D(0, water_height + meniscus_radius, 0));
	m_parts.push_back(meniscus_ptr);
}

}