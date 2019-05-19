#pragma once

#include "raytracing/objects/Compound.h"
#include "raytracing/maths/AABB.h"

namespace rt
{

class BeveledWedge : public Compound
{
public:
	BeveledWedge(double y0,		// minimum y value
				 double y1,		// minimum y value
				 double r0,		// inner radius
				 double r1,		// outer radius
				 double rb,		// bevel radius
				 double phi0,	// minimum azimuth angle in degrees
				 double phi1);	// maximum azimuth angle

    virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const override;

    virtual AABB GetBoundingBox() const override { return bbox; }

private:
	double 	y0;		// minimum y value
	double 	y1;		// minimum y value
	double 	r0; 	// inner radius
	double 	r1;		// outer radius
	double 	rb;		// bevel radius
	double 	phi0;	// maximum azimuth angle
	double 	phi1;	// minimum azimuth angle

    AABB	bbox;	// bounding box

}; // BeveledWedge

}