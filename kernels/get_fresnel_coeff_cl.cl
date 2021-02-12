#include "kernel.h"

__kernel void    get_fresnel_coeff_cl(__global float3 *ray_buf, \
                                    __global float3 *normal_buf, \
                                    __global t_material *material_buf)
{
    int i = get_global_id(0);
    float i_dot_n;
    float etai = 1.0;
	float etat;
    float sint;
    float cost;
    float cosi;
    float r_s;
    float r_p;
    if (material_buf[i].refraction > 0.0)
    {
        i_dot_n = dot(ray_buf[i], normal_buf[i]);
        etat = material_buf[i].refraction;
        if (i_dot_n > 0.0)
        {
            etai = etat;
            etat = 1.0;
        }
        sint = etai / etat * sqrt(max(0.0, 1.0 - i_dot_n * i_dot_n));
        if (sint > 1.0)
            material_buf[i].kr = 1.0;
        else
        {
            cost = sqrt(max(0.0, 1.0 - sint * sint));
            cosi = fabs(cost);
            r_s = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
            r_p = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
            material_buf[i].kr = (r_s * r_s + r_p * r_p) / 2.0;
        }
    }
}
