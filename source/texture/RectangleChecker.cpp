#include "raytracing/texture/RectangleChecker.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

RGBColor RectangleChecker::GetColor(const ShadeRec& sr) const
{
	double x = sr.local_hit_point.x;
	double y = sr.local_hit_point.y;
	double z = sr.local_hit_point.z;

	double x_size = m_b.x / m_num_x_checkers;   	// in radians - azimuth angle
	double z_size = m_a.z / m_num_z_checkers;

	int i_phi 	= static_cast<int>(floor(x / x_size));
	int i_theta = static_cast<int>(floor(z / z_size));

	double f_phi 	= x / x_size - i_phi;
	double f_theta 	= z / z_size - i_theta;

	double phi_line_width = 0.5 * m_x_line_width;
	double theta_line_width = 0.5 * m_z_line_width;

	bool in_outline = (	f_phi < phi_line_width || f_phi > 1.0 - phi_line_width) ||
						(f_theta < theta_line_width || f_theta > 1.0 - theta_line_width);

	if ((i_phi + i_theta) % 2 == 0) {
		if (!in_outline)
			return (m_color1);
	}
	else {
		if (!in_outline)
			return (m_color2);
	}
	return (m_line_color);
}

}