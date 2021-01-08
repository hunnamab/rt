#include "rt_device.cuh"

__global__ void helloCUDA(float f)
{
    printf("Hello thread %d, f=%f\n", threadIdx.x, f);
}

__global__ void kernel_getray(float3 *viewport, float3 camera, float3 *ray_buf)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    
    ray_buf[i].x = viewport[i].x - camera.x;
    ray_buf[i].y = viewport[i].y - camera.y;
    ray_buf[i].z = viewport[i].z - camera.z; 
}

 __global__ void kernel_get_intersection_point(float3 *intersection_buf, float3 *ray_buf, float *depth_buf, float3 camera, int *index_buf)
 {
    int i = blockDim.x * blockIdx.x + threadIdx.x;

    if (index_buf[i] != -1)
    {
        intersection_buf[i] = scale(ray_buf[i], depth_buf[i]);
            intersection_buf[i] = add(\
            intersection_buf[i], camera);
    }
    else
    {
        intersection_buf[i].x = 0;
        intersection_buf[i].y = 0;
        intersection_buf[i].z = 0;
    }
}

__global__  void get_material_buf_device(int *index_buf, t_material *material_buf, t_object_d *objs)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    int j = index_buf[i];
    if (j != -1)
    {
        material_buf[i].color.red = objs[j].color.red;
        material_buf[i].color.blue = objs[j].color.blue;
        material_buf[i].color.green = objs[j].color.green;
        material_buf[i].color.alpha = 255;
        material_buf[i].specular = objs[j].specular;
    }
    else
    {
        material_buf[i].color.red = 0;
        material_buf[i].color.blue = 0;
        material_buf[i].color.green = 0;
        material_buf[i].color.alpha = 255;
        material_buf[i].specular = -1;
    }
}

 __global__ void	get_frame_buf_device(t_color *frame_buf, float3	*ray_buf, float3 *intersection_buf,
	int *index_buf,
	float3 *normal_buf,
	t_material *material_buf,
	t_object_d *objs, t_light *light, int light_nmb)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    int j = index_buf[i];
    if (j != -1)
        frame_buf[i] = reflection_color(ray_buf, normal_buf, material_buf, light, intersection_buf, objs, i, light_nmb);
    else
    {
        frame_buf[i].red = 0;
        frame_buf[i].blue = 0;
        frame_buf[i].green = 0;
        frame_buf[i].alpha = 0;
    } 
}