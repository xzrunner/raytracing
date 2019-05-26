#include "raytracing/noise/LatticeNoise.h"
#include "raytracing/maths/maths.h"
#include "raytracing/sampler/MultiJittered.h"
#include "raytracing/utilities/Constants.h"

#include <memory>

namespace rt
{

// The following table is by Darwyn Peachey in Ebert et al. (2003), page 70.

const unsigned char
LatticeNoise::permutation_table[kTableSize] =
						{
					        225,155,210,108,175,199,221,144,203,116, 70,213, 69,158, 33,252,
					        5, 82,173,133,222,139,174, 27,  9, 71, 90,246, 75,130, 91,191,
					        169,138,  2,151,194,235, 81,  7, 25,113,228,159,205,253,134,142,
					        248, 65,224,217, 22,121,229, 63, 89,103, 96,104,156, 17,201,129,
					        36,  8,165,110,237,117,231, 56,132,211,152, 20,181,111,239,218,
					        170,163, 51,172,157, 47, 80,212,176,250, 87, 49, 99,242,136,189,
					        162,115, 44, 43,124, 94,150, 16,141,247, 32, 10,198,223,255, 72,
					        53,131, 84, 57,220,197, 58, 50,208, 11,241, 28,  3,192, 62,202,
					        18,215,153, 24, 76, 41, 15,179, 39, 46, 55,  6,128,167, 23,188,
					        106, 34,187,140,164, 73,112,182,244,195,227, 13, 35, 77,196,185,
					        26,200,226,119, 31,123,168,125,249, 68,183,230,177,135,160,180,
					        12,  1,243,148,102,166, 38,238,251, 37,240,126, 64, 74,161, 40,
					        184,149,171,178,101, 66, 29, 59,146, 61,254,107, 42, 86,154,  4,
					        236,232,120, 21,233,209, 45, 98,193,114, 78, 19,206, 14,118,127,
					        48, 79,147, 85, 30,207,219, 54, 88,234,190,122, 95, 67,143,109,
					        137,214,145, 93, 92,100,245,  0,216,186, 60, 83,105, 97,204, 52
				    	};

LatticeNoise::LatticeNoise(void)
	: 	num_octaves(1),
		lacunarity(2.0),
		gain(0.5)
{
	init_value_table(seed_value);
	init_vector_table(seed_value);
	compute_fbm_bounds();
}


//---------------------------------------------------------------------------------------- constructor

LatticeNoise::LatticeNoise(int _num_octaves)
	: 	num_octaves(_num_octaves),
		lacunarity(2.0),
		gain(0.5)
{
	init_value_table(seed_value);
	init_vector_table(seed_value);
	compute_fbm_bounds();
}


//---------------------------------------------------------------------------------------- constructor

LatticeNoise::LatticeNoise(int _num_octaves, float _lacunarity, float _gain)
	: 	num_octaves(_num_octaves),
		lacunarity(_lacunarity),
		gain(_gain) {
	init_value_table(seed_value);
	init_vector_table(seed_value);
	compute_fbm_bounds();
}

//---------------------------------------------------------------------------------------- destructor

LatticeNoise::~LatticeNoise(void) {}


//---------------------------------------------------------------------------------------- init_value_table

// This initialises the integer lattice of PRN's.
// It's based on valueTabInit in Peachey (2003).

void
LatticeNoise::init_value_table(int seed_value)
{
    set_rand_seed(seed_value);
    for (int i = 0; i < kTableSize; i++)
    	value_table[i] = 1.0 - 2.0 * rand_float();   // In the range [-1, +1]
}


//---------------------------------------------------------------------------------------- init_vector_table

// init_vector_table initialises the integer lattice with pseudo-random unit vectors.
// It uses multi-jittered points on a unit sphere with a p = 0 distribution, but currently
// the multi-jittered class doesn't seed randf when it generates the samples on the unit square.

// I couldn't get the same water ripples in repeated runs in the textures demo scene when
// I used MultiJittered samples, not even when I seeded rand in MultiJittered::generate_samples.

// The problem was the setup_shuffled_indices function called in all Sampler constructors.
// This calls the standard algorithm random_shuffle, which in turn calls long_random,
// which isn't affected by the seed.
// The solution is to call a new Sampler function sample_one_set, which doesn't use the shuffled indices.
// This works fine.
// Other sampling techniques can be used in place of multi-jittered.

// The formulae for mapping points from a unit square a unit sphere are in Pharr and Humphreys pp 651-652.


void
LatticeNoise::init_vector_table(int seed_value) {
	float r1, r2;
	float x, y, z;
	float r, phi;

	set_rand_seed(seed_value);
	auto sample_ptr = std::make_shared<MultiJittered>(256, 1);  // 256 samples, 1 set

    for (int j = 0; j < kTableSize; j++)
    {
    	Point2D sample_point = sample_ptr->SampleOneSet();  // on the unit square
    	r1 	= sample_point.x;
    	r2 	= sample_point.y;
    	z 	= 1.0 - 2.0 * r1;
    	r 	= sqrt(1.0 - z * z);
    	phi = TWO_PI * r2;
    	x 	= r * cos(phi);
    	y 	= r * sin(phi);
    	vector_table[j] = Vector3D(x, y, z).Normalized();   // random unit vector
    }
}


//---------------------------------------------------------------------------------------- set_num_octaves
// we must re-compute the fbm bounds

void
LatticeNoise::set_num_octaves (int _num_octaves) {
	num_octaves = _num_octaves;
	compute_fbm_bounds();
}


//---------------------------------------------------------------------------------------- set_lacunarity

// we do not have to re-compute the fbm bounds because the lacunarity doesn't affect them

void
LatticeNoise::set_lacunarity(float _lacunarity) {
	lacunarity = _lacunarity;
}


//---------------------------------------------------------------------------------------- set_gain
// we must re-compute the fbm bounds

void
LatticeNoise::set_gain(float _gain) {
	gain = _gain;
	compute_fbm_bounds();
}


//---------------------------------------------------------------------------------------- compute_fbm_bounds

void
LatticeNoise::compute_fbm_bounds(void) {
	if (gain == 1.0)
		fbm_max = num_octaves;
	else
		fbm_max = (1.0 - pow(gain, num_octaves)) / (1.0 - gain);

	fbm_min = -fbm_max;
}



//---------------------------------------------------------------------------------------- value_fractal_sum

// This function implements the scalar fractal sum function
// This has gain = 0.5, and lacunarity = 2.0 for self-similarity

float
LatticeNoise::value_fractal_sum(const Point3D& p) const {
	float 	amplitude 	= 1.0;
	float	frequency 	= 1.0;
	float 	fractal_sum	= 0.0;

	for (int j = 0; j < num_octaves; j++) {
		fractal_sum += amplitude * value_noise(frequency * p);
		amplitude 	*= 0.5;
		frequency 	*= 2.0;
	}

	fractal_sum = (fractal_sum - fbm_min) / (fbm_max - fbm_min);  // map to [0, 1]

	return (fractal_sum);
}


//---------------------------------------------------------------------------------------- vector_fractal_sum

Vector3D
LatticeNoise::vector_fractal_sum(const Point3D& p) const {
	float 		amplitude 	= 1.0;
	float		frequency 	= 1.0;
	Vector3D	sum(0, 0, 0);

	for (int j = 0; j < num_octaves; j++)
    {
		sum = sum + amplitude * vector_noise(frequency * p);
		amplitude 	*= 0.5;
		frequency 	*= 2.0;
	}

	return (sum);
}


//---------------------------------------------------------------------------------------- value_turbulence
// min turbulence = 0.0, max turbulence = fbm_max

float
LatticeNoise::value_turbulence(const Point3D& p) const {
	float 	amplitude 	= 1.0;
	float	frequency 	= 1.0;
	float 	turbulence	= 0.0;

	for (int j = 0 ; j < num_octaves; j++) {
		turbulence	+= amplitude * fabs(value_noise(frequency * p));
//		turbulence	+= amplitude * sqrt(fabs(value_noise(frequency * p)));  // for the thin lines in Figure 30.6 (c) & (d)
		amplitude 	*= 0.5;
		frequency 	*= 2.0;
	}

	turbulence /= fbm_max;  // map to [0, 1]

	return (turbulence);
}


//---------------------------------------------------------------------------------------- value_fbm
// this returns a value in the range [0, 1]

float
LatticeNoise::value_fbm(const Point3D& p) const {
	float 	amplitude 	= 1.0;
	float	frequency 	= 1.0;
	float 	fbm		 	= 0.0;

	for (int j = 0; j < num_octaves; j++) {
		fbm 		+= amplitude * value_noise(frequency * p);
		amplitude 	*= gain;
		frequency 	*= lacunarity;
	}

	fbm = (fbm - fbm_min) / (fbm_max - fbm_min);  // map to [0, 1]

	return (fbm);
}


//---------------------------------------------------------------------------------------- vector_fbm

Vector3D
LatticeNoise::vector_fbm(const Point3D& p) const {
	float 		amplitude 	= 1.0;
	float		frequency 	= 1.0;
	Vector3D	sum(0, 0, 0);

	for (int j = 0; j < num_octaves; j++)
    {
		sum = sum + amplitude * vector_noise(frequency * p);
		amplitude 	*= gain;
		frequency 	*= lacunarity;
	}

	return (sum);
}

}