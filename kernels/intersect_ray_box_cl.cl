#include "kernel.h"

int cut(float3 point, __global t_cutting_surface *cs, int cs_nmb)
{
    int i;
    float result;
    i = 0;
    while(i < cs_nmb)
    {
        if (cs[i].type == PLANE)
        {
            result = cs[i].param1.x * point.x + cs[i].param1.y * point.y + cs[i].param1.z * point.z + cs[i].param3;
            if (result >= 0)
                return (0);
        }
		if(cs[i].type == SPHERE)
		{
			float3 buf;
			buf = point - cs[i].param1;
			result = length(buf);
			if (result >= cs[i].param3 && !cs[i].is_negative)
				return(0);
			if(result <= cs[i].param3 && cs[i].is_negative)
				return(0);
		}
        i++;
    }
    return (1);
}

float box_intersection(__global t_object_d *box, float3 ray_start, float3 ray_dir)
{
	float3 	t_min;
	float3 	t_max;
	float		a;
	float		b;
	float		c;
	float		t0;
	float		t1;
	int		face_in;
	int		face_out;
	float		tmin;

	a = 1.0f / ray_dir.x;
	if (a >= 0)
	{
		t_min.x = (box[0].primitive.box.a.x - ray_start.x) * a;
		t_max.x = (box[0].primitive.box.b.x - ray_start.x) * a;
	}
	else
	{
		t_min.x = (box[0].primitive.box.b.x - ray_start.x) * a;
		t_max.x = (box[0].primitive.box.a.x - ray_start.x) * a;
	}
	b = 1.0f / ray_dir.y;
	if (b >= 0)
	{
		t_min.y = (box[0].primitive.box.a.y - ray_start.y) * b;
		t_max.y = (box[0].primitive.box.b.y - ray_start.y) * b;
	}
	else
	{
		t_min.y = (box[0].primitive.box.b.y - ray_start.y) * b;
		t_max.y = (box[0].primitive.box.a.y - ray_start.y) * b;
	}
	c = 1.0f / ray_dir.z;
	if (c >= 0)
	{
		t_min.z = (box[0].primitive.box.a.z - ray_start.z) * c;
		t_max.z = (box[0].primitive.box.b.z - ray_start.z) * c;
	}
	else
	{
		t_min.z = (box[0].primitive.box.b.z - ray_start.z) * c;
		t_max.z = (box[0].primitive.box.a.z - ray_start.z) * c;
	}
	//find largest entering t value
	if (t_min.x > t_min.y)
	{
		t0 = t_min.x;
		face_in = (a >= 0.0f) ? 0 : 3;
	}
	else
	{
		t0 = t_min.y;
		face_in = (b >= 0.0f) ? 1 : 4;
	}
	if (t_min.z > t0)
	{
		t0 = t_min.z;
		face_in = (c >= 0.0f) ? 2 : 5;
	}
	//find smallest exiting t value
	if (t_max.x < t_max.y)
	{
		t1 = t_max.x;
		face_out = (a >= 0.0f) ? 3 : 0;
	}
	else
	{
		t1 = t_max.y;
		face_out = (b >= 0.0f) ? 4 : 1;
	}
	if (t_max.z < t1)
	{
		t1 = t_max.z;
		face_out = (c >= 0.0f) ? 5 : 2;
	}
	if (t0 < t1 && t1 > 0.1f)
	{
		if (t0 > 0.1f)
		{
			tmin = t0;
			box[0].primitive.box.face_hit = face_in;
		}
		else
		{
			tmin =  t1;
			box[0].primitive.box.face_hit = face_out;
		}
		return (tmin);
	}
	return (0);
}

__kernel void intersect_ray_box(__global float3 *ray_arr, \
								__global float3 *camera_start, \
								__global t_object_d *obj, __global float *depth_buf, \
								__global int *index_buf, int index, \
								int bounce_cnt, __global t_cutting_surface *cs, \
								int cs_nmb, __global t_material *material_buf)
{
    int i = get_global_id(0);
    float result;
	float3 ray;
	ray = camera_start[i] + ray_arr[i] * 0.00001f;
	if (bounce_cnt == 0 || material_buf[i].reflection > 0.0)
		result = box_intersection(&obj[index], ray, ray_arr[i]);
	else
		return ;
    if (result > 0.01 && result < depth_buf[i])
    {
		float3 intersection_point;
        intersection_point = ray_arr[i] * result;
        intersection_point = intersection_point + camera_start[i];
        if (cut(intersection_point, cs, cs_nmb))
        {
			depth_buf[i] = result;
			index_buf[i] = index;
		}
    }
}
