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

float ellipsoid_intersection(t_ellipsoid el, float3 ray_start, float3 ray_dir, float *t1, float *t2)
{
 	float k1;
    float k2;
    float k3;
	float a = el.a;
	float b = el.b;
	float c = el.c;
	float3 co = ray_start - el.center;
    k1 = ray_dir.x * ray_dir.x / (a * a) + ray_dir.y * ray_dir.y / (b * b) + ray_dir.z * ray_dir.z / (c * c);
    k2 = 2 * co.x * ray_dir.x / (a * a) + 2 * co.y * ray_dir.y / (b * b) + 2 * co.z * ray_dir.z / (c * c);
    k3 = co.x * co.x / (a * a) + co.y * co.y / (b * b) + co.z * co.z / (c * c) - 1;
    float d = k2 * k2 - 4 * k1 * k3;
    if (d >= 0)
    {
        *t1 = (-k2 + sqrt(d)) / (2 * k1);
        *t2 = (-k2 - sqrt(d)) / (2 * k1);
    }
    return (0);
}

__kernel  void    intersect_ray_ellipsoid(__global float3 *ray_arr, \
                                        __global float3 *camera_start, \
                                        t_ellipsoid el, __global float *depth_buf, \
                                        __global int *index_buf, int index, \
										int bounce_cnt, __global t_cutting_surface *cs, \
										int cs_nmb, __global t_material *material_buf)
{
    int i = get_global_id(0);
    float result;
    float3 ray;
    float t1, t2;
    int cut1, cut2;
    ray = camera_start[i] + ray_arr[i] * 0.00001f;
	if (bounce_cnt == 0 || material_buf[i].reflection > 0.0)
		result = ellipsoid_intersection(el, ray, ray_arr[i], &t1, &t2);
	else
		return ;
    float3 intersection_point;
    intersection_point = ray_arr[i] * t1;
    intersection_point = intersection_point + camera_start[i];
    cut1 = cut(intersection_point, cs, cs_nmb);
    intersection_point = ray_arr[i] * t2;
    intersection_point = intersection_point + camera_start[i];
    cut2 = cut(intersection_point, cs, cs_nmb);
    if(cut1 && cut2)
    {
        if ((t1 < t2 && t1 > 0) || (t2 < 0 && t1 >= 0))
            result = t1;
        if ((t2 < t1 && t2 > 0) || (t1 < 0 && t2 >= 0))
            result = t2;
        if (t2 == t1 && t2 >= 0)
            result = t2;  
    }
    else if(cut1)
        result = t1;
    else if(cut2)
        result = t2;
    if (result > 0.01 && result < depth_buf[i])
    {
		depth_buf[i] = result;
		index_buf[i] = index;
    }
}
