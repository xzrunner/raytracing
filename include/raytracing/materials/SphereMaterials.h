#pragma once

#include "raytracing/materials/Material.h"

#include <memory>

namespace rt
{

class SphereMaterials : public Material
{
public:
    SphereMaterials();

    virtual RGBColor GetLe(const ShadeRec& sr) const override;

    virtual RGBColor Shade(const ShadeRec& sr) const override;
    virtual RGBColor AreaLightShade(const ShadeRec& sr) const override;

    void SetNumHorizontal(int num_horizontal) {
        num_horizontal_checkers = num_horizontal;
    }
    void SetNumVertical(int num_vertical) {
        num_vertical_checkers = num_vertical;
    }

    void SetHoriLineWidth(float width) { horizontal_line_width = width; }
    void SetVertLineWidth(float width) { vertical_line_width = width; }

    void SetChecker1Color(const RGBColor& col) { color1 = col; }
    void SetChecker2Color(const RGBColor& col) { color2 = col; }
    void SetLineColor(const RGBColor& col) { line_color = col; }

    void SetChecker1Material(std::unique_ptr<Material>& material) {
        checker1_m = std::move(material);
    }
    void SetChecker2Material(std::unique_ptr<Material>& material) {
        checker2_m = std::move(material);
    }
    void SetLineMaterial(std::unique_ptr<Material>& material) {
        line_m = std::move(material);
    }

private:
    int	     num_horizontal_checkers = 10;	// number of checkers in the horizontal (azithum) direction
    int      num_vertical_checkers   = 20;	// number of checkers in the vertical (polar) direction
    float    horizontal_line_width   = 0;	// width of the horizontal lines as a fraction of the checker width
    float    vertical_line_width     = 0;	// width of the vertical lines as a fraction of the checker width
    RGBColor color1;					    // checker color 1
    RGBColor color2;					    // checker color 2
    RGBColor line_color;				    // the line color

    std::unique_ptr<Material> checker1_m = nullptr;
    std::unique_ptr<Material> checker2_m = nullptr;
    std::unique_ptr<Material> line_m     = nullptr;

}; // SphereMaterials

}