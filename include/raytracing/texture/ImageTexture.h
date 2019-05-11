#pragma once

#include "raytracing/texture/Texture.h"

#include <memory>

namespace rt
{

class Image;
class Mapping;

class ImageTexture : public Texture
{
public:
	ImageTexture();
	ImageTexture(const std::shared_ptr<Image>& image);

	virtual RGBColor GetColor(const ShadeRec& sr) const;

    void SetImage(const std::shared_ptr<Image>& image);
    void SetMapping(const std::shared_ptr<Mapping>& mapping) {
        m_mapping = mapping;
    }

private:
	int 		             m_hres;		            // horizontal resolution of the image
	int			             m_vres;		            // vertical resolution of the image
    std::shared_ptr<Image>   m_image = nullptr;		    // the image
    std::shared_ptr<Mapping> m_mapping = nullptr;		// mapping technique used, if any

}; // ImageTexture

}
