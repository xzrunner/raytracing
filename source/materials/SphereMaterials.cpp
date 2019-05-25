#include "raytracing/materials/SphereMaterials.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

SphereMaterials::SphereMaterials()
    : color1(1.0f, 1.0f, 1.0f)
    , color2(0.5f, 0.5f, 0.5f)
    , line_color(0, 0, 0)
{
}

RGBColor SphereMaterials::GetLe(const ShadeRec& sr) const
{
    return BLACK;
}

RGBColor SphereMaterials::Shade(const ShadeRec& sr) const
{
	double x = sr.local_hit_point.x;
	double y = sr.local_hit_point.y;
	double z = sr.local_hit_point.z;

	double theta = acos(y);
	double phi   = atan2(x, z);
	if (phi < 0.0)
		phi += 2.0 * PI;

	double phi_size = 2 * PI / num_horizontal_checkers;   	// in radians - azimuth angle
	double theta_size = PI / num_vertical_checkers;   		// in radians - polar angle

	int i_phi 	= static_cast<int>(floor(phi / phi_size));
	int i_theta = static_cast<int>(floor(theta / theta_size));

	double f_phi 	= phi / phi_size - i_phi;
	double f_theta 	= theta / theta_size - i_theta;

	double phi_line_width = 0.5 * vertical_line_width;
	double theta_line_width = 0.5 * horizontal_line_width;

	bool in_outline = (	f_phi < phi_line_width || f_phi > 1.0 - phi_line_width) ||
						(f_theta < theta_line_width || f_theta > 1.0 - theta_line_width);

	ShadeRec newsr = sr;

	if ((i_phi + i_theta) % 2 == 0) {
		if (!in_outline)
			return (checker1_m->Shade(newsr)/*color1*/);
	}
	else {
		if (!in_outline)
			return (checker2_m->Shade(newsr)/*color2*/);
	}

	return (line_m->Shade(newsr)/*line_color*/);
}

RGBColor SphereMaterials::AreaLightShade(const ShadeRec& sr) const
{
	double x = sr.local_hit_point.x;
	double y = sr.local_hit_point.y;
	double z = sr.local_hit_point.z;

	double theta = acos(y);
	double phi   = atan2(x, z);
	if (phi < 0.0)
		phi += 2.0 * PI;

	double phi_size = 2 * PI / num_horizontal_checkers;   	// in radians - azimuth angle
	double theta_size = PI / num_vertical_checkers;   		// in radians - polar angle

	int i_phi 	= static_cast<int>(floor(phi / phi_size));
	int i_theta = static_cast<int>(floor(theta / theta_size));

	double f_phi 	= phi / phi_size - i_phi;
	double f_theta 	= theta / theta_size - i_theta;

	double phi_line_width = 0.5 * vertical_line_width;
	double theta_line_width = 0.5 * horizontal_line_width;

	bool in_outline = (	f_phi < phi_line_width || f_phi > 1.0 - phi_line_width) ||
						(f_theta < theta_line_width || f_theta > 1.0 - theta_line_width);

	ShadeRec newsr = sr;

	if ((i_phi + i_theta) % 2 == 0) {
		if (!in_outline)
			return (checker1_m->Shade(newsr)/*color1*/);
	}
	else {
		if (!in_outline)
			return (checker2_m->Shade(newsr)/*color2*/);
	}

	return (line_m->Shade(newsr)/*line_color*/);
}

}