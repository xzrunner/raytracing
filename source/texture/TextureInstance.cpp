#include "raytracing/texture/TextureInstance.h"
#include "raytracing/maths/Vector3D.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

RGBColor TextureInstance::GetColor(const ShadeRec& sr) const
{
    ShadeRec local_sr(sr);
    local_sr.local_hit_point = inv_matrix * local_sr.local_hit_point;

    return texture->GetColor(local_sr);
}

void TextureInstance::Translate(const Vector3D& trans)
{
	Matrix inv_translation_matrix;				// temporary inverse translation matrix

	inv_translation_matrix.m[0][3] = -trans.x;
	inv_translation_matrix.m[1][3] = -trans.y;
	inv_translation_matrix.m[2][3] = -trans.z;

	inv_matrix = inv_matrix * inv_translation_matrix;
}

void TextureInstance::Scale(const Vector3D& scale)
{
    Matrix inv_scaling_matrix;			// temporary inverse scaling matrix

	inv_scaling_matrix.m[0][0] = 1.0 / scale.x;
	inv_scaling_matrix.m[1][1] = 1.0 / scale.y;
	inv_scaling_matrix.m[2][2] = 1.0 / scale.z;

	inv_matrix = inv_matrix * inv_scaling_matrix;
}

void TextureInstance::RotateX(double r)
{
	double sin_theta = sin(r * PI_ON_180);
	double cos_theta = cos(r * PI_ON_180);

	Matrix inv_x_rotation_matrix;					// temporary inverse rotation matrix about x axis

	inv_x_rotation_matrix.m[1][1] = cos_theta;
	inv_x_rotation_matrix.m[1][2] = sin_theta;
	inv_x_rotation_matrix.m[2][1] = -sin_theta;
	inv_x_rotation_matrix.m[2][2] = cos_theta;

	inv_matrix = inv_matrix * inv_x_rotation_matrix;
}

void TextureInstance::RotateY(double r)
{
	double sin_theta = sin(r * PI / 180.0);
	double cos_theta = cos(r * PI / 180.0);

	Matrix inv_y_rotation_matrix;					// temporary inverse rotation matrix about y axis

	inv_y_rotation_matrix.m[0][0] = cos_theta;
	inv_y_rotation_matrix.m[0][2] = -sin_theta;
	inv_y_rotation_matrix.m[2][0] = sin_theta;
	inv_y_rotation_matrix.m[2][2] = cos_theta;

	inv_matrix = inv_matrix * inv_y_rotation_matrix;
}

void TextureInstance::RotateZ(double r)
{
	double sin_theta = sin(r * PI / 180.0);
	double cos_theta = cos(r * PI / 180.0);

	Matrix inv_z_rotation_matrix;					// temporary inverse rotation matrix about y axis

	inv_z_rotation_matrix.m[0][0] = cos_theta;
	inv_z_rotation_matrix.m[0][1] = sin_theta;
	inv_z_rotation_matrix.m[1][0] = -sin_theta;
	inv_z_rotation_matrix.m[1][1] = cos_theta;

	inv_matrix = inv_matrix * inv_z_rotation_matrix;
}

void TextureInstance::Shear(const Matrix& s)
{
	Matrix inverse_shearing_matrix;    // inverse shear matrix

	// discriminant

	double d = 1.0 	- s.m[1][0] * s.m[0][1] - s.m[2][0] * s.m[0][2]  - s.m[2][1] * s.m[1][2]
					+ s.m[1][0] * s.m[2][1] * s.m[0][2] + s.m[2][0] * s.m[0][1] * s.m[2][1];

	// diagonals

	inverse_shearing_matrix.m[0][0] = 1.0 - s.m[2][1] * s.m[1][2];
	inverse_shearing_matrix.m[1][1] = 1.0 - s.m[2][0] * s.m[0][2];
	inverse_shearing_matrix.m[2][2] = 1.0 - s.m[1][0] * s.m[0][1];
	inverse_shearing_matrix.m[3][3] = d;

	// first row

	inverse_shearing_matrix.m[0][1] = -s.m[1][0] + s.m[2][0] * s.m[1][2];
	inverse_shearing_matrix.m[0][2] = -s.m[2][0] + s.m[1][0] * s.m[2][1];

	// second row

	inverse_shearing_matrix.m[1][0] = -s.m[0][1] + s.m[2][1] * s.m[0][2];
	inverse_shearing_matrix.m[1][2] = -s.m[2][1] + s.m[2][0] * s.m[0][1];

	// third row

	inverse_shearing_matrix.m[2][0] = -s.m[0][2] + s.m[0][1] * s.m[1][2];
	inverse_shearing_matrix.m[2][1] = -s.m[1][2] + s.m[1][0] * s.m[0][2] ;

	// divide by discriminant

	inverse_shearing_matrix = inverse_shearing_matrix / d;

	inv_matrix = inv_matrix * inverse_shearing_matrix;
}

}