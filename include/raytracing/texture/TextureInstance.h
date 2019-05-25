#pragma once

#include "raytracing/texture/Texture.h"
#include "raytracing/maths/Matrix.h"

#include <memory>

namespace rt
{

class Texture;
class Vector3D;

class TextureInstance : public Texture
{
public:
    TextureInstance() {}

    virtual RGBColor GetColor(const ShadeRec& sr) const override;

    void Translate(const Vector3D& trans);
    void Scale(const Vector3D& scale);
    void RotateX(double r);
    void RotateY(double r);
    void RotateZ(double r);
    void Shear(const Matrix& m);

    void SetTexture(const std::shared_ptr<Texture>& tex) {
        texture = tex;
    }

private:
    std::shared_ptr<Texture> texture = nullptr;	 // texture being transformed
    Matrix inv_matrix;		                     // inverse transformation matrix

}; // TextureInstance

}