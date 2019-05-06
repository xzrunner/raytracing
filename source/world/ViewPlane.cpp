#include "raytracing/world/ViewPlane.h"
#include "raytracing/samplers/Regular.h"
#include "raytracing/samplers/MultiJittered.h"
#include "raytracing/samplers/Jittered.h"

#include <stdio.h>

namespace rt
{

ViewPlane::ViewPlane()
	: m_hori_res(400)
	, m_vert_res(400)
	, m_pixel_size(1)
	, m_num_samples(1)
	, m_max_depth(0)
	, m_sampler(nullptr)
	, m_show_out_of_gamut(false)
	, m_gamma(1)
	, m_inv_gamma(1)
{
}

ViewPlane::ViewPlane(const ViewPlane& vp)
	: m_hori_res(vp.m_hori_res)
	, m_vert_res(vp.m_vert_res)
	, m_pixel_size(vp.m_pixel_size)
	, m_num_samples(vp.m_num_samples)
	, m_max_depth(vp.m_max_depth)
	, m_sampler(nullptr)
	, m_show_out_of_gamut(vp.m_show_out_of_gamut)
	, m_gamma(vp.m_gamma)
	, m_inv_gamma(vp.m_inv_gamma)
{
	SetSampler(vp.m_sampler);
}

void ViewPlane::SetSampler(const std::shared_ptr<Sampler>& sampler)
{
    m_sampler = sampler;
	m_num_samples = sampler->GetNumSamples();
}

void ViewPlane::SetSamples(int n)
{
	m_num_samples = n;
  	if (m_num_samples > 1) {
  		m_sampler = std::make_shared<MultiJittered>(m_num_samples);
  	} else {
  		m_sampler = std::make_shared<Regular>(1);
  	}
}

}