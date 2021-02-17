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

float triangle_intersection(t_triangle triangle, float3 ray_start, float3 ray_dir)
{
	float3 edge1;
	float3 edge2;
    float3 vec1;
	float3 vec2;
	float3 vec3;
    float det;
    float uv1;
	float uv2;

    edge1 = triangle.vertex[1] - triangle.vertex[0];
	edge2 = triangle.vertex[2] - triangle.vertex[0];
	vec1 = cross(ray_dir, edge2);
	det = dot(edge1, vec1);
	if (det < 1e-8 && det > -1e-8)
		return (0);
	det = 1 / det;
	vec2 = ray_start - triangle.vertex[0];
	uv1 = dot(vec2, vec1) * det;
	if (uv1 < 0 || uv1 > 1)
		return (0);
	vec3 = cross(vec2, edge1);
	uv2 = dot(ray_dir, vec3) * det;
	if (uv2 < 0 || uv1 + uv2 > 1)
		return (0);
	float res;
	res = dot(edge2, vec3) * det;
	return (res);
}

__kernel void intersect_ray_triangle_cl(__global float3 *ray_arr, \
                                __global float3 *camera_start, \
                                __global float *depth_buf, \
                                t_triangle triangle, \
                                __global int *index_buf, \
                                int index, int bounce_cnt,\
								 __global t_cutting_surface *cs, \
								int cs_nmb, __global t_material *material_buf)
{
    int i = get_global_id(0);
    float res;
	float3 ray;
	int cut1;
    ray = camera_start[i] + ray_arr[i] * 0.00001f;
	if (bounce_cnt == 0 || material_buf[i].reflection > 0.0)
		res = triangle_intersection(triangle, ray, ray_arr[i]);
	else
		return ;
	float3 intersection_point;
    intersection_point = ray_arr[i] * res;
    intersection_point = intersection_point + camera_start[i];
    cut1 = cut(intersection_point, cs, cs_nmb);
	if (res > 0.01 && res < depth_buf[i] && cut1)
	{
		depth_buf[i] = res;
		index_buf[i] = index;
	}
}
