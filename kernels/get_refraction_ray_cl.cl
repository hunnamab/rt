#include "kernel.h"

float3 refract(float3 I, float3 N, float refr)
{
	float3 Nrefr = N;
	float NdotI = dot(Nrefr, I);
	float etai = 1.0;
	float etat = refr;
	float swap;
	if (NdotI < 0)
	{
		NdotI = -NdotI;
	}
	else
	{
		Nrefr = -N;
		swap = etai;
		etai = etat;
		etat = swap;
	}
	float eta = etai / etat;
	float k = 1 - (eta * eta) * (1 - NdotI * NdotI);
	if (k < 0)
		return (0);
	else
	{
		return (eta * I + (eta * NdotI - sqrt(k)) * Nrefr);
	}
	return (0);
}

__kernel void get_refraction_ray_cl(__global float3 *ray_arr, \
                                __global int *index_buf, \
                                __global float3 *normal_buf, \
								__global int *exception_buf, \
                                __global t_object_d *obj, \
								__global t_material *material_buf)
{
    int i = get_global_id(0);
	
	if (index_buf[i] != -1)
	{
		if (obj[index_buf[i]].refraction > 0.0f && material_buf[i].kr < 1.0f)
		{
			normal_buf[i] = refract(ray_arr[i], normal_buf[i], obj[index_buf[i]].refraction);
			exception_buf[i] = index_buf[i];
		}
		else
			return ;
	}
	else
		return ;
}
