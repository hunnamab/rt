#include "kernel.h"

/* t_color reflection_color(__global float3 *ray_buf, \
						__global float3 *normal_buf, \
						__global t_material *material_buf, \
						__global t_light *light, \
						__global float3 *intersection_buf, \
						__global t_object_d *obj, \
						int i, int light_nmb)
{
	if (i == 20400)
	{
		printf("success yaay it's reflection_color func\n");
		printf("(reflection_color) light_nmb = %d\n\tlight_intensity = %f\n\tlight_type = %d\n", light_nmb, light[1].intensity, light[1].type);
		printf("(reflection_color) obj type = %d\n", obj[0].type);
	}
	t_color col;
	col.red = 0;
	col.green = 0;
	col.blue = 0;
	col.alpha = 0;
	return (col);
} */
int			in_shadow(int index, float3 l)
{
	return(0);
}

float3		scale(float3 v, float t)
{
	v.x += t;
	v.y += t;
	v.z += t;
	return(v);
}

float		get_specular(int index, int j, float3 l, __global float3 *normal_buf, __global t_light *light,\
 						__global t_material *material_buf, __global float3 *ray_buf)
{
	float		nri[3];
 	float3		r;
	float3		d;
	float3		lb;

	lb = l / length(l);
	nri[2] = 0;
	nri[0] = dot(normal_buf[index], lb);
	r = normal_buf[index] * 2.0f;
	r = r * nri[0];
	r = r - lb;
	d.x = -ray_buf[index].x;
	d.y = -ray_buf[index].y;
	d.z = -ray_buf[index].z;
	nri[1] = dot(r, d);
	if (nri[1] > 0)
		nri[2] += light[j].intensity * pow((float)nri[1] / \
		(length(r) * length(d)), \
		material_buf[index].specular);
	return (nri[2]);
}

float3		get_light_vec(int index, int j, __global float3 *intersection_buf, __global t_light *light)
{
	float3 light_vec;

	light_vec = 0;
	if (light[j].type == POINT)
		light_vec = light[j].position - intersection_buf[index];
	if (light[j].type == DIRECTIONAL)
		light_vec = light[j].direction;
	return (light_vec);
}

t_color		reflection_color(__global float3 *ray_buf, \
                            __global float3 *intersection_buf, \
                            __global float3 *normal_buf, \
                            __global int *index_buf, \
                            __global t_material *material_buf, \
                            __global t_object_d *obj, \
                            __global t_light *light, \
							int light_nmb, \
							int index)
{
	float		i;
	float3		l;
	float		n_dot_l;
	int			j;

	j = -1;
	i = 0;
	while (++j < light_nmb)
	{
		if (light[j].type == AMBIENT)
			i += light[j].intensity;
		else
		{
			l = get_light_vec(index, j, intersection_buf, light);
			n_dot_l = dot(normal_buf[index], l);
			if (!(in_shadow(index, l)) && n_dot_l > 0)
			{
				if (material_buf[index].specular != -1)
					i += get_specular(index, j, l, normal_buf, light, material_buf, ray_buf);
				i += (light[j].intensity * n_dot_l) / length(l);
			}
		}
	}
	i = i > 1 ? 1 : i;
	t_color result;
	result.red = material_buf[index].color.red * i;
	result.blue = material_buf[index].color.blue * i;
	result.green = material_buf[index].color.green * i;
	if (index == 1280 * 360 + 640 * 11)
		printf("result color device (%hhu, %hhu, %hhu)\n", result.red, result.green, result.blue);
	return (result);
}
__kernel void get_frame_buf_cl(__global t_color *frame_buf, \
                            __global float3 *ray_buf, \
                            __global float3 *intersection_buf, \
                            __global float3 *normal_buf, \
                            __global int *index_buf, \
                            __global t_material *material_buf, \
                            __global t_object_d *obj, \
                            __global t_light *light, \
                            int light_nmb)
{
    int i = get_global_id(0);
	int j = index_buf[i];
	if (j != -1)
	{
		frame_buf[i] = reflection_color(ray_buf, intersection_buf, normal_buf, index_buf, material_buf, obj, light, light_nmb, i);
	}
	else
	{
		frame_buf[i].red = 0;
		frame_buf[i].green = 0;
		frame_buf[i].blue = 0;
		frame_buf[i].alpha = 255;
	}
}
