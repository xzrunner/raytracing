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

	// generate sample patterns in a unit square
	virtual void GenerateSamples() = 0;

	void MapSamplesToUnitDisk();

	void MapSamplesToHemisphere(const float p);

	// get next sample on unit square
	const Point2D& SampleUnitSquare() const;

	// get next sample on unit disk
	const Point2D& SampleUnitDisk() const;

	// get next sample on unit hemisphere
	const Point3D& SampleHemisphere();

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

	// shuffled samples array indices
	std::vector<int> m_shuffled_indices;

	// the current number of sample points used
	mutable unsigned long m_count;
	// random index jump
	mutable int m_jump;

}; // Sampler

}
