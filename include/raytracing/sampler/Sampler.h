#pragma once

#include "raytracing/maths/Point2D.h"
#include "raytracing/maths/Point3D.h"

#include <vector>

namespace rt
{

class Sampler
{
public:
	Sampler();
	Sampler(int num);
    Sampler(int num, int num_sets);

	// generate sample patterns in a unit square
	virtual void GenerateSamples() = 0;

	void MapSamplesToUnitDisk();
	void MapSamplesToHemisphere(const float p);
    void MapSamplesToSphere();

	// get next sample on unit square
	const Point2D& SampleUnitSquare() const;

	// get next sample on unit disk
	const Point2D& SampleUnitDisk() const;

	// get next sample on unit hemisphere
	const Point3D& SampleHemisphere();

    // get next sample on unit sphere
    const Point3D& SampleSphere() const;

    // only used to set up a vector noise table
    // this is not discussed in the book, but see the
    // file LatticeNoise.cpp in Chapter 31
    const Point2D& SampleOneSet() const;

	int GetNumSamples() const { return m_num_samples; }

private:
	void SetupShuffledIndices();

protected:
	// the number of sample points
	int m_num_samples;
	// the number of sample sets
	int m_num_sets;

	// sample points on a unit square
	std::vector<Point2D> m_samples;

	// sample points on a unit disk
	std::vector<Point2D> m_disk_samples;

	// sample points on a unit hemisphere
	std::vector<Point3D> m_hemisphere_samples;

    // sample points on a unit sphere
    std::vector<Point3D> m_sphere_samples;

	// shuffled samples array indices
	std::vector<int> m_shuffled_indices;

	// the current number of sample points used
	mutable unsigned long m_count;
	// random index jump
	mutable int m_jump;

}; // Sampler

}
