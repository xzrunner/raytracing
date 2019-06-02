#include "raytracing/sampler/NRooks.h"
#include "raytracing/maths/maths.h"

namespace rt
{

NRooks::NRooks(int num)
    : Sampler(num)
{
    GenerateSamples();
}

void NRooks::GenerateSamples()
{
    for (int p = 0; p < m_num_sets; p++) {
        for (int j = 0; j < m_num_samples; j++) {
            Point2D sp((j + rand_float()) / m_num_samples, (j + rand_float()) / m_num_samples);
            m_samples.push_back(sp);
        }
    }

    ShuffleXCoordinates();
    ShuffleYCoordinates();
}

}