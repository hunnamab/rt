__kernel    void    init_buffers(__global int *index_buf, \
                                __global float *depth_buf, \
                                __global float3 *intersection_buf)
{
    int i = get_global_id(0);

    index_buf[i] = 0;
    depth_buf[i] = 0;
    intersection_buf[i] = (float3)(0.0f,0.0f,0.0f);
}