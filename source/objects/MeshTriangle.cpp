#include "raytracing/objects/MeshTriangle.h"
#include "raytracing/utilities/Mesh.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/maths/Ray.h"

#include <algorithm>

namespace rt
{

MeshTriangle::MeshTriangle(std::shared_ptr<Mesh> mesh, int i1, int i2, int i3)
    : mesh(std::move(mesh))
    , index0(i1)
    , index1(i2)
    , index2(i3)
{
}

bool MeshTriangle::ShadowHit(const Ray& ray, float& tmin) const
{
    if (!m_shadows) {
        return false;
    }

	Point3D v0(mesh->vertices[index0]);
	Point3D v1(mesh->vertices[index1]);
	Point3D v2(mesh->vertices[index2]);

	double a = v0.x - v1.x, b = v0.x - v2.x, c = ray.dir.x, d = v0.x - ray.ori.x;
	double e = v0.y - v1.y, f = v0.y - v2.y, g = ray.dir.y, h = v0.y - ray.ori.y;
	double i = v0.z - v1.z, j = v0.z - v2.z, k = ray.dir.z, l = v0.z - ray.ori.z;

	double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	double q = g * i - e * k, s = e * j - f * i;

	double inv_denom  = 1.0 / (a * m + b * q + c * s);

	double e1 = d * m - b * n - c * p;
	double beta = e1 * inv_denom;

	if (beta < 0.0)
	 	return false;

	double r = r = e * l - h * i;
	double e2 = a * n + d * q + c * r;
	double gamma = e2 * inv_denom;

	if (gamma < 0.0 )
	 	return false;

	if (beta + gamma > 1.0)
		return false;

	double e3 = a * p - b * r + d * s;
	double t = e3 * inv_denom;

	if (t < EPSILON)
		return false;

	tmin = static_cast<float>(t);

	return true;
}

AABB MeshTriangle::GetBoundingBox() const
{
	double delta = 0.0001;  // to avoid degenerate bounding boxes

	Point3D v1(mesh->vertices[index0]);
	Point3D v2(mesh->vertices[index1]);
	Point3D v3(mesh->vertices[index2]);

	return(AABB(std::min(std::min(v1.x, v2.x), v3.x) - delta, std::max(std::max(v1.x, v2.x), v3.x) + delta,
				std::min(std::min(v1.y, v2.y), v3.y) - delta, std::max(std::max(v1.y, v2.y), v3.y) + delta,
				std::min(std::min(v1.z, v2.z), v3.z) - delta, std::max(std::max(v1.z, v2.z), v3.z) + delta));
}

void MeshTriangle::ComputeNormal(bool reverse_normal)
{
    normal = (mesh->vertices[index1] - mesh->vertices[index0]) ^
        (mesh->vertices[index2] - mesh->vertices[index0]);
    normal.Normalize();

    if (reverse_normal)
        normal = -normal;
}

}