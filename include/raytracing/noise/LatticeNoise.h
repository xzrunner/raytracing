#pragma once

#include "raytracing/maths/Vector3D.h"

#define PERM(x)          permutation_table[(x)&kTableMask]
#define INDEX(ix,iy,iz)  PERM((ix)+PERM((iy)+PERM(iz)))
#define FLOOR(x) 		 ((int)(x) - ((x) < 0 && (x) != (int) (x)))

namespace rt
{

class LatticeNoise
{
public:
	LatticeNoise();
	LatticeNoise(int octaves);
	LatticeNoise(int octaves, float lacunarity, float gain);
	virtual ~LatticeNoise();

	// noise
	virtual float value_noise(const Point3D& p) const = 0;
	virtual Vector3D vector_noise(const Point3D& p) const = 0;

	// fractal sum
	virtual float value_fractal_sum(const Point3D& p) const;
	virtual Vector3D vector_fractal_sum(const Point3D& p) const;

	// turbulence (no vector version)
	virtual float value_turbulence(const Point3D& p) const;

	// fbm
	virtual float value_fbm(const Point3D& p) const;
	virtual Vector3D vector_fbm(const Point3D& p) const;

	// access functions
	void set_num_octaves(int octaves);
	void set_lacunarity(float lacunarity);
	void set_gain(float gain);

protected:
    static const int kTableSize = 256;
    static const int kTableMask = kTableSize - 1;
    static const int seed_value = 253;

protected:
	int 							num_octaves;
	float							lacunarity;
	float							gain;

	static const	unsigned char 	permutation_table[kTableSize];	// permutation array
					float 			value_table[kTableSize];		// array of pseudo-random numbers
					Vector3D		vector_table[kTableSize];		// array of pseudo-random unit vectors


private:

	float							fbm_min;  						// minimum value of fbm
	float							fbm_max;						// maximum value of fbm

	void															// initialise the integer lattice
	init_value_table(int seed);

	void															// initialise the integer lattice
	init_vector_table(int seed);

	void															// compute fbm_min and fbm_max
	compute_fbm_bounds(void);

}; // LatticeNoise

}