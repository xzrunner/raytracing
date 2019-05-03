#include "raytracing/maths/Vector3D.h"
#include "raytracing/maths//Point3D.h"
#include "raytracing/maths//Normal.h"
#include "raytracing/maths//Matrix.h"

namespace rt
{

Vector3D::Vector3D(const Point3D& p)
	: x(p.x), y(p.y), z(p.z)
{}

Vector3D::Vector3D(const Normal& n)
	: x(n.x), y(n.y), z(n.z)
{}

Vector3D operator * (const Matrix& mat, const Vector3D& v)
{
	return (Point3D(mat.m[0][0] * v.x + mat.m[0][1] * v.y + mat.m[0][2] * v.z,
		mat.m[1][0] * v.x + mat.m[1][1] * v.y + mat.m[1][2] * v.z,
		mat.m[2][0] * v.x + mat.m[2][1] * v.y + mat.m[2][2] * v.z));
}

}