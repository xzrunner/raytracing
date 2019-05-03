#pragma once

#include "raytracing/utilities/RGBColor.h"
#include "raytracing/utilities/Object.h"

#include <vector>

namespace rt
{

class Image : public Object
{
public:
	Image();

	void ReadPPMFile(const char* file_name);

	const RGBColor& GetColor(int x, int y) const;

	int GetHres() const { return m_hres; }
	int GetVres() const { return m_vres; }

private:
	int m_hres, m_vres;

	std::vector<RGBColor> m_pixels;

}; // Image

}
