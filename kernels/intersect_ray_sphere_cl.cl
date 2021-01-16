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
            result = cs[i].plane.normal.x * point.x + cs[i].plane.normal.y * point.y + cs[i].plane.normal.z * point.z + cs[i].plane.d;
            if (result >= 0)
                return(0);
        }
        i++;
    }
    return(1);
}
__kernel void intersect_ray_sphere_cl(__global float3 *ray_arr, \
                                __global float3 *camera_start, \
                                t_sphere sphere, \
                                __global float *depth_buf, \
                                __global int *index_buf, \
                                int index, __global t_cutting_surface *cs, int cs_nmb)
{
    int i = get_global_id(0);
    float a = dot(ray_arr[i], ray_arr[i]);
    float b;
    float c;
    float t1;
    float t2;
    float3 dist = camera_start[0] - sphere.center;
    b = 2 * dot(dist, ray_arr[i]);
    c = dot(dist, dist) - (sphere.radius * sphere.radius);
    c = b * b - 4 * a * c;
    if (c >= 0)
    {
        c = sqrt(c);
        t1 = (-b + c) / (2 * a);
        t2 = (-b - c) / (2 * a);
        float result;
        result = 0;
        if ((t1 < t2 && t1 > 0) || (t2 < 0 && t1 >= 0))
            result = t1;
        if ((t2 < t1 && t2 > 0) || (t1 < 0 && t2 >= 0))
            result = t2;
        if (t2 == t1 && t2 >= 0)
            result = t2;
        if (result > 0 && result < depth_buf[i])
        {
            float3 intersection_point;
            intersection_point = ray_arr[i] * result;
            intersection_point = intersection_point + camera_start[0];
            if (cut(intersection_point, cs, cs_nmb))
            {
                depth_buf[i] = result;
                index_buf[i] = index;
            }
        }
    }
}
