#pragma once

#include "raytracing/texture/Texture.h"
#include "raytracing/maths/Point3D.h"
#include "raytracing/maths/Vector3D.h"

namespace rt
{

class RectangleChecker : public Texture
{
public:
    RectangleChecker() {}

    virtual RGBColor GetColor(const ShadeRec& sr) const override;

    void SetNumXCheckers(int num) { m_num_x_checkers = num; }
    void SetNumZCheckers(int num) { m_num_z_checkers = num; }
    void SetXLineWidth(float width) { m_x_line_width = width; }
    void SetZLineWidth(float width) { m_z_line_width = width; }

    void SetP0(const Point3D& p) { m_p0 = p; }
    void SetA(const Vector3D& v) { m_a = v; }
    void SetB(const Vector3D& v) { m_b = v; }

    void SetColor1(const RGBColor& c)    { m_color1     = c; }
    void SetColor2(const RGBColor& c)    { m_color2     = c; }
    void SetLineColor(const RGBColor& c) { m_line_color = c; }

private:
	int   m_num_x_checkers = 20;
	int   m_num_z_checkers = 20;
	float m_x_line_width   = 0;
	float m_z_line_width   = 0;

	Point3D  m_p0 = Point3D(-1, 0, -1);
	Vector3D m_a  = Vector3D(0, 0, 2);
	Vector3D m_b  = Vector3D(2, 0, 0);

	RGBColor m_color1     = RGBColor(1.0f, 1.0f, 1.0f);   // checker color 1
	RGBColor m_color2     = RGBColor(0.5f, 0.5f, 0.5f);   // checker color 2
	RGBColor m_line_color = RGBColor(0, 0, 0);            // the line color

}; // RectangleChecker

}