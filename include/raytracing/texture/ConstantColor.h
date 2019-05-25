#pragma once

#include "raytracing/texture/Texture.h"

namespace rt
{

class ConstantColor : public Texture
{
public:
    ConstantColor();

    virtual RGBColor GetColor(const ShadeRec& sr) const override {
        return m_color;
    }

    void SetColor(const RGBColor& c) { m_color = c; }

private:
    RGBColor m_color;

}; // ConstantColor

}