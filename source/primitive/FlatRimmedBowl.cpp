#include "raytracing/primitive/FlatRimmedBowl.h"
#include "raytracing/primitive/ConvexPartSphere.h"
#include "raytracing/primitive/Annulus.h"
#include "raytracing/maths/Point3D.h"

namespace rt
{

FlatRimmedBowl::FlatRimmedBowl()
{
    BuildComponents();
}

FlatRimmedBowl::FlatRimmedBowl(double inner_radius, double outer_radius)
    : inner_radius(inner_radius)
    , outer_radius(outer_radius)
{
    BuildComponents();
}

void FlatRimmedBowl::BuildComponents()
{
    m_parts.push_back(std::make_shared<ConvexPartSphere>(Point3D(),
											             outer_radius,
											             0, 360,  		// azimuth angle range - full circle
											             90,  			// minimum polar angle measured from top
											             180));         // maximum polar angle measured from top


    m_parts.push_back(std::make_shared<ConvexPartSphere>(Point3D(),
											             inner_radius,
											             0, 360, 	    // azimuth angle - full circle
											             90,            // mimimum polar angle measured from top
											             180));   		// maximum polar angle measured from top

    m_parts.push_back(std::make_shared<Annulus>(Point3D(),
                                                Normal(0, 1, 0),
                                                inner_radius,
                                                outer_radius-inner_radius));   // maximum polar angle measured from top
}

}