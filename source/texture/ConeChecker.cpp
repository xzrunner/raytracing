#include "raytracing/texture/ConeChecker.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

RGBColor ConeChecker::GetColor(const ShadeRec& sr) const
{
    double x = sr.local_hit_point.x;
	double y = sr.local_hit_point.y;
	double z = sr.local_hit_point.z;

	double ph = y;
	double phi   = atan2(x, z);

	if (phi < 0.0)
		phi += 2.0 * PI;

	double phi_size = 2 * PI / m_num_horizontal_checkers;   	// in radians - azimuth angle
	double theta_size = m_h / m_num_vertical_checkers;   		// in radians - polar angle

	int i_phi 	= static_cast<int>(floor(phi / phi_size));
	int i_theta = static_cast<int>(floor(ph / theta_size));

	double f_phi 	= phi / phi_size - i_phi;
	double f_theta 	= ph / theta_size - i_theta;

	double phi_line_width = 0.5 * m_vertical_line_width;
	double theta_line_width = 0.5 * m_horizontal_line_width;

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