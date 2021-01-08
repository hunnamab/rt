extern "C"{
    #include "rt_host.h"
    }
    #include "rt_device.cuh"

    __device__ void get_normal_sphere(t_object_d *obj, float3 *ray_buf, int *index_buf, float3 *normal_buf, float3 *intersection_buf, int index)
    {
        float l;
        int j;
    
        j = index_buf[index];
        normal_buf[index] = sub(intersection_buf[index], obj[j].sphere.center);
        l = length(normal_buf[index]);
        normal_buf[index] = div_by_scalar(normal_buf[index], l);
        if (dot(ray_buf[index], normal_buf[index]) > 0.0001)
            normal_buf[index] = scale(normal_buf[index], -1);
    }
    __device__ void get_normal_ellipsoid(t_object_d *obj, float3 *ray_buf, int *index_buf, float3 *normal_buf, float3 *intersection_buf, int index)
    {
        float l;
        int j;
    
        j = index_buf[index];
        normal_buf[index] = sub(intersection_buf[index], obj[j].ellipsoid.center);
        l = length(normal_buf[index]);
        normal_buf[index] = div_by_scalar(normal_buf[index], l);
        if (dot(ray_buf[index], normal_buf[index]) > 0.0001)
            normal_buf[index] = scale(normal_buf[index], -1);
    }
    __device__ void get_normal_plane(t_object_d *obj, float3 *ray_buf, int *index_buf, float3 *normal_buf, float3 *intersection_buf, int index)
    {
        int j;
    
        j = index_buf[index];
        normal_buf[index] = obj[j].plane.normal;
        if (dot(ray_buf[index], normal_buf[index]) > 0.0001)
            normal_buf[index] = scale(normal_buf[index], -1);
    }

    __device__ void get_normal_triangle(t_object_d *obj, float3 *ray_buf, int *index_buf, float3 *normal_buf, float3 *intersection_buf, int index)
    {
        int j;
    
        j = index_buf[index];
        normal_buf[index] = obj[j].triangle.normal;
        if (dot(ray_buf[index], normal_buf[index]) > 0.0001)
            normal_buf[index] = scale(normal_buf[index], -1);
    }

    __device__ void get_normal_cone(t_object_d *obj, float3 *ray_buf, int *index_buf, float3 *normal_buf, float3 *intersection_buf, int index, float3 camera_position, float *depth_buf)
    {
        float	m;
        float3 buf;
        int j;
    
        j = index_buf[index];
        buf = sub(camera_position, obj[j].cone.position);
        m = dot(ray_buf[index],  obj[j].cone.vec) * \
                        depth_buf[index] + dot(buf, obj[j].cone.vec);
        buf = scale(obj[j].cone.vec, m);
        normal_buf[index] = scale(buf, (1 +  obj[j].cone.angle *  obj[j].cone.angle));
        buf = sub(intersection_buf[index], obj[j].cone.position);
        normal_buf[index] = sub(buf, normal_buf[index]);
        normal_buf[index] = div_by_scalar(normal_buf[index], \
                                    length(normal_buf[index]));
        if (dot(ray_buf[index], normal_buf[index]) > 0.0001)
            normal_buf[index] = scale(normal_buf[index], -1);
    }

    __device__ void get_normal_cylinder(t_object_d *obj, float3 *ray_buf, int *index_buf, float3 *normal_buf, float3 *intersection_buf, int index, float3 camera_position, float *depth_buf)
    {
        float		m;
        float3		p;
        float3		buf[2];
        int j;
    
        j = index_buf[index];
        buf[0] = sub(camera_position, obj[j].cylinder.position);
        m = dot(ray_buf[index], obj[j].cylinder.vec) * \
            depth_buf[index] + dot(buf[0], obj[j].cylinder.vec);
        buf[0] = scale(ray_buf[index], depth_buf[index]);
        p = add(camera_position, buf[0]);
        buf[0] = sub(p, obj[j].cylinder.position);
        buf[1] = scale(obj[j].cylinder.vec, m);
        normal_buf[index] = sub(buf[0], buf[1]);
        normal_buf[index] = div_by_scalar(normal_buf[index], \
                                    length(normal_buf[index]));
        if (dot(ray_buf[index], normal_buf[index]) > 0.0001)
            normal_buf[index] = scale(normal_buf[index], -1);
    }

    __global__ void	get_normal_buf_device(t_object_d *obj, float3 *ray_buf, int *index_buf, float3 *normal_buf, float3 *intersection_buf, float *depth_buf, float3 camera_position)
    {
        int i = blockDim.x * blockIdx.x + threadIdx.x;
        int j = index_buf[i];
    
        if(j != -1)
        {
            if (obj[j].type == SPHERE)
                get_normal_sphere(obj, ray_buf, index_buf, normal_buf,intersection_buf, i);
            else if (obj[j].type == CONE)
                get_normal_cone(obj, ray_buf, index_buf, normal_buf,intersection_buf, i, camera_position, depth_buf);
            else if (obj[j].type == CYLINDER)
                get_normal_cylinder(obj, ray_buf, index_buf, normal_buf,intersection_buf, i, camera_position, depth_buf);
            else if (obj[j].type == PLANE)
                get_normal_plane(obj, ray_buf, index_buf, normal_buf,intersection_buf, i);
            else if (obj[j].type == TRIANGLE)
                get_normal_triangle(obj, ray_buf, index_buf, normal_buf,intersection_buf, i);
            else if (obj[j].type == ELLIPSOID)
                get_normal_ellipsoid(obj, ray_buf, index_buf, normal_buf,intersection_buf, i);
        }
    }