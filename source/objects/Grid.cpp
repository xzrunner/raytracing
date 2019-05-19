#include "raytracing/objects/Grid.h"
#include "raytracing/objects/Triangle.h"
#include "raytracing/objects/SmoothTriangle.h"
#include "raytracing/objects/FlatMeshTriangle.h"
#include "raytracing/objects/SmoothMeshTriangle.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/maths/maths.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/utilities/Mesh.h"

#define TINYPLY_IMPLEMENTATION
#include "external/tinyply.h"

#include <fstream>

#include <float.h>
#include <assert.h>

namespace
{

enum TriangleType
{
	Flat,
	Smooth
};

}

namespace rt
{

Grid::Grid()
	: nx(0)
	, ny(0)
	, nz(0)
	, mesh(std::make_unique<Mesh>())
	, reverse_normal(false)
{
}

Grid::~Grid()
{
}

bool Grid::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double ox = ray.ori.x;
	double oy = ray.ori.y;
	double oz = ray.ori.z;
	double dx = ray.dir.x;
	double dy = ray.dir.y;
	double dz = ray.dir.z;

	double x0 = aabb.x0;
	double y0 = aabb.y0;
	double z0 = aabb.z0;
	double x1 = aabb.x1;
	double y1 = aabb.y1;
	double z1 = aabb.z1;

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max;

	// the following code includes modifications from Shirley and Morley (2003)

	double a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}

	double b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}

	double c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}

	double t0, t1;

	if (tx_min > ty_min)
		t0 = tx_min;
	else
		t0 = ty_min;

	if (tz_min > t0)
		t0 = tz_min;

	if (tx_max < ty_max)
		t1 = tx_max;
	else
		t1 = ty_max;

	if (tz_max < t1)
		t1 = tz_max;

	if (t0 > t1)
		return(false);


	// initial cell coordinates

	int ix, iy, iz;

	if (aabb.Inside(ray.ori)) {  			// does the ray start inside the grid?
		ix = static_cast<int>(clamp((ox - x0) * nx / (x1 - x0), 0, nx - 1));
		iy = static_cast<int>(clamp((oy - y0) * ny / (y1 - y0), 0, ny - 1));
		iz = static_cast<int>(clamp((oz - z0) * nz / (z1 - z0), 0, nz - 1));
	}
	else {
		Point3D p = ray.ori + t0 * ray.dir;  // initial hit point with grid's bounding box
		ix = static_cast<int>(clamp((p.x - x0) * nx / (x1 - x0), 0, nx - 1));
		iy = static_cast<int>(clamp((p.y - y0) * ny / (y1 - y0), 0, ny - 1));
		iz = static_cast<int>(clamp((p.z - z0) * nz / (z1 - z0), 0, nz - 1));
	}

	// ray parameter increments per cell in the x, y, and z directions

	double dtx = (tx_max - tx_min) / nx;
	double dty = (ty_max - ty_min) / ny;
	double dtz = (tz_max - tz_min) / nz;

	double 	tx_next, ty_next, tz_next;
	int 	ix_step, iy_step, iz_step;
	int 	ix_stop, iy_stop, iz_stop;

	if (dx > 0) {
		tx_next = tx_min + (ix + 1) * dtx;
		ix_step = +1;
		ix_stop = nx;
	}
	else {
		tx_next = tx_min + (nx - ix) * dtx;
		ix_step = -1;
		ix_stop = -1;
	}

	if (dx == 0.0) {
		tx_next = FLT_MAX;
		ix_step = -1;
		ix_stop = -1;
	}


	if (dy > 0) {
		ty_next = ty_min + (iy + 1) * dty;
		iy_step = +1;
		iy_stop = ny;
	}
	else {
		ty_next = ty_min + (ny - iy) * dty;
		iy_step = -1;
		iy_stop = -1;
	}

	if (dy == 0.0) {
		ty_next = FLT_MAX;
		iy_step = -1;
		iy_stop = -1;
	}

	if (dz > 0) {
		tz_next = tz_min + (iz + 1) * dtz;
		iz_step = +1;
		iz_stop = nz;
	}
	else {
		tz_next = tz_min + (nz - iz) * dtz;
		iz_step = -1;
		iz_stop = -1;
	}

	if (dz == 0.0) {
		tz_next = FLT_MAX;
		iz_step = -1;
		iz_stop = -1;
	}


	// traverse the grid

	while (true) {
		auto& object_ptr = cells[ix + nx * iy + nx * ny * iz];

		if (tx_next < ty_next && tx_next < tz_next) {
			if (object_ptr && object_ptr->Hit(ray, tmin, sr) && tmin < tx_next) {
				m_material = object_ptr->GetMaterial();
				return true;
			}

			tx_next += dtx;
			ix += ix_step;

			if (ix == ix_stop)
				return false;
		}
		else {
			if (ty_next < tz_next) {
				if (object_ptr && object_ptr->Hit(ray, tmin, sr) && tmin < ty_next) {
					m_material = object_ptr->GetMaterial();
					return true;
				}

				ty_next += dty;
				iy += iy_step;

				if (iy == iy_stop)
					return false;
			}
			else {
				if (object_ptr && object_ptr->Hit(ray, tmin, sr) && tmin < tz_next) {
                    m_material = object_ptr->GetMaterial();
					return true;
				}

				tz_next += dtz;
				iz += iz_step;

				if (iz == iz_stop)
					return false;
			}
		}
	}
}

