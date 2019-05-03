#pragma once

namespace rt
{

class Sampler;

class ViewPlane
{
public:
	ViewPlane();
	ViewPlane(const ViewPlane& vp);
	~ViewPlane();

	int GetWidth() const { return m_hori_res; }
	int GetHeight() const { return m_vert_res; }
	void SetWidth(int w) { m_hori_res = w; }
	void SetHeight(int h) { m_vert_res = h; }

	float GetPixelSize() const { return m_pixel_size; }
	void SetPixelSize(float sz) { m_pixel_size = sz; }

	int GetSamplesNum() const { return m_num_samples; }
	const Sampler* GetSampler() const { return m_sampler; }
	void SetSampler(Sampler* sampler);
	void SetSamples(int n);

	bool ShowOutOfGamut() const { return m_show_out_of_gamut; }

	float GetGamma() const { return m_gamma; }
	float GetInvGamma() const { return m_inv_gamma; }

	void SetMaxDepth(int depth) { m_max_depth = depth; }

private:
	// resolution
	int m_hori_res, m_vert_res;

	// pixel size
	float m_pixel_size;							

	// number of samples per pixel
	int	m_num_samples;

	// max number of reflected bounce rays
	int	m_max_depth;

	Sampler* m_sampler;

	// display red if RGBColor out of gamut
	bool m_show_out_of_gamut;

	// gamma correction factor
	float m_gamma;
	// the inverse of the gamma correction factor
	float m_inv_gamma;

}; // ViewPlane

}
