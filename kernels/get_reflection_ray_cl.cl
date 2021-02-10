__kernel void get_reflection_ray_cl(__global float3 *ray_buf, \
                                __global float3 *normal_buf)
{
    int i = get_global_id(0);
	float buf;
	float3 buf2;

	buf = dot(ray_buf[i], normal_buf[i]);
	buf2.x = 2 * ray_buf[i].x * buf;
	buf2.y = 2 * ray_buf[i].y * buf;
	buf2.z = 2 * ray_buf[i].z * buf;
	ray_buf[i] = normal_buf[i] - buf2;
}
