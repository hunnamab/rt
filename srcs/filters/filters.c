#include "filters.h"

void    filters_init(t_filter_data data)
{
    int fd;
    int ret;
    char *str;
    int err = 0;

    fd = open("./sepia_kernel.cl", O_RDONLY);
    str = protected_malloc(sizeof(char), 64001);
    ret = read(fd, str, 64000);
    str[ret] = '\0';
    data.programs[0] = clCreateProgramWithSource(data.context, 1, (const char **)&str, NULL, &err);
    err != 0 ? printf("filter %d program create error %d\n", 0, err) : 0;
    clBuildProgram(data.programs[0], 1, &data.device_id, NULL, NULL, &err);
    err != 0 ? printf("filter %d program build error %d\n", 0, err) : 0;
    data.kernels[0] = clCreateKernel(data.programs[0], "sepia", &err);
    err != 0 ? printf("filter %d kernel compile error %d\n", 0, err) : 0;
}

void    sepia_filter(t_scene *scene)
{
    t_color buf;
    int check;
    for(int i = 0; i < WID * HEI; i++)
    {
        buf.red = scene->frame_buf[i].red;
        buf.green = scene->frame_buf[i].green;
        buf.blue = scene->frame_buf[i].blue;
        check = buf.red * 0.393 + buf.green * 0.769 + buf.blue * 0.189;
        scene->frame_buf[i].red = check > 255 ? 255 : check;
        check = buf.red * 0.349 + buf.green * 0.686 + buf.blue * 0.168;
        scene->frame_buf[i].green = check > 255 ? 255 : check;
        check = buf.red * 0.272 + buf.green * 0.534 + buf.blue * 0.131;
        scene->frame_buf[i].blue = check > 255 ? 255 : check;
    }
}

void    gray_scale(t_scene *scene)
{
    t_color result;
    for(int i = 0; i < WID * HEI; i++)
    {
        result.red = scene->frame_buf[i].red * 0.3 + scene->frame_buf[i].green * 0.59 + scene->frame_buf[i].blue * 0.11;
        scene->frame_buf[i].red = result.red;
        scene->frame_buf[i].green = result.red;
        scene->frame_buf[i].blue = result.red;
    }
}

void    negative(t_filter_data data)
{

}

void    gauss_filter(t_scene *scene)
{

}