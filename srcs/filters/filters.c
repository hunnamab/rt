#include "filters.h"
/* у фильтров свои указатели на кернели с отдельной инициализацией, для использования
необходимо передать в t_filter_data.pixels указатель на буфер с пикселями в памяти видеокарты */
void    filters_init(t_filter_data *data)
{
    int fd;
    int ret;
    char *str;
    int err = 0;

    data->kernels = malloc(sizeof(cl_kernel) * 3);
    data->programs = malloc(sizeof(cl_program) * 3);
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
}

void    sepia_filter(t_filter_data *data)
{
    size_t global = WID * HEI;
	size_t local;
    cl_int w;
    cl_int h;
    cl_int err;
    w = WID;
    h = HEI;

    err = clSetKernelArg(data->kernels[0], 0, sizeof(cl_mem), &data->pixels);
	err = clSetKernelArg(data->kernels[0], 1, sizeof(cl_int), &w);
	err = clSetKernelArg(data->kernels[0], 2, sizeof(cl_int), &h);
    err = clGetKernelWorkGroupInfo(data->kernels[0], data->device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    err = clEnqueueNDRangeKernel(data->commands, data->kernels[0], 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(data->commands);
}

void    gray_scale(t_filter_data data)
{
    /* t_color result;
    for(int i = 0; i < WID * HEI; i++)
    {
        result.red = scene->frame_buf[i].red * 0.3 + scene->frame_buf[i].green * 0.59 + scene->frame_buf[i].blue * 0.11;
        scene->frame_buf[i].red = result.red;
        scene->frame_buf[i].green = result.red;
        scene->frame_buf[i].blue = result.red;
    } */
}

void    negative(t_filter_data data)
{

}

void    gauss_filter(t_filter_data data)
{

}