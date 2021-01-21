#include "kernel.h"

float ellipsoid_intersection(t_ellipsoid el, float3 ray_start, float3 ray_dir)
{
    float k1;
    float k2;
    float k3;
    float3 dir;
    float a2;
    float b2;
    float c2;

    a2 = el.abc.x * el.abc.x;
    b2 = el.abc.y * el.abc.y;
    c2 = el.abc.z * el.abc.z;
    dir = ray_dir;
    k1 = dir.x * dir.x * b2 * c2;
    k1 += dir.y * dir.y * a2 * c2; 
    k1 += dir.z * dir.z * a2 * b2;
    k2 = ray_start.x * dir.x * b2 * c2 * 2;
    k2 += ray_start.y * dir.y * a2 * c2 * 2;
    k2 += ray_start.z * dir.z * b2 * a2 * 2;
    k3 = ray_start.x * ray_start.x * b2 * c2;
    k3 += ray_start.z * ray_start.z * a2 * b2;
    k3 += ray_start.y * ray_start.y * a2 * c2;
    k3 -= a2 * b2 * c2;
    float d = k2 * k2 - 4 * k1 * k3;
    if (d >= 0)
    {
        float t1 = (-k2 + sqrt(d)) / (2 * k1);
        float t2 = (-k2 - sqrt(d)) / (2 * k1);
        if ((t1 < t2 && t1 > 0) || (t2 < 0 && t1 >= 0))
            return (t1);
        if ((t2 < t1 && t2 > 0) || (t1 < 0 && t2 >= 0))
            return (t2);
        if (t2 == t1 && t2 >= 0)
            return (t2);
    }
    return (0);
}

__kernel  void    intersect_ray_ellipsoid(__global float3 *ray_arr, float3 camera_start, t_ellipsoid el, __global float *depth_buf, __global int *index_buf, int index)
{
    int i = get_global_id(0);
    float result;
    result = ellipsoid_intersection(el, camera_start, ray_arr[i]);
    /* float k1;
    float k2;
    float k3;
    float3 dir;
    float a2;
    float b2;
    float c2;

    a2 = el.abc.x * el.abc.x;
    b2 = el.abc.y * el.abc.y;
    c2 = el.abc.z * el.abc.z;
    dir = ray_arr[i];
    k1 = dir.x * dir.x * b2 * c2;
    k1 += dir.y * dir.y * a2 * c2; 
    k1 += dir.z * dir.z * a2 * b2;
    k2 = camera_start.x * dir.x * b2 * c2 * 2;
    k2 += camera_start.y * dir.y * a2 * c2 * 2;
    k2 += camera_start.z * dir.z * b2 * a2 * 2;
    k3 = camera_start.x * camera_start.x * b2 * c2;
    k3 += camera_start.z * camera_start.z * a2 * b2;
    k3 += camera_start.y * camera_start.y * a2 * c2;
    k3 -= a2 * b2 * c2;
    float d = k2 * k2 - 4 * k1 * k3;
    if (d >= 0)
    {
        float t1 = (-k2 + sqrt(d)) / (2 * k1);
        float t2 = (-k2 - sqrt(d)) / (2 * k1);
        float result = 0;
        if ((t1 < t2 && t1 > 0) || (t2 < 0 && t1 >= 0))
            result = t1;
        if ((t2 < t1 && t2 > 0) || (t1 < 0 && t2 >= 0))
            result = t2;
        if (t2 == t1 && t2 >= 0)
            result = t2;
        if (result > 0 && result < depth_buf[i])
        {
            depth_buf[i] = result;
            index_buf[i] = index;
        }
    } */
    if (result > 0 && result < depth_buf[i])
    {
        depth_buf[i] = result;
        index_buf[i] = index;
    }
}