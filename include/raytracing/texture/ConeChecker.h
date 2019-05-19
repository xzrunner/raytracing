#pragma once

#include "raytracing/texture/Texture.h"

namespace rt
{

class ConeChecker : public Texture
{
public:
    ConeChecker() {}

    virtual RGBColor GetColor(const ShadeRec& sr) const override;

    void SetNumHorizontalCheckers(int num_horizontal) { m_num_horizontal_checkers = num_horizontal; }
    void SetNumVerticalCheckers(int num_vertical)     { m_num_vertical_checkers   = num_vertical; }
    void SetHorizontalLineWidth(float width)          { m_horizontal_line_width   = width; }
    void SetVerticalLineWidth(float width)            { m_vertical_line_width     = width; }

    void SetColor1(const RGBColor& c)    { m_color1     = c; }
    void SetColor2(const RGBColor& c)    { m_color2     = c; }
    void SetLineColor(const RGBColor& c) { m_line_color = c; }

private:
	int	     m_num_horizontal_checkers = 20;	           // number of checkers in the horizontal (azithum) direction
	int	     m_num_vertical_checkers   = 10;		       // number of checkers in the vertical (polar) direction
	float    m_horizontal_line_width   = 0.0f;		       // width of the horizontal lines as a fraction of the checker width
	float    m_vertical_line_width     = 0.0f;		       // width of the vertical lines as a fraction of the checker width

	RGBColor m_color1     = RGBColor(1.0f, 1.0f, 1.0f);   // checker color 1
	RGBColor m_color2     = RGBColor(0.5f, 0.5f, 0.5f);   // checker color 2
	RGBColor m_line_color = RGBColor(0, 0, 0);            // the line color

	double   m_h = 2.0;

}; // ConeChecker

}