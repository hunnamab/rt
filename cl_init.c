#include "rt.h"

// const char *get_ray_arr = "\n" \
// "__kernel void get_ray_arr(                                             \n" \
// "   __global float3* ray_arr,                                           \n" \
// "   __global float3 *camera_start,                                       \n" \
// "    __global float3* viewport,                                         \n" \
// "   const unsigned int count)                                           \n" \
// "{                                                                      \n" \
// "   int i = get_global_id(0);                                           \n" \
// "   ray_arr[i] = viewport[i] - camera_start[0];                         \n" \
// "}                                                                      \n" \
// "\n";

// const char *get_ray = "\n" \
// "__kernel void get_ray(                                             \n" \
// "   __global float3* ray_arr,                                           \n" \
// "   __global float3 *camera_start,                                       \n" \
// "    __global float3* viewport,                                         \n" \
// "   const unsigned int count)                                           \n" \
// "{                                                                      \n" \
// "   int i = get_global_id(0);                                           \n" \
// "   ray_arr[i] = viewport[i] - camera_start[0];                         \n" \
// "}                                                                      \n" \
// "\n";

// const char *get_closest_point = "\n" \
// "__kernel void get_closest_point(                                       \n"\
// "   __global float3 *ray_arr,                                           \n"\
// "   __global float3 *camera_start,                                      \n"\
// "   __global float3 *s_center,                                      	\n"\
// "   __global float3 *s_radius,                                      	\n"\
// "    __global float3 *depth_buf)                                        \n"\
// "{                                                                      \n"\
// "   int i = get_global_id(0);                                           \n"\
// "   float a = dot(ray_arr[i], ray_arr[i]);                              \n"\
// "   float b;                                                            \n"\
// "   float c;                                                            \n"\
// "   float t1;                                                           \n"\
// "   float t2;                                                           \n"\
// "   float3 dist = camera_start[0] - s_center[0];                        \n"\
// "   b = 2 * dot(dist, ray_arr[i]);                                      \n"\
// "   c = dot(dist, dist) - (s_radius[0] * s_radius[0]);                  \n"\
// "   c = b * b - 4 * a * c;                                              \n"\
// "   if (c >= 0)                                                         \n"\
// "   {                                                                   \n"\
// "    	c = sqrt(c);                                                    \n"\
// "    	t1 = (-b + c) / (2 * a);                                        \n"\
// "    	t2 = (-b - c) / (2 * a);                                        \n"\
// "    	depth_buf[i] = t1 < t2 ? t1 : t2;                               \n"\
// "   }                                                                   \n"\
// "}                                                                      \n"\
// "\n";

int    cl_init(t_scene *scene)
{
	int err;
	unsigned int correct; // number of correct results returned
	unsigned int count = WID * HEI;
	size_t global; // global domain size for our calculation
	size_t local; // local domain size for our calculation
	int i;
	err = 0;
    cl_uint nP;
    cl_uint status = clGetPlatformIDs(0, NULL, &nP);
    cl_platform_id pfs;
    status = clGetPlatformIDs(nP, &pfs, NULL);
    size_t size;
    char *str;
    clGetPlatformInfo(pfs, CL_PLATFORM_NAME, 0, NULL, &size);
    str = malloc(sizeof(char) * size);
    clGetPlatformInfo(pfs, CL_PLATFORM_NAME, size, str, NULL);
    printf("%s\n", str);
	err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &scene->cl_data.device_id, NULL);

	// выделение памяти
	scene->cl_data.programs = malloc(sizeof(cl_program) * KERNEL_NUM);
	scene->cl_data.kernels = malloc(sizeof(cl_kernel) * KERNEL_NUM);
	
	int		ret1;
	char	*get_ray_arr;
	int fd1 = open("get_ray_arr.cl", O_RDONLY);
	get_ray_arr = protected_malloc(sizeof(char), 256000);
	ret1 = read(fd1, get_ray_arr, 64000);
	get_ray_arr[ret1] = '\0';

	scene->cl_data.context = clCreateContext(0, 1, &scene->cl_data.device_id, NULL, NULL, &err);
	scene->cl_data.commands = clCreateCommandQueue(scene->cl_data.context, scene->cl_data.device_id, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &err);
	scene->cl_data.programs[0] = clCreateProgramWithSource(scene->cl_data.context, 1, (const char **)&get_ray_arr, NULL, &err);
	err = clBuildProgram(scene->cl_data.programs[0], 0, NULL, NULL, NULL, NULL);

	scene->cl_data.kernels[0] = clCreateKernel(scene->cl_data.programs[0], "get_ray_arr", &err);
	
	char info[1024];
	clGetDeviceInfo(scene->cl_data.device_id, CL_DEVICE_NAME, 1024, info, NULL);
	printf("%s\n", info);
	
	int		ret2;
	char	*get_closest_point;
	int fd2 = open("get_closest_point.cl", O_RDONLY);
	get_closest_point = protected_malloc(sizeof(char), 256000);
	ret2 = read(fd2, get_closest_point, 64000);
	get_closest_point[ret2] = '\0';
	if ((scene->cl_data.programs[1] = clCreateProgramWithSource(scene->cl_data.context, 1, (const char **)&get_closest_point, NULL, &err)))
		printf("cоздана программа\n");
	if ((clBuildProgram(scene->cl_data.programs[1], 0, NULL, NULL, NULL, &err)))
		printf("собрана программа\n");
	if (!(scene->cl_data.kernels[1] = clCreateKernel(scene->cl_data.programs[1], "get_closest_point", &err)))
		printf("не собрана программа 1, error %d\n", err);

	//Создание буферов на гпу
	scene->cl_data.scene.ray_buf = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(cl_float3) * count, NULL, NULL);
	scene->cl_data.scene.viewport = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(cl_float3) * count, NULL, NULL);
	scene->cl_data.scene.camera = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(cl_float3), NULL, NULL);
	scene->cl_data.scene.intersection_buf = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(cl_float3) * count, NULL, NULL);
	scene->cl_data.scene.index_buf = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(int) * count, NULL, NULL);
	scene->cl_data.scene.depth_buf = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(float) * count, NULL, NULL);
	scene->cl_data.scene.normal_buf = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(cl_float3) * count, NULL, NULL);
	return (0);
}
