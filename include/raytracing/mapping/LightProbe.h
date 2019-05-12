#pragma once

#include "raytracing/mapping/Mapping.h"

namespace rt
{

class LightProbe: public Mapping
{
public:
	virtual void GetTexelCoordinates(const Point3D& hit_point,
        int xres, int yres, int& row, int& column) const override;

    enum class MapType
    {
        Regular,
        Panoramic
    };

    void set_map_type(MapType t) {
        m_map_type = t;
    }

private:
    MapType m_map_type = MapType::Regular;

}; // LightProbe

}
