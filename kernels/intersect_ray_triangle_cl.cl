__kernel void intersect_ray_triangle_cl(__global float3 *ray_arr, \
                                __global float3 *camera_start, \
                                __global float *depth_buf, \
                                __global float3 *vertex)
{
    int i = get_global_id(0);
    
    float3 edge[2];
    float3 vec[3];
    float det;
    float uv[2];
    float3 ver[3];

    ver[0] = vertex[0];
    ver[1] = vertex[1];
    ver[2] = vertex[2];
    edge[0] = ver[1] - ver[0];
	edge[1] = ver[2] - ver[0];
	vec[0] = cross(ray_arr[i], edge[1]);
	det = dot(edge[0], vec[0]);

	if (det < 1e-8 && det > -1e-8)
		exit (1);
	det = 1 / det;
	vec[1] = camera_start - ver[0]);
	uv[0] = dot(vec[1], vec[0]) * det;
	if (uv[0] < 0 || uv[0] > 1)
		exit (1);
	vec[2] = cross(vec[1], edge[0]);
	uv[1] = dot(ray_arr[i], vec[2]) * det;
	if (uv[1] < 0 || uv[0] + uv[1] > 1)
		exit (1);
	if ((dot(edge[1], vec[2]) * det) > 0)
		depth_buf[i] = dot(edge[1], vec[2]) * det;
}
