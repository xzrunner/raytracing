#include "raytracing/primitive/RoundRimmedBowl.h"
#include "raytracing/primitive/ConvexPartSphere.h"
#include "raytracing/primitive/ConcavePartSphere.h"
#include "raytracing/primitive/Torus.h"
#include "raytracing/primitive/GeoInstance.h"
#include "raytracing/maths/Point3D.h"

namespace rt
{

RoundRimmedBowl::RoundRimmedBowl()
{
    BuildComponents();
}

RoundRimmedBowl::RoundRimmedBowl(double inner_radius, double outer_radius, double openning_angle)
    : inner_radius(inner_radius)
    , outer_radius(outer_radius)
    , openning(openning_angle / 2)
{
    BuildComponents();
}

void RoundRimmedBowl::BuildComponents()
{
    double outer_angle = acos(cos(openning * PI_ON_180) * inner_radius / outer_radius);

	m_parts.push_back(std::make_shared<ConvexPartSphere>(Point3D(),
											             outer_radius,
											             0, 360,  				     // azimuth angle range - full circle
											             outer_angle * 180 * INV_PI, // minimum polar angle measured from top
											             180));                 	 // maximum polar angle measured from top


    m_parts.push_back(std::make_shared<ConcavePartSphere>(Point3D(),
											inner_radius,
											0, 360, 				// azimuth angle - full circle
											openning,    // mimimum polar angle measured from top
											180));   			// maximum polar angle measured from top

	double delta = (sin(outer_angle) * outer_radius - sin(openning* PI_ON_180) * inner_radius ) / 2;

    double swept_radius = sin(outer_angle) * outer_radius - delta;
    double tube_radius = delta;
	auto rim_ptr = std::make_shared<GeoInstance>(std::make_shared<Torus>(swept_radius, tube_radius));

    double dy = cos(openning*PI_ON_180) * (inner_radius + outer_radius) / 2.0 - (outer_radius - inner_radius) / 2.0;
	rim_ptr->Translate(rt::Vector3D(0, dy, 0));

    m_parts.push_back(rim_ptr);
	/*
	objects.push_back(new Torus(
								fabs(openning / 90) * (inner_radius + outer_radius) / 2.0,	// a
								fabs(openning / 90) * (outer_radius - inner_radius) / 2.0		// b
								));   			// maximum polar angle measured from top*/
}

}