bool Grid::ShadowHit(const Ray& ray, float& tmin) const
{
	float t = 10000;

	double ox = ray.ori.x;
	double oy = ray.ori.y;
	double oz = ray.ori.z;
	double dx = ray.dir.x;
	double dy = ray.dir.y;
	double dz = ray.dir.z;

	double x0 = aabb.x0;
	double y0 = aabb.y0;
	double z0 = aabb.z0;
	double x1 = aabb.x1;
	double y1 = aabb.y1;
	double z1 = aabb.z1;

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max;

	// the following code includes modifications from Shirley and Morley (2003)

	double a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}

	double b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}

	double c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}

	double t0, t1;

	if (tx_min > ty_min)
		t0 = tx_min;
	else
		t0 = ty_min;

	if (tz_min > t0)
		t0 = tz_min;

	if (tx_max < ty_max)
		t1 = tx_max;
	else
		t1 = ty_max;

	if (tz_max < t1)
		t1 = tz_max;

	if (t0 > t1)
		return(false);


	// initial cell coordinates

	int ix, iy, iz;

	if (aabb.Inside(ray.ori)) {  			// does the ray start inside the grid?
		ix = clamp((ox - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = clamp((oy - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = clamp((oz - z0) * nz / (z1 - z0), 0, nz - 1);
	}
	else {
		Point3D p = ray.ori + t0 * ray.dir;  // initial hit point with grid's bounding box
		ix = clamp((p.x - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = clamp((p.y - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = clamp((p.z - z0) * nz / (z1 - z0), 0, nz - 1);
	}

	// ray parameter increments per cell in the x, y, and z directions

	double dtx = (tx_max - tx_min) / nx;
	double dty = (ty_max - ty_min) / ny;
	double dtz = (tz_max - tz_min) / nz;

	double 	tx_next, ty_next, tz_next;
	int 	ix_step, iy_step, iz_step;
	int 	ix_stop, iy_stop, iz_stop;

	if (dx > 0) {
		tx_next = tx_min + (ix + 1) * dtx;
		ix_step = +1;
		ix_stop = nx;
	}
	else {
		tx_next = tx_min + (nx - ix) * dtx;
		ix_step = -1;
		ix_stop = -1;
	}

	if (dx == 0.0) {
		tx_next = HUGE_VALUE;
		ix_step = -1;
		ix_stop = -1;
	}


	if (dy > 0) {
		ty_next = ty_min + (iy + 1) * dty;
		iy_step = +1;
		iy_stop = ny;
	}
	else {
		ty_next = ty_min + (ny - iy) * dty;
		iy_step = -1;
		iy_stop = -1;
	}

	if (dy == 0.0) {
		ty_next = HUGE_VALUE;
		iy_step = -1;
		iy_stop = -1;
	}

	if (dz > 0) {
		tz_next = tz_min + (iz + 1) * dtz;
		iz_step = +1;
		iz_stop = nz;
	}
	else {
		tz_next = tz_min + (nz - iz) * dtz;
		iz_step = -1;
		iz_stop = -1;
	}

	if (dz == 0.0) {
		tz_next = HUGE_VALUE;
		iz_step = -1;
		iz_stop = -1;
	}

	// traverse the grid

	while (true) {
		auto object_ptr = cells[ix + nx * iy + nx * ny * iz];

		if (tx_next < ty_next && tx_next < tz_next) {
			if (object_ptr && object_ptr->ShadowHit(ray, t) && t < tmin) {		//This part is different from hit function, Notice it!
				tmin = t;
				return (true);
			}

			tx_next += dtx;
			ix += ix_step;
			if (ix == ix_stop)
				return (false);
		}
		else {
			if (ty_next < tz_next) {
				if (object_ptr && object_ptr->ShadowHit(ray, t) && t < tmin) {
					tmin = t;
					return (true);
				}

				ty_next += dty;
				iy += iy_step;

				if (iy == iy_stop)
					return (false);
		 	}
		 	else {
				if (object_ptr && object_ptr->ShadowHit(ray, t) && t < tmin) {
					tmin = t;
					return (true);
				}

				tz_next += dtz;
				iz += iz_step;

				if (iz == iz_stop)
					return (false);
		 	}
		}
	}
}

AABB Grid::GetBoundingBox() const
{
	return aabb;
}

void Grid::SetMaterial(const std::shared_ptr<Material>& material) const
{
    Compound::SetMaterial(material);
    for (auto& obj : cells) {
        if (obj) {
            obj->SetMaterial(material);
        }
    }
}

void Grid::SetupCells(void)
{
	// find the minimum and maximum coordinates of the grid

	Point3D p0 = FindMinBounds();
	Point3D p1 = FindMaxBounds();

	aabb.x0 = p0.x;
	aabb.y0 = p0.y;
	aabb.z0 = p0.z;
	aabb.x1 = p1.x;
	aabb.y1 = p1.y;
	aabb.z1 = p1.z;

	// compute the number of grid cells in the x, y, and z directions

	int num_objects = m_parts.size();

	// dimensions of the grid in the x, y, and z directions

	double wx = p1.x - p0.x;
	double wy = p1.y - p0.y;
	double wz = p1.z - p0.z;

	double multiplier = 2.0;  	// multiplyer scales the number of grid cells relative to the number of m_objects

	double s = pow(wx * wy * wz / num_objects, 0.3333333);
	nx = static_cast<int>(multiplier * wx / s + 1);
	ny = static_cast<int>(multiplier * wy / s + 1);
	nz = static_cast<int>(multiplier * wz / s + 1);

	// set up the array of grid cells with null pointers

	int num_cells = nx * ny * nz;
	cells.reserve(num_objects);

	for (int j = 0; j < num_cells; j++)
		cells.push_back(nullptr);

	// set up a temporary array to hold the number of m_objects stored in each cell

	std::vector<int> counts;
	counts.reserve(num_cells);

	for (int j = 0; j < num_cells; j++)
		counts.push_back(0);


	// put the m_objects into the cells

	AABB obj_bBox; 	// object's bounding box
	int index;  	// cell's array index

	for (int j = 0; j < num_objects; j++) {
		obj_bBox =  m_parts[j]->GetBoundingBox();

		// compute the cell indices at the corners of the bounding box of the object

		int ixmin = static_cast<int>(clamp((obj_bBox.x0 - p0.x) * nx / (p1.x - p0.x), 0, nx - 1));
		int iymin = static_cast<int>(clamp((obj_bBox.y0 - p0.y) * ny / (p1.y - p0.y), 0, ny - 1));
		int izmin = static_cast<int>(clamp((obj_bBox.z0 - p0.z) * nz / (p1.z - p0.z), 0, nz - 1));
		int ixmax = static_cast<int>(clamp((obj_bBox.x1 - p0.x) * nx / (p1.x - p0.x), 0, nx - 1));
		int iymax = static_cast<int>(clamp((obj_bBox.y1 - p0.y) * ny / (p1.y - p0.y), 0, ny - 1));
		int izmax = static_cast<int>(clamp((obj_bBox.z1 - p0.z) * nz / (p1.z - p0.z), 0, nz - 1));

		// add the object to the cells

		for (int iz = izmin; iz <= izmax; iz++) 					// cells in z direction
			for (int iy = iymin; iy <= iymax; iy++)					// cells in y direction
				for (int ix = ixmin; ix <= ixmax; ix++) {			// cells in x direction
					index = ix + nx * iy + nx * ny * iz;

					if (counts[index] == 0) {
						cells[index] = m_parts[j];
						counts[index] += 1;  						// now = 1
					}
					else {
						if (counts[index] == 1) {
							auto compound_ptr = std::make_shared<Compound>();	// construct a compound object
							compound_ptr->AddObject(cells[index]); // add object already in cell
							compound_ptr->AddObject(m_parts[j]);  	// add the new object
							cells[index] = compound_ptr;			// store compound in current cell
							counts[index] += 1;  					// now = 2
						}
						else {
							// counts[index] > 1
							cells[index]->AddObject(m_parts[j]);	// just add current object
							counts[index] += 1;						// for statistics only
						}
					}
				}
	}  // end of for (int j = 0; j < num_objects; j++)


	// erase the Compound::std::vector that stores the object pointers, but don't delete the m_objects
	ClearObjects();

	// display some statistics on counts
	// this is useful for finding out how many cells have no m_objects, one object, etc
	// comment this out if you don't want to use it

	int num_zeroes 	= 0;
	int num_ones 	= 0;
	int num_twos 	= 0;
	int num_threes 	= 0;
	int num_greater = 0;

	for (int j = 0; j < num_cells; j++) {
		if (counts[j] == 0)
			num_zeroes += 1;
		if (counts[j] == 1)
			num_ones += 1;
		if (counts[j] == 2)
			num_twos += 1;
		if (counts[j] == 3)
			num_threes += 1;
		if (counts[j] > 3)
			num_greater += 1;
	}

	std::cout << "num_cells =" << num_cells << std::endl;
	std::cout << "numZeroes = " << num_zeroes << "  numOnes = " << num_ones << "  numTwos = " << num_twos << std::endl;
	std::cout << "numThrees = " << num_threes << "  numGreater = " << num_greater << std::endl;

	// erase the temporary counts std::vector

	counts.erase (counts.begin(), counts.end());
}

void Grid::ReadFlatTriangles(const std::string& filename)
{
    ReadPlyFile(filename, TriangleType::Flat);
}

void Grid::ReadSmoothTriangles(const std::string& filename)
{
    ReadPlyFile(filename, TriangleType::Smooth);
    ComputeMeshNormals();
}

void Grid::TessellateFlatSphere(int horizontal_steps, int vertical_steps)
{
	double pi = 3.1415926535897932384;

	// define the top triangles which all touch the north pole

	int k = 1;

	for (int j = 0; j <= horizontal_steps - 1; j++) {
		// define vertices

		Point3D v0(	0, 1, 0);																		// top (north pole)

		Point3D v1(	sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 			// bottom left
					cos(pi * k / vertical_steps),
					cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);

		Point3D v2(	sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), 		// bottom  right
					cos(pi * k / vertical_steps),
					cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps)	);

		auto triangle_ptr = std::make_shared<Triangle>(v0, v1, v2);
		m_parts.push_back(triangle_ptr);
	}


	// define the bottom triangles which all touch the south pole

	k = vertical_steps - 1;

	for (int j = 0; j <= horizontal_steps - 1; j++) {
		// define vertices

		Point3D v0(	sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 			// top left
					cos(pi * k / vertical_steps),
					cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);

		Point3D v1(	0, -1, 0);																		// bottom (south pole)

		Point3D v2(	sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), 		// top right
					cos(pi * k / vertical_steps),
					cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps)	);

		auto triangle_ptr = std::make_shared<Triangle>(v0, v1, v2);
		m_parts.push_back(triangle_ptr);
	}



	//  define the other triangles

	for (int k = 1; k <= vertical_steps - 2; k++) {
		for (int j = 0; j <= horizontal_steps - 1; j++) {
			// define the first triangle

			// vertices

			Point3D v0(	sin(2.0 * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), 				// bottom left, use k + 1, j
						cos(pi * (k + 1) / vertical_steps),
						cos(2.0 * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)	);

			Point3D v1(	sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), 		// bottom  right, use k + 1, j + 1
						cos(pi * (k + 1) / vertical_steps),
						cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)	);

			Point3D v2(	sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 					// top left, 	use k, j
						cos(pi * k / vertical_steps),
						cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);

			auto triangle_ptr1 = std::make_shared<Triangle>(v0, v1, v2);
			m_parts.push_back(triangle_ptr1);


			// define the second triangle

			// vertices

			v0 = Point3D(	sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), 			// top right, use k, j + 1
							cos(pi * k / vertical_steps),
							cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps) );

			v1 = Point3D (	sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 				// top left, 	use k, j
							cos(pi * k / vertical_steps),
							cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);

			v2 = Point3D (	sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), 	// bottom  right, use k + 1, j + 1
							cos(pi * (k + 1) / vertical_steps),
							cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)	);

			auto triangle_ptr2 = std::make_shared<Triangle>(v0, v1, v2);
			m_parts.push_back(triangle_ptr2);
		}
	}
}

