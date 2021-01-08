#ifndef RT_DEVICE_CUH
# define RT_DEVICE_CUH
# include <cuda_runtime_api.h>
# include <vector_types.h>
# include <cuda.h>
# include <stdio.h>
# include <math.h>
# include "device_launch_parameters.h"
# include "types.h"

#if defined(__CUDA_ARCH__) && (__CUDA_ARCH__ < 200)
    #define printf(f, ...) ((void)(f, __VA_ARGS__),0)
#endif

__device__ float	dot(float3 v1, float3 v2);
__device__ float3	sub(float3 v1, float3 v2);
__device__ float3	cross(float3 v1, float3 v2);
__device__ float3	scale(float3 v, float c);
__device__ float3	add(const float3 v1, const float3 v2);
__global__ void kernel_getray(float3 *viewport, float3 camera, float3 *ray_buf);
__global__ void helloCUDA(float f);
//intersect_cuda.cu
__global__ void intersect_ray_sphere_c(float3 *ray_arr, float3 camera_start, float3 s_center, \
     float s_radius, float *depth_buf, int *index_buf, int index);
__global__ void intersect_ray_triangle_c(float3 *ray_arr, float3 camera_start, float *depth_buf, \
     float3 *vertex, int *index_buf, int index);
__global__ void intersect_ray_cone_c(float3 *ray_arr, float3 camera_start, float3 position, \
    float3 vector, float angle, float *depth_buf, int *index_buf, int index);
__global__ void intersect_ray_plane_c(float3 *ray_arr, float3 camera_start, \
     float *depth_buf, float3 normal, float d, int *index_buf, int index);
__global__ void intersect_ray_cylinder_c(float3 *ray_arr, float3 camera_start, \
    float3 position, float *depth_buf, float3 vector, \
    float radius, int *index_buf, int index);
__global__  void    intersect_ray_ellipsoid_c(float3 *ray_arr, float3 camera_start, float3 e_center, float a, float b, float c, float *depth_buf, int *index_buf, int index);
__global__  void    intersect_ray_hyperboloid_c(float3 *ray_arr, float3 camera_start, float3 e_center, float a, float b, float c, float *depth_buf, int *index_buf, int index);
__global__  void    intersect_ray_paraboloid_c(float3 *ray_arr, float3 camera_start, float3 e_center,float p, float q, float *depth_buf, int *index_buf, int index);
__global__ void     kernel_get_intersection_point(float3 *intersection_buf, float3 *ray_buf, float *depth_buf, float3 camera, int *index_buf);
__device__ float3	div_by_scalar(float3 vector, float scalar);
__device__ float	length(float3 vector);
__global__ void test(t_object_d *objs, int index);
__global__ void new_sphere_device(t_object_d *obj, float3 center, float radius, t_color color, \
    float *rotation, float specular, int index);
__host__ void	device_objects_init(t_scene *scene);
__host__ void	device_light_init(t_scene *scene);
__global__  void get_material_buf_device(int *index_buf, t_material *material_buf, t_object_d *obj);
//get_normal_buf.cu
__global__ void	get_normal_buf_device(t_object_d *obj, float3 *ray_buf, int *index_buf, float3 *normal_buf, float3 *intersection_buf, float *depth_buf,float3 camera_position);
__device__ void get_normal_sphere(t_object_d *obj, float3 *ray_buf, int *index_buf, float3 *normal_buf, float3 *intersection_buf, int index);
__device__ void get_normal_plane(t_object_d *obj, float3 *ray_buf, int *index_buf, float3 *normal_buf, float3 *intersection_buf, int index);
__device__ void get_normal_cone(t_object_d *obj, float3 *ray_buf, int *index_buf, float3 *normal_buf, float3 *intersection_buf, int index, float3 camera_position, float *depth_buf);
__device__ void get_normal_cylinder(t_object_d *obj, float3 *ray_buf, int *index_buf, float3 *normal_buf, float3 *intersection_buf, int index, float3 camera_position, float *depth_buf);
__device__ void get_normal_triangle(t_object_d *obj, float3 *ray_buf, int *index_buf, float3 *normal_buf, float3 *intersection_buf, int index);
__device__ void get_normal_ellipsoid(t_object_d *obj, float3 *ray_buf, int *index_buf, float3 *normal_buf, float3 *intersection_buf, int index);
__global__ void	get_frame_buf_device(t_color *frame_buf, float3	*ray_buf, float3 *intersection_buf,
	int					*index_buf,
	float3				*normal_buf,
	t_material			*material_buf,
    t_object_d			*obj, t_light *light, int light_nmb);
__device__ t_color		reflection_color(float3 *ray_buf,float3 *normal_buf, t_material *material_buf, t_light *light, float3 *intersection_buf, t_object_d *objs, int index, int light_nmb);
__device__ int			in_shadow(int index, float3 l);

#endif