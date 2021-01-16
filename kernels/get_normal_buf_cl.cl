#include "kernel.h"

float3  get_normal_cylinder(t_cylinder obj, \
                        float3 ray_buf, \
                        int index_buf, \
                        float3 intersection_buf, \
						float3 camera_position, \
						float depth_buf, int index)
{
/* 	if (index == 640)
	{
		printf("cyl pos (%f,%f,%f) vec (%f,%f,%f) %f\n", obj.position.x,obj.position.y,obj.position.z,\
		obj.vec.x,obj.vec.y,obj.vec.z, obj.radius);
		//printf("normal_buf[%d] (%f,%f,%f)\n", index, normal_buf[0].x,normal_buf[0].y,normal_buf[0].z);
	} */
	float	m;
    float3 buf1;
	float3 buf2;
	float3 p;
	float3 normal;
    buf1 = camera_position - obj.position;
    m = dot(ray_buf,  obj.vec) * depth_buf + dot(buf1, obj.vec);
    buf1.x = ray_buf.x * depth_buf;
	buf1.y = ray_buf.y * depth_buf;
	buf1.z = ray_buf.z * depth_buf;
	p = camera_position + buf1;
	buf1 = p - obj.position;
	buf2.x = obj.vec.x * m;
	buf2.y = obj.vec.y * m;
	buf2.z = obj.vec.z * m;
	normal = buf1 - buf2;
 	normal = native_divide(normal, length(normal));
	//normalize(normal);
	/* normal.x /= length(normal);
	normal.y /= length(normal);
	normal.z /= length(normal); */
    if (dot(ray_buf, normal) > 0.0001)
    {
		normal.x *= -1.0f;
		normal.y *= -1.0f;
		normal.z *= -1.0f;
	}
/* 	if (index == 640)
	{
		printf("cyl pos (%f,%f,%f) vec (%f,%f,%f) %f\n", obj.position.x,obj.position.y,obj.position.z,\
		obj.vec.x,obj.vec.y,obj.vec.z, obj.radius);
		//printf("normal_buf[%d] (%f,%f,%f)\n", index, normal_buf[0].x,normal_buf[0].y,normal_buf[0].z);
	}
	if (index == 640)
	{
		printf("normal[%d] (%f,%f,%f)\n", index, normal.x,normal.y,normal.z);
		printf("depth_buf[%d] == %f\n", index, depth_buf);
	} */
	return(normal);
}

void get_normal_cone(__global t_object_d *obj, \
                        __global float3 *ray_buf, \
                        __global int *index_buf, \
                        __global float3 *normal_buf, \
                        __global float3 *intersection_buf, \
						float3 camera_position, \
						__global float *depth_buf)
{
	float	m;
    float3 buf;
	float n;
    buf = camera_position - obj[0].primitive.cone.position;
    m = dot(ray_buf[0],  obj[0].primitive.cone.vec) * depth_buf[0] + dot(buf, obj[0].primitive.cone.vec);
    buf.x = obj[0].primitive.cone.vec.x * m;
	buf.y = obj[0].primitive.cone.vec.y * m;
	buf.z = obj[0].primitive.cone.vec.z * m;
	n = 1 +  obj[0].primitive.cone.angle * obj[0].primitive.cone.angle;
    normal_buf[0].x = buf.x * n;
	normal_buf[0].y = buf.y * n;
	normal_buf[0].z = buf.z * n;
    buf = intersection_buf[0] - obj[0].primitive.cone.position;
    normal_buf[0] = buf - normal_buf[0];
    normal_buf[0] = native_divide(normal_buf[0], length(normal_buf[0]));
    if (dot(ray_buf[0], normal_buf[0]) > 0.0001)
    {
		normal_buf[0].x = normal_buf[0].x * -1;
		normal_buf[0].y = normal_buf[0].y * -1;
		normal_buf[0].z = normal_buf[0].z * -1;
	}
}

void get_normal_sphere(__global t_object_d *obj, \
                        __global float3 *ray_buf, \
                        __global int *index_buf, \
                        __global float3 *normal_buf, \
                        __global float3 *intersection_buf)
{
	float l;

	normal_buf[0] = intersection_buf[0] - obj[0].primitive.sphere.center;
	l = length(normal_buf[0]);
	normal_buf[0] = native_divide(normal_buf[0], l);
    if (dot(ray_buf[0], normal_buf[0]) > 0.0001)
	{
		normal_buf[0].x = normal_buf[0].x * -1;
		normal_buf[0].y = normal_buf[0].y * -1;
		normal_buf[0].z = normal_buf[0].z * -1;
	}
}

void get_normal_triangle(__global t_object_d *obj, \
                        __global float3 *ray_buf, \
                        __global float3 *normal_buf)
{
	normal_buf[0] = obj[0].primitive.triangle.normal;
    if (dot(ray_buf[0], normal_buf[0]) > 0.0001)
	{
		normal_buf[0].x = normal_buf[0].x * -1;
		normal_buf[0].y = normal_buf[0].y * -1;
		normal_buf[0].z = normal_buf[0].z * -1;
	}
}

void get_normal_plane(__global t_object_d *obj, \
                        __global float3 *ray_buf, \
                        __global float3 *normal_buf)
{
	normal_buf[0] = obj[0].primitive.plane.normal;
    if (dot(ray_buf[0], normal_buf[0]) > 0.0001)
	{
		normal_buf[0].x = normal_buf[0].x * -1;
		normal_buf[0].y = normal_buf[0].y * -1;
		normal_buf[0].z = normal_buf[0].z * -1;
	}
}

__kernel void get_normal_buf_cl(__global t_object_d *obj, \
                                __global float3 *ray_buf, \
                                __global int *index_buf, \
                                __global float3 *normal_buf, \
                                __global float3 *intersection_buf, \
								__global float *depth_buf, \
								float3 camera_position)
{
    int i = get_global_id(0);
	int j = index_buf[i];
	float l;

	if (j != -1)
	{
		if (obj[j].type == SPHERE)
			get_normal_sphere(&obj[j], &ray_buf[i], &index_buf[i], &normal_buf[i], &intersection_buf[i]);
		else if (obj[j].type == PLANE)
			get_normal_plane(&obj[j], &ray_buf[i], &normal_buf[i]);
		else if (obj[j].type == TRIANGLE)
			get_normal_triangle(&obj[j], &ray_buf[i], &normal_buf[i]);
		else if (obj[j].type == CONE)
			get_normal_cone(&obj[j], &ray_buf[i], &index_buf[i], &normal_buf[i], &intersection_buf[i], camera_position, &depth_buf[i]);
		else if (obj[j].type == CYLINDER)
			normal_buf[i] = get_normal_cylinder(obj[j].primitive.cylinder, ray_buf[i], index_buf[i], intersection_buf[i], camera_position, depth_buf[i], i);
	}
}
