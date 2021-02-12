#include "kernel.h"

__kernel void    get_fresnel_coeff_cl(__global float3 *ray_buf, \
                                    __global float3 *normal_buf, \
                                    __global t_material *material_buf)
{
    int i = get_global_id(0);
}
