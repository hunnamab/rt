#include "kernel.h"

float box_intersection(t_box box, float3 ray_start, float3 ray_dir)
{
	float3 	t_min;
	float3 	t_max;
	float		a;
	float		b;
	float		c;
	float		t0;
	float		t1;
	//int			face_in;
	//int			face_out;
	float		tmin;

	a = 1.0f / ray_dir.x;
	if (a >= 0)
	{
		t_min.x = (box.a.x - ray_start.x) * a;
		t_max.x = (box.b.x - ray_start.x) * a;
	}
	else
	{
		t_min.x = (box.b.x - ray_start.x) * a;
		t_max.x = (box.a.x - ray_start.x) * a;
	}
	b = 1.0f / ray_dir.y;
	if (b >= 0)
	{
		t_min.y = (box.a.y - ray_start.y) * b;
		t_max.y = (box.b.y - ray_start.y) * b;
	}
	else
	{
		t_min.y = (box.b.y - ray_start.y) * b;
		t_max.y = (box.a.y - ray_start.y) * b;
	}
	c = 1.0f / ray_dir.z;
	if (c >= 0)
	{
		t_min.z = (box.a.z - ray_start.z) * c;
		t_max.z = (box.b.z - ray_start.z) * c;
	}
	else
	{
		t_min.z = (box.b.z - ray_start.z) * c;
		t_max.z = (box.a.z - ray_start.z) * c;
	}
	// find largest entering t value
	if (t_min.x > t_min.y)
	{
		t0 = t_min.x;
		//face_in = (a >= 0.0) ? 0 : 3;
	}
	else
	{
		t0 = t_min.y;
		//face_in = (b >= 0.0) ? 1 : 4;
	}
	if (t_min.z > t0)
	{
		t0 = t_min.z;
		//face_in = (c >= 0.0) ? 2 : 5;
	}
	//find smallest exiting t value
	if (t_max.x < t_max.y)
	{
		t1 = t_max.x;
		//face_out = (a >= 0.0) ? 3 : 0;
	}
	else
	{
		t1 = t_max.y;
		//face_out = (b >= 0.0) ? 4 : 1;
	}
	if (t_max.z < t1)
	{
		t1 = t_max.z;
		//face_out = (c >= 0.0) ? 5 : 2;
	}
	if (t0 < t1 && t1 > 0.1f)
	{
		if (t0 > 0.1f)
		{
			tmin = t0;
			//box->face_hit = face_in;
		}
		else
		{
			tmin =  t1;
			//box->face_hit = face_out;
		}
		return (tmin);
	}
	return (0);
}

__kernel void intersect_ray_box(__global float3 *ray_arr, float3 camera_start, t_box box, __global float *depth_buf, __global int *index_buf, int index)
{
    int i = get_global_id(0);
    float result;
    result = box_intersection(box, camera_start, ray_arr[i]);
    if (result > 0 && result < depth_buf[i])
    {
        depth_buf[i] = result;
        index_buf[i] = index;
    }
}