void Grid::TessellateSmoothSphere(int horizontal_steps, int vertical_steps)
{
	double pi = 3.1415926535897932384;

	// define the top triangles

	int k = 1;

	for (int j = 0; j <= horizontal_steps - 1; j++) {
		// define vertices

		Point3D v0(	0, 1, 0);																		// top

		Point3D v1(	sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 			// bottom left
					cos(pi * k / vertical_steps),
					cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);

		Point3D v2(	sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), 		// bottom  right
					cos(pi * k / vertical_steps),
					cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps)	);

		auto triangle_ptr = std::make_shared<SmoothTriangle>(v0, v1, v2);
        triangle_ptr->SetNormals(v0, v1, v2);
		m_parts.push_back(triangle_ptr);
	}


	// define the bottom triangles

	k = vertical_steps - 1;

	for (int j = 0; j <= horizontal_steps - 1; j++) {
		// define vertices

		Point3D v0(	sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 			// top left
					cos(pi * k / vertical_steps),
					cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);

		Point3D v1(	0, -1, 0);																		// bottom

		Point3D v2(	sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), 		// top right
					cos(pi * k / vertical_steps),
					cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps)	);

		auto triangle_ptr = std::make_shared<SmoothTriangle>(v0, v1, v2);
        triangle_ptr->SetNormals(v0, v1, v2);
		m_parts.push_back(triangle_ptr);
	}


	//  define the other triangles

	for (int k = 1; k <= vertical_steps - 2; k++) {
		for (int j = 0; j <= horizontal_steps - 1; j++) {
			// define the first triangle

			// vertices

			Point3D v0(	sin(2.0 * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), 				// bottom left, use k + 1, j
						cos(pi * (k + 1) / vertical_steps),
						cos(2.0 * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)	);

			Point3D v1(	sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), 		// bottom  right, use k + 1, j + 1
						cos(pi * (k + 1) / vertical_steps),
						cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)	);

			Point3D v2(	sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 					// top left, 	use k, j
						cos(pi * k / vertical_steps),
						cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);

			auto triangle_ptr1 = std::make_shared<SmoothTriangle>(v0, v1, v2);
            triangle_ptr1->SetNormals(v0, v1, v2);
			m_parts.push_back(triangle_ptr1);


			// define the second triangle

			// vertices

			v0 = Point3D(	sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), 			// top right, use k, j + 1
							cos(pi * k / vertical_steps),
							cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps) );

			v1 = Point3D (	sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), 				// top left, 	use k, j
							cos(pi * k / vertical_steps),
							cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)	);

			v2 = Point3D (	sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), 	// bottom  right, use k + 1, j + 1
							cos(pi * (k + 1) / vertical_steps),
							cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)	);

			auto triangle_ptr2 = std::make_shared<SmoothTriangle>(v0, v1, v2);
            triangle_ptr2->SetNormals(v0, v1, v2);
			m_parts.push_back(triangle_ptr2);
		}
	}
}

Point3D Grid::FindMinBounds() const
{
	AABB object_box;
	Point3D p0(FLT_MAX, FLT_MAX, FLT_MAX);

	int num_objects = m_parts.size();
	for (int j = 0; j < num_objects; j++) {
		object_box = m_parts[j]->GetBoundingBox();

        if (object_box.x0 < p0.x) {
            p0.x = static_cast<float>(object_box.x0);
        }
        if (object_box.y0 < p0.y) {
            p0.y = static_cast<float>(object_box.y0);
        }
        if (object_box.z0 < p0.z) {
            p0.z = static_cast<float>(object_box.z0);
        }
	}

	p0.x -= static_cast<float>(EPSILON);
    p0.y -= static_cast<float>(EPSILON);
    p0.z -= static_cast<float>(EPSILON);

	return p0;
}

Point3D Grid::FindMaxBounds() const
{
	AABB object_box;
	Point3D p1(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	int num_objects = m_parts.size();
	for (int j = 0; j < num_objects; j++) {
		object_box = m_parts[j]->GetBoundingBox();

        if (object_box.x1 > p1.x) {
            p1.x = static_cast<float>(object_box.x1);
        }
        if (object_box.y1 > p1.y) {
            p1.y = static_cast<float>(object_box.y1);
        }
        if (object_box.z1 > p1.z) {
            p1.z = static_cast<float>(object_box.z1);
        }
	}

	p1.x += static_cast<float>(EPSILON);
    p1.y += static_cast<float>(EPSILON);
    p1.z += static_cast<float>(EPSILON);

	return p1;
}

void Grid::ReadPlyFile(const std::string& filename, int triangle_type)
{
    try {
        std::ifstream ss(filename, std::ios::binary);
        if (ss.fail()) throw std::runtime_error("failed to open " + filename);

        PlyFile file;
        file.parse_header(ss);

        std::cout << "........................................................................\n";
        for (auto c : file.get_comments()) std::cout << "Comment: " << c << std::endl;
        for (auto e : file.get_elements())
        {
            std::cout << "element - " << e.name << " (" << e.size << ")" << std::endl;
            for (auto p : e.properties) std::cout << "\tproperty - " << p.name << " (" << tinyply::PropertyTable[p.propertyType].str << ")" << std::endl;
        }
        std::cout << "........................................................................\n";

		// Tinyply treats parsed data as untyped byte buffers. See below for examples.
		std::shared_ptr<PlyData> vertices, normals, faces, texcoords;

		// The header information can be used to programmatically extract properties on elements
		// known to exist in the header prior to reading the data. For brevity of this sample, properties
		// like vertex position are hard-coded:
		try { vertices = file.request_properties_from_element("vertex", { "x", "y", "z" }); }
		catch (const std::exception & e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

		try { normals = file.request_properties_from_element("vertex", { "nx", "ny", "nz" }); }
		catch (const std::exception & e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

		try { texcoords = file.request_properties_from_element("vertex", { "u", "v" }); }
		catch (const std::exception & e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

		// Providing a list size hint (the last argument) is a 2x performance improvement. If you have
		// arbitrary ply files, it is best to leave this 0.
		try { faces = file.request_properties_from_element("face", { "vertex_indices" }, 3); }
		catch (const std::exception & e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

		file.read(ss);

		if (vertices) std::cout << "\tRead " << vertices->count << " total vertices "<< std::endl;
		if (normals) std::cout << "\tRead " << normals->count << " total vertex normals " << std::endl;
		if (texcoords) std::cout << "\tRead " << texcoords->count << " total vertex texcoords " << std::endl;
		if (faces) std::cout << "\tRead " << faces->count << " total faces (triangles) " << std::endl;

        mesh->num_vertices = vertices->count;
        mesh->vertices.resize(vertices->count);
        const size_t num_vertices_bytes = vertices->buffer.size_bytes();
        std::memcpy(mesh->vertices.data(), vertices->buffer.get(), num_vertices_bytes);

        mesh->num_triangles = faces->count;
        mesh->vertex_faces.resize(faces->count);
        assert(faces->t == tinyply::Type::INT32);
        std::vector<int> face_data(faces->count * 3);
        const size_t num_faces_bytes = faces->buffer.size_bytes();
        assert(num_faces_bytes == faces->count * 3 * sizeof(int));
        std::memcpy(face_data.data(), faces->buffer.get(), num_faces_bytes);
        int idx_ptr = 0;
        int count = 0; // the number of faces read
        for (size_t i = 0; i < faces->count; ++i)
        {
            std::vector<int> face(3);
            face[0] = face_data[idx_ptr++];
            face[1] = face_data[idx_ptr++];
            face[2] = face_data[idx_ptr++];
            mesh->vertex_faces[i] = face;

			if (triangle_type == Flat) {
			    auto triangle_ptr = std::make_shared<FlatMeshTriangle>(mesh, face[0], face[1], face[2]);
				triangle_ptr->ComputeNormal(reverse_normal);
				m_parts.push_back(triangle_ptr);
			}

			if (triangle_type == Smooth) {
			    auto triangle_ptr = std::make_shared<SmoothMeshTriangle>(mesh, face[0], face[1], face[2]);
				triangle_ptr->ComputeNormal(reverse_normal); 	// the "flat triangle" normal is used to compute the average normal at each mesh vertex
				m_parts.push_back(triangle_ptr); 				// it's quicker to do it once here, than have to do it on average 6 times in compute_mesh_normals

				// the following code stores a list of all faces that share a vertex
				// it's used for computing the average normal at each vertex in order(num_vertices) time

				mesh->vertex_faces[face[0]].push_back(count);
				mesh->vertex_faces[face[1]].push_back(count);
				mesh->vertex_faces[face[2]].push_back(count);
				count++;
			}
        }
        if (triangle_type == Flat)
            mesh->vertex_faces.erase(mesh->vertex_faces.begin(), mesh->vertex_faces.end());
    } catch (const std::exception & e) {
        std::cerr << "Caught tinyply exception: " << e.what() << std::endl;
    }
}

void Grid::ComputeMeshNormals()
{
	mesh->normals.reserve(mesh->num_vertices);

    // for each vertex
	for (int index = 0; index < mesh->num_vertices; index++)
    {
		Normal normal;    // is zero at this point

        for (size_t j = 0; j < mesh->vertex_faces[index].size(); j++) {
            normal += m_parts[mesh->vertex_faces[index][j]]->GetNormal();
        }

		// The following code attempts to avoid (nan, nan, nan) normalised normals when all components = 0

        if (normal.x == 0.0 && normal.y == 0.0 && normal.z == 0.0) {
            normal.y = 1.0;
        }
        else {
            normal.Normalize();
        }

		mesh->normals.push_back(normal);
	}

	// erase the vertex_faces arrays because we have now finished with them

    for (int index = 0; index < mesh->num_vertices; index++) {
        for (size_t j = 0; j < mesh->vertex_faces[index].size(); j++) {
            mesh->vertex_faces[index].erase(mesh->vertex_faces[index].begin(), mesh->vertex_faces[index].end());
        }
    }

	mesh->vertex_faces.erase (mesh->vertex_faces.begin(), mesh->vertex_faces.end());

	cout << "finished constructing normals" << endl;
}

}