#pragma once

#include "raytracing/texture/Texture.h"

#include <memory>

namespace rt
{

class SphereTextures : public Texture
{
public:
    SphereTextures() {}

    virtual RGBColor GetColor(const ShadeRec& sr) const override;

    void SetNumHorizontalCheckers(int num_horizontal) {
        m_num_horizontal_checkers = num_horizontal;
    }
    void SetNumVerticalCheckers(int num_vertical) {
        m_num_vertical_checkers = num_vertical;
    }

    void SetTexture1(std::unique_ptr<Texture>& tex);
    void SetTexture2(std::unique_ptr<Texture>& tex);

private:
    int	m_num_horizontal_checkers = 12;	    // number of checkers in the horizontal (azithum) direction
    int	m_num_vertical_checkers   = 6;		// number of checkers in the vertical (polar) direction
    std::unique_ptr<Texture> m_texture1 = nullptr;
    std::unique_ptr<Texture> m_texture2 = nullptr;

}; // SphereTextures

}