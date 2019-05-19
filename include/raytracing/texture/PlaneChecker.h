#pragma once

#include "raytracing/texture/Texture.h"

namespace rt
{

class PlaneChecker : public Texture
{
public:
    PlaneChecker();

    virtual RGBColor GetColor(const ShadeRec& sr) const override;

    void SetOutlineWidth(float width) { m_outline_width = width; }

    void SetColor1(const RGBColor& c)    { m_color1     = c; }
    void SetColor2(const RGBColor& c)    { m_color2     = c; }
    void SetOutlineColor(const RGBColor& c) { m_outline_color = c; }

    void SetSize(float size) { m_size = size; }

private:
    float    m_outline_width = 0;		// width of the horizontal lines as a fraction of the checker width
    RGBColor m_color1;					// checker color 1
    RGBColor m_color2;					// checker color 2
    RGBColor m_outline_color;			// the line color
    float    m_size = 0;

}; // PlaneChecker

}