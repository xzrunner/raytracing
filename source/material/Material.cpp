#include "raytracing/material/Material.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

Material::Material()
{
}

Material::Material(const Material& m)
{
}

RGBColor Material::AreaLightShade(const ShadeRec& sr) const
{
	return BLACK;
}

RGBColor Material::GetLe(const ShadeRec& sr) const
{
	return BLACK;
}

}