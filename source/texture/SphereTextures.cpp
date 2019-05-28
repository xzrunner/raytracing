#include "raytracing/texture/SphereTextures.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

RGBColor SphereTextures::GetColor(const ShadeRec& sr) const
{
	double x = sr.local_hit_point.x;
	double y = sr.local_hit_point.y;
	double z = sr.local_hit_point.z;

	double theta = acos(y);
	double phi   = atan2(x, z);
	if (phi < 0.0)
		phi += 2.0 * PI;

	double phi_size = 2 * PI / m_num_horizontal_checkers;   	// in radians - azimuth angle
	double theta_size = PI / m_num_vertical_checkers;   		// in radians - polar angle

	int i_phi 	= static_cast<int>(floor(phi / phi_size));
	int i_theta = static_cast<int>(floor(theta / theta_size));

	double f_phi 	= phi / phi_size - i_phi;
	double f_theta 	= theta / theta_size - i_theta;


	if ((i_phi + i_theta) % 2 == 0)
	{
		return (m_texture1->GetColor(sr));
	}
	else
	{
		return (m_texture2->GetColor(sr));
	}
}

void SphereTextures::SetTexture1(std::unique_ptr<Texture>& tex)
{
    m_texture1 = std::move(tex);
}

void SphereTextures::SetTexture2(std::unique_ptr<Texture>& tex)
{
    m_texture2 = std::move(tex);
}

}