__kernel  void    intersect_ray_ellipsoid(__global float3 *ray_arr, float3 camera_start, t_ellipsoid el, __global float *depth_buf, __global int *index_buf, int index)
{
    int i = get_global_id(0);
    float k1;
    float k2;
    float k3;
    float3 dir;
    float a2;
    float b2;
    float c2;

    a2 = a * a;
    b2 = b * b;
    c2 = c * c;
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
    }
}