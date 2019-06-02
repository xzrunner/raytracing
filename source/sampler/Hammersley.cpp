#include "raytracing/sampler/Hammersley.h"
#include "raytracing/maths/maths.h"

#include <math.h>

namespace rt
{

Hammersley::Hammersley(int num)
	: Sampler(num)
{
    m_num_sets = 1;
	GenerateSamples();
}

void Hammersley::GenerateSamples()
{
    for (int p = 0; p < m_num_sets; p++) {
        for (int j = 0; j < m_num_samples; j++) {
            Point2D pv((float)j / (float)m_num_samples, Phi(j));
            m_samples.push_back(pv);
        }
    }
}

double Hammersley::Phi(int j)
{
	double x = 0.0;
	double f = 0.5;

	while (j) {
		x += f * (double) (j % 2);
		j /= 2;
		f *= 0.5;
	}

	return (x);
}

}