#include "raytracing/objects/Grid.h"
#include "raytracing/objects/FlatMeshTriangle.h"
#include "raytracing/objects/SmoothMeshTriangle.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/maths/maths.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/utilities/Mesh.h"

#include "external/ply.h"

#include <float.h>
#include <iostream>

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
				SetMaterial(object_ptr->GetMaterial());
				return (true);
			}

			tx_next += dtx;
			ix += ix_step;

			if (ix == ix_stop)
				return (false);
		}
		else {
			if (ty_next < tz_next) {
				if (object_ptr && object_ptr->Hit(ray, tmin, sr) && tmin < ty_next) {
					SetMaterial(object_ptr->GetMaterial());
					return (true);
				}

				ty_next += dty;
				iy += iy_step;

				if (iy == iy_stop)
					return (false);
			}
			else {
				if (object_ptr && object_ptr->Hit(ray, tmin, sr) && tmin < tz_next) {
					SetMaterial(object_ptr->GetMaterial());
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
	// Vertex definition

	typedef struct Vertex {
	  float x,y,z;      // space coordinates
	} Vertex;

	// Face definition. This is the same for all files but is placed here to keep all the definitions together

	typedef struct Face {
	  	unsigned char nverts;    // number of vertex indices in list
	  	int* verts;              // vertex index list
	} Face;

	// list of property information for a vertex
	// this varies depending on what you are reading from the file

	PlyProperty vert_props[] = {
	  {"x", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,x), 0, 0, 0, 0},
	  {"y", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,y), 0, 0, 0, 0},
	  {"z", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,z), 0, 0, 0, 0}
	};

	// list of property information for a face.
	// there is a single property, which is a list
	// this is the same for all files

	PlyProperty face_props[] = {
	  	{"vertex_indices", PLY_INT, PLY_INT, offsetof(Face,verts),
	   		1, PLY_UCHAR, PLY_UCHAR, offsetof(Face,nverts)}
	};

	// local variables

	int 			i,j;
  	PlyFile*		ply;
  	int 			nelems;		// number of element types: 2 in our case - vertices and faces
  	char**			elist;
	int 			file_type;
	float 			version;
	int 			nprops;		// number of properties each element has
	int 			num_elems;	// number of each type of element: number of vertices or number of faces
	PlyProperty**	plist;
	Vertex**		vlist;
	Face**			flist;
	char*			elem_name;
	int				num_comments;
	char**			comments;
	int 			num_obj_info;
	char**			obj_info;


  	// open a ply file for reading

	ply = ply_open_for_reading(const_cast<char*>(filename.c_str()), &nelems, &elist, &file_type, &version);

  	// print what we found out about the file

  	printf ("version %f\n", version);
  	printf ("type %d\n", file_type);

  	// go through each kind of element that we learned is in the file and read them

  	for (i = 0; i < nelems; i++) {  // there are only two elements in our files: vertices and faces
	    // get the description of the first element

  	    elem_name = elist[i];
	    plist = ply_get_element_description (ply, elem_name, &num_elems, &nprops);

	    // print the name of the element, for debugging

		std::cout << "element name  " << elem_name << "  num elements = " << num_elems << "  num properties =  " << nprops << std::endl;

	    // if we're on vertex elements, read in the properties

    	if (equal_strings ("vertex", elem_name)) {
	      	// set up for getting vertex elements
	      	// the three properties are the vertex coordinates

			ply_get_property (ply, elem_name, &vert_props[0]);
	      	ply_get_property (ply, elem_name, &vert_props[1]);
		  	ply_get_property (ply, elem_name, &vert_props[2]);

		  	// reserve mesh elements

		  	mesh->num_vertices = num_elems;
		  	mesh->vertices.reserve(num_elems);

		  	// grab all the vertex elements

		  	for (j = 0; j < num_elems; j++) {
				Vertex* vertex_ptr = new Vertex;

		        // grab an element from the file

				ply_get_element (ply, (void *) vertex_ptr);
		  		mesh->vertices.push_back(Point3D(vertex_ptr->x, vertex_ptr->y, vertex_ptr->z));
		  		delete vertex_ptr;
		  	}
    	}

	    // if we're on face elements, read them in

	    if (equal_strings ("face", elem_name)) {
		    // set up for getting face elements

			ply_get_property (ply, elem_name, &face_props[0]);   // only one property - a list

		  	mesh->num_triangles = num_elems;
		  	m_parts.reserve(num_elems);  // triangles will be stored in Compound::m_parts

			// the following code stores the face numbers that are shared by each vertex

		  	mesh->vertex_faces.reserve(mesh->num_vertices);
		  	std::vector<int> faceList;

		  	for (j = 0; j < mesh->num_vertices; j++)
		  		mesh->vertex_faces.push_back(faceList); // store empty lists so that we can use the [] notation below

			// grab all the face elements

			int count = 0; // the number of faces read

			for (j = 0; j < num_elems; j++) {
			    // grab an element from the file

			    Face* face_ptr = new Face;

			    ply_get_element (ply, (void *) face_ptr);

			    // construct a mesh triangle of the specified type

			    if (triangle_type == Flat) {
			    	auto triangle_ptr = std::make_shared<FlatMeshTriangle>(mesh, face_ptr->verts[0], face_ptr->verts[1], face_ptr->verts[2]);
					triangle_ptr->ComputeNormal(reverse_normal);
					m_parts.push_back(triangle_ptr);
				}

			    if (triangle_type == Smooth) {
			    	auto triangle_ptr = std::make_shared<SmoothMeshTriangle>(mesh, face_ptr->verts[0], face_ptr->verts[1], face_ptr->verts[2]);
					triangle_ptr->ComputeNormal(reverse_normal); 	// the "flat triangle" normal is used to compute the average normal at each mesh vertex
					m_parts.push_back(triangle_ptr); 				// it's quicker to do it once here, than have to do it on average 6 times in compute_mesh_normals

					// the following code stores a list of all faces that share a vertex
					// it's used for computing the average normal at each vertex in order(num_vertices) time

					mesh->vertex_faces[face_ptr->verts[0]].push_back(count);
					mesh->vertex_faces[face_ptr->verts[1]].push_back(count);
					mesh->vertex_faces[face_ptr->verts[2]].push_back(count);
					count++;
				}
			}

			if (triangle_type == Flat)
				mesh->vertex_faces.erase(mesh->vertex_faces.begin(), mesh->vertex_faces.end());
	    }

	    // print out the properties we got, for debugging

	    for (j = 0; j < nprops; j++)
	    	printf ("property %s\n", plist[j]->name);

	}  // end of for (i = 0; i < nelems; i++)


	// grab and print out the comments in the file

	comments = ply_get_comments (ply, &num_comments);

	for (i = 0; i < num_comments; i++)
	    printf ("comment = '%s'\n", comments[i]);

	// grab and print out the object information

	obj_info = ply_get_obj_info (ply, &num_obj_info);

	for (i = 0; i < num_obj_info; i++)
	    printf ("obj_info = '%s'\n", obj_info[i]);

	// close the ply file

	ply_close (ply);
}

void Grid::ComputeMeshNormals()
{

}

}