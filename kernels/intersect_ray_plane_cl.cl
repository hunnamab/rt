#include "kernel.h"
float	get_sum(float4 matrix1[4], float4 matrix2[4], int i, int j)
{
	float	sum;
	int		k;

	k = 0;
	sum = 0;
	while (k < 4)
	{
		sum += matrix1[i][k] * matrix2[k][j];
		k++;
	}
	return (sum);
}

void	matr_mul_void(float4 *matrix1, float4 matrix2[4])
{
	int		i;
	int		j;
	float	buf;
	float4	m_buf;

	i = -1;
	while (i++ < 4)
		m_buf[i] = matrix1[0][i];
	i = 0;
	j = 0;
	buf = 0;
	while (i < 4)
	{
		while (j < 4)
		{
			buf += matrix2[i][j] * m_buf[j];
			j++;
		}
		matrix1[0][i] = buf;
		buf = 0;
		j = 0;
		i++;
	}
}

void    matr_mul(float4 matrix1[4], float4 matrix2[4], float4 result[4])
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (i < 4)
	{
		while (j < 4)
		{
			result[i][j] = get_sum(matrix1, matrix2, i, j);
			j++;
		}
		j = 0;
		i++;
	}
}
 
 void    rotate_x(float x, float4 matrix[4])
{
	matrix[0][0] = 1;
	matrix[0][2] = 0;
	matrix[1][1] = cos(x);
	matrix[1][2] = -sin(x);
	matrix[2][1] = sin(x);
	matrix[2][2] = cos(x);
	matrix[3][3] = 1;
}

void    rotate_z(float z, float4 matrix[4])
{
	matrix[0][0] = cos(z);
	matrix[0][1] = -sin(z);
	matrix[1][0] = sin(z);
	matrix[1][1] = cos(z);
	matrix[2][2] = 1;
	matrix[3][3] = 1;
}

void    rotate_y(float y, float4 matrix[4])
{
	matrix[0][0] = cos(y);
	matrix[0][2] = sin(y);
	matrix[1][1] = 1;
	matrix[2][0] = -sin(y);
	matrix[2][2] = cos(y);
	matrix[3][3] = 1;
}

 void	get_rotation_matrix(float3 angle, float4 *vec)
{
	float4 rotation_matrix[4];
	float4 rotation_buf[4];
	float4 rotation_buf_second[4];
    int i = 0;

    while(i < 4)
    {
        rotation_matrix[i] = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
        rotation_buf[i] = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
        rotation_buf_second[i] = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
        i++;
    }
    i++;
	rotate_z(angle.z, rotation_buf);
	rotate_y(angle.y, rotation_buf_second);
	matr_mul(rotation_buf, rotation_buf_second, rotation_matrix);
    while(i < 4)
    {
        rotation_buf[i] = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
        rotation_buf_second[i] = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
        i++;
    }
	rotate_x(angle.x, rotation_buf);
	matr_mul(rotation_matrix, rotation_buf, rotation_buf_second);
    matr_mul_void(vec, rotation_buf_second);
}

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
        if(cs[i].type == CYLINDER)
        {
            float4 buf_point;
            buf_point.x = point.x;
            buf_point.y = point.y;
            buf_point.z = point.z;
            buf_point.w = 1;
            get_rotation_matrix(cs[i].param2, &buf_point);
            result = (buf_point.x * buf_point.x - 2 * buf_point.x * cs[i].param1.x + cs[i].param1.x * cs[i].param1.x) / cs[i].param3 + \
            (buf_point.y * buf_point.y - 2 * buf_point.y * cs[i].param1.y + cs[i].param1.y * cs[i].param1.y) / cs[i].param3 - 1;
            if (result < 0 && cs[i].is_negative)
                return(0);
            if (result > 0 && !cs[i].is_negative)
                return(0);
        }
        i++;
    }
    return (1);
}

float plane_intersection(t_plane plane, float3 ray_start, float3 ray_dir)
{
	float k1;
	float k2;

    if ((dot(ray_dir, plane.normal)) == 0)
		return (0);
	k1 = dot(ray_start, plane.normal) + plane.d;
	k2 = dot(ray_dir, plane.normal);
	if (k1 == 0 || (k1 < 0 && k2 < 0) || (k1 > 0 && k2 > 0))
		return (0);
	k1 = -k1 / k2;
	return (k1);
}

__kernel void intersect_ray_plane_cl(__global float3 *ray_arr, \
                                __global float3 *camera_start, \
                                __global float *depth_buf, \
                                t_plane plane, \
                                __global int *index_buf, \
                                int index, int bounce_cnt, \
								__global t_cutting_surface *cs, \
								int cs_nmb, __global t_material *material_buf, \
								int is_refractive, __global float3 *normal_buf, \
								int refraction, __global int *exception_buf)
{
    int i = get_global_id(0);
    
	float k1;
	float3 ray;
	int cut1;
	if (exception_buf[i] == index)
	{
		exception_buf[i] = -1;
		return ;
	}
    if (bounce_cnt > 0 || is_refractive)
    	camera_start[i] = camera_start[i] + ray_arr[i] * 0.00001f;
	if ((bounce_cnt == 0 && !is_refractive) || (bounce_cnt == 0 && is_refractive && material_buf[i].refraction > 0.0f && material_buf[i].kr < 1.0f) || material_buf[i].reflection > 0.0f)
		k1 = plane_intersection(plane, camera_start[i], ray_arr[i]);
	else
		return ;
	float3 intersection_point;
    intersection_point = ray_arr[i] * k1;
    intersection_point = intersection_point + camera_start[i];
    cut1 = cut(intersection_point, cs, cs_nmb);
    if (k1 < depth_buf[i] && k1 > 0.01 && cut1)
    {
		depth_buf[i] = k1;
		index_buf[i] = index;
    }
}
