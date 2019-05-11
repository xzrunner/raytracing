#include "raytracing/texture/ImageTexture.h"
#include "raytracing/mapping/Mapping.h"
#include "raytracing/texture/Image.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

ImageTexture::ImageTexture()
	: m_image(nullptr)
	, m_mapping(nullptr)
{
}

ImageTexture::ImageTexture(const std::shared_ptr<Image>& image)
	: m_hres(image->GetHres())
	, m_vres(image->GetVres())
    , m_image(image)
	, m_mapping(nullptr)
{
}

RGBColor ImageTexture::GetColor(const ShadeRec& sr) const
{
	int row, column;
	if (m_mapping) {
		m_mapping->GetTexelCoordinates(sr.local_hit_point, m_hres, m_vres, row, column);
	} else {
		row 	= (int)(sr.v * (m_vres - 1));
		column 	= (int)(sr.u * (m_hres - 1));
	}

	return m_image->GetColor(column, row);
}

void ImageTexture::SetImage(const std::shared_ptr<Image>& image)
{
    m_image = image;

    m_hres = image->GetHres();
    m_vres = image->GetVres();
}

}