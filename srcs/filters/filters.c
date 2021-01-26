#include "filters.h"
/* у фильтров свои указатели на кернели с отдельной инициализацией, для использования
необходимо передать в t_filter_data.pixels указатель на буфер с пикселями в памяти видеокарты */
void    filters_init(t_filter_data *data)
{
    int fd;
    int ret;
    char *str;
    int err = 0;

    data->kernels = malloc(sizeof(cl_kernel) * 10);
    data->programs = malloc(sizeof(cl_program) * 10);
    fd = open("./srcs/filters/sepia.cl", O_RDONLY);
    str = protected_malloc(sizeof(char), 64001);
    ret = read(fd, str, 64000);
    str[ret] = '\0';
    data->programs[0] = clCreateProgramWithSource( data->context, 1, (const char **)&str, NULL, &err);
    err != 0 ? printf("filter sepia program create error %d\n", err) : 0;
    clBuildProgram(data->programs[0], 1, & data->device_id, NULL, NULL, &err);
    err != 0 ? printf("filter sepia program build error %d\n", err) : 0;
     data->kernels[0] = clCreateKernel( data->programs[0], "sepia", &err);
    err != 0 ? printf("filter sepia kernel compile error %d\n", err) : printf("sepia kernel created\n");
    close(fd);
    ft_memset(str, 0, 64001);
    fd = open("./srcs/filters/negative.cl", O_RDONLY);
    str = protected_malloc(sizeof(char), 64001);
    ret = read(fd, str, 64000);
    str[ret] = '\0';
    data->programs[NEGATIVE] = clCreateProgramWithSource( data->context, 1, (const char **)&str, NULL, &err);
    err != 0 ? printf("filter negative program create error %d\n", err) : 0;
    clBuildProgram(data->programs[NEGATIVE], 1, & data->device_id, NULL, NULL, &err);
    err != 0 ? printf("filter negative program build error %d\n", err) : 0;
    data->kernels[NEGATIVE] = clCreateKernel( data->programs[NEGATIVE], "negative", &err);
    err != 0 ? printf("filter negative kernel compile error %d\n", err) : printf("negative kernel created\n");
     ft_memset(str, 0, 64001);
    fd = open("./srcs/filters/grayscale.cl", O_RDONLY);
    str = protected_malloc(sizeof(char), 64001);
    ret = read(fd, str, 64000);
    str[ret] = '\0';
    data->programs[GRAYSCALE] = clCreateProgramWithSource( data->context, 1, (const char **)&str, NULL, &err);
    err != 0 ? printf("filter grayscale program create error %d\n", err) : 0;
    clBuildProgram(data->programs[GRAYSCALE], 1, & data->device_id, NULL, NULL, &err);
    err != 0 ? printf("filter grayscale program build error %d\n", err) : 0;
    data->kernels[GRAYSCALE] = clCreateKernel( data->programs[GRAYSCALE], "grayscale", &err);
    err != 0 ? printf("filter grayscale kernel compile error %d\n", err) : printf("grayscale kernel created\n");
    ft_memset(str, 0, 64001);
    close(fd);
}

void    sepia_filter(t_filter_data *data)
{
    size_t global = WID * HEI;
	size_t local;
    cl_int err;

    err = clSetKernelArg(data->kernels[0], 0, sizeof(cl_mem), &data->pixels);
    err = clGetKernelWorkGroupInfo(data->kernels[0], data->device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    err = clEnqueueNDRangeKernel(data->commands, data->kernels[0], 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(data->commands);
}

void    gray_scale(t_filter_data *data)
{
     size_t global = WID * HEI;
	size_t local;
    cl_int err;

    err = clSetKernelArg(data->kernels[GRAYSCALE], 0, sizeof(cl_mem), &data->pixels);
    err = clGetKernelWorkGroupInfo(data->kernels[GRAYSCALE], data->device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    err = clEnqueueNDRangeKernel(data->commands, data->kernels[GRAYSCALE], 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(data->commands);
}

void    negative(t_filter_data *data)
{
    size_t global = WID * HEI;
	size_t local;
    cl_int err;

    err = clSetKernelArg(data->kernels[NEGATIVE], 0, sizeof(cl_mem), &data->pixels);
    err = clGetKernelWorkGroupInfo(data->kernels[NEGATIVE], data->device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    err = clEnqueueNDRangeKernel(data->commands, data->kernels[NEGATIVE], 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(data->commands);
}

void    gauss_filter(t_filter_data *data)
{

}