__kernel    void    init_buffers(__global int *index_buf, \
                                __global float *depth_buf, \
                                __global float3 *intersection_buf, \
                                float3 camera_position, \
                                int flag, __global int *exeption_buf, int is_refractive)

{
    int i = get_global_id(0);

    index_buf[i] =  -1;
    depth_buf[i] = 10000000;
    if(flag == 1)
        intersection_buf[i] = camera_position;
    if(!is_refractive)
        exeption_buf[i] = -1;
}