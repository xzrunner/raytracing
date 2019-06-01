#pragma once

#include "raytracing/texture/Texture.h"
#include "raytracing/maths/Point3D.h"

namespace rt
{

class DiskChecker : public Texture
{
public:
    DiskChecker() {}

    virtual RGBColor GetColor(const ShadeRec& sr) const override;

    void SetNumAngularCheckers(int num_angular) { m_num_angular_checkers = num_angular; }
    void SetNumRadialCheckers(int num_radial)   { m_num_radial_checkers  = num_radial; }
    void SetAngularLineWidth(float width)       { m_angular_line_width   = width; }
    void SetRadialLineWidth(float width)        { m_radial_line_width    = width; }

    void SetRadius(float r)          { m_radius = r; }
    void SetCenter(const Point3D& p) { m_center = p; }

    void SetColor1(const RGBColor& c)    { m_color1     = c; }
    void SetColor2(const RGBColor& c)    { m_color2     = c; }
    void SetLineColor(const RGBColor& c) { m_line_color = c; }

private:
	int   m_num_angular_checkers = 10;
	int   m_num_radial_checkers  = 20;
	float m_angular_line_width   = 0;
	float m_radial_line_width    = 0;

	float   m_radius = 1.0f;
	Point3D m_center = Point3D(0, 0, 0);

	RGBColor m_color1     = RGBColor(1.0f, 1.0f, 1.0f);   // checker color 1
	RGBColor m_color2     = RGBColor(0.5f, 0.5f, 0.5f);   // checker color 2
	RGBColor m_line_color = RGBColor(0, 0, 0);            // the line color

}; // DiskChecker

}