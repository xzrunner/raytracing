#include "raytracing/texture/DiskChecker.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

RGBColor DiskChecker::GetColor(const ShadeRec& sr) const
{
	double x = sr.local_hit_point.x - m_center.x;
	double y = sr.local_hit_point.y - m_center.y;
	double z = sr.local_hit_point.z - m_center.z;

	double phi   = atan2(x, z);

	double phi_size = 2 * PI / m_num_angular_checkers;   	// in radians - azimuth angle
	double theta_size = m_radius / m_num_radial_checkers;
	double ra = sqrt(x*x + z*z) / m_radius;   		// in radians - polar angle

	int i_phi 	= static_cast<int>(floor(phi / phi_size));
	int i_theta = static_cast<int>(floor(ra / theta_size));

	double f_phi 	= phi / phi_size - i_phi;
	double f_theta 	= ra / theta_size - i_theta;

	double phi_line_width = 0.5 * m_angular_line_width;
	double theta_line_width = 0.5 * m_radial_line_width;

	bool in_outline = (	f_phi < phi_line_width || f_phi > 1.0 - phi_line_width) ||
						(f_theta < theta_line_width || f_theta > 1.0 - theta_line_width);

	if ((i_phi + i_theta) % 2 == 0) {
		if (!in_outline)
			return (m_color1);
	} else {
		if (!in_outline)
			return (m_color2);
	}
	return (m_line_color);
}

}