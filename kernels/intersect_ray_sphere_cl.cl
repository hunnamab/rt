__kernel void intersect_ray_sphere_cl(__global float3 *ray_arr, \
                                __global float3 *camera_start, \
                                __global float3 *s_center, \
                                __global float *s_radius, \
                                __global float *depth_buf)
{
    int i = get_global_id(0);
    float a = dot(ray_arr[i], ray_arr[i]);
    float b;
    float c;
    float t1;
    float t2;
    float3 dist = camera_start[0] - s_center[0];
    b = 2 * dot(dist, ray_arr[i]);
    c = dot(dist, dist) - (s_radius[0] * s_radius[0]);
    c = b * b - 4 * a * c;
    if (c >= 0)
    {
        c = sqrt(c);
        t1 = (-b + c) / (2 * a);
        t2 = (-b - c) / (2 * a);
        depth_buf[i] = t1 < t2 ? t1 : t2;
    }
}
