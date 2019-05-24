#include "raytracing/materials/SV_Emissive.h"
#include "raytracing/texture/Texture.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

RGBColor SV_Emissive::GetLe(const ShadeRec& sr) const
{
	/*
	return ls * ce;		//here is added, this is very important to multiply ce with ls!!!!!!
	*/
	return m_ls * m_tex->GetColor(sr);
}

RGBColor SV_Emissive::Shade(const ShadeRec& sr) const
{
	/*
	Vector3D 	wo 			= -sr.ray.d;
	RGBColor 	L 			= //ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);
	int 		num_lights 	= sr.w.lights.size();

	for (int j = 0; j < num_lights; j++) {
		Light* light_ptr = sr.w.lights[j];
		Vector3D wi = light_ptr->get_direction(sr);	//compute_direction ?
		wi.normalize();
		float ndotwi = sr.normal * wi;
		float ndotwo = sr.normal * wo;

		if (ndotwi > 0.0 && ndotwo > 0.0) {
			bool in_shadow = false;

			if (sr.w.lights[j]->casts_shadows()) {
				Ray shadow_ray(sr.hit_point, wi);		//hitPoint ?
				in_shadow = light_ptr->in_shadow(shadow_ray, sr);	//.object ?
			}

			if (!in_shadow)
				L += diffuse_brdf->f(sr, wo, wi) * light_ptr->L(sr) * light_ptr->G(sr) * ndotwi;
		}
	}

	return (L);

	*/
	return m_ls * m_tex->GetColor(sr);
	/*
	if (-sr.normal * sr.ray.d > 0.0)		//here may be ConcaveSphere not support
		return (ls * ce);
	else
		return (black);*/
}

RGBColor SV_Emissive::AreaLightShade(const ShadeRec& sr) const
{
	return m_ls * m_tex->GetColor(sr);
	/*
	if (-sr.normal * sr.ray.d > 0.0)		//here may be ConcaveSphere not support
		return (ls * ce);
	else
		return (black);*/
}

RGBColor SV_Emissive::PathShade(ShadeRec& sr) const
{
	/*
	if (sr.depth == 1)
		return (black);
		*/
	return m_ls * m_tex->GetColor(sr);
	/*
	if (-sr.normal * sr.ray.d > 0.0)
		return (ls * ce);
	else
		return (black);*/
}

RGBColor SV_Emissive::GlobalShade(ShadeRec& sr) const
{
	if (sr.depth == 1)
		return BLACK;

	return m_ls * m_tex->GetColor(sr);
	/*
	if (-sr.normal * sr.ray.d > 0.0)
		return (ls * ce);
	else
		return (black);*/
}

}