#include "raytracing/sampler/PureRandom.h"
#include "raytracing/maths/maths.h"

#include <math.h>

namespace rt
{

PureRandom::PureRandom(int num)
	: Sampler(num)
{
	m_num_sets = 1;
	GenerateSamples();
}

void PureRandom::GenerateSamples()
{
    int n = (int)sqrt((float)m_num_samples);
    for (int p = 0; p < m_num_samples; p++) {
        for (int q = 0; q < n; q++) {
            m_samples.push_back(Point2D(rand_float(), rand_float()));
        }
    }
}

}