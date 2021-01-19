#include "rt.h"
#include "types.h"

/* void	get_material_buf(t_scene *scene)
{
	int x;
	int y;
	int i;
	t_color col;
	cl_float3 t;
	size_t global = WID * HEI;

	y = -1;
	while (++y < HEI)
	{
		x = -1;
		while (++x < WID)
		{
			i = y * WID + x;
			if (scene->index_buf[i] != -1)
			{	
				if (scene->texts[scene->objs[scene->index_buf[i]]->texture_id] != NULL)
				{
					t = text_map_select(scene->objs[scene->index_buf[i]], scene->intersection_buf[i]);
					col = get_color_tex(scene->texts[scene->objs[scene->index_buf[i]]->texture_id], t.x, t.y);
				}
				else
					col = scene->objs[scene->index_buf[i]]->color;
				copy_color(&scene->material_buf[i].color,  &col);
				scene->material_buf[i].specular = scene->objs[scene->index_buf[i]]->specular;
			}
			else
			{
				set_color_zero(&scene->material_buf[i].color);
				scene->material_buf[i].specular = -1;
			}
		}
	}
	//clEnqueueWriteBuffer(scene->cl_data.commands, scene->cl_data.scene.material_buf, CL_FALSE, 0, sizeof(t_material) * global, scene->material_buf, 0, NULL, NULL);
} */

void	get_material_buf(t_scene *scene)
{
	size_t global;
	size_t local;

	global = WID * HEI;
    clSetKernelArg(scene->cl_data.kernels[10], 0, sizeof(cl_mem), &scene->cl_data.scene.textures);
	clSetKernelArg(scene->cl_data.kernels[10], 1, sizeof(cl_mem), &scene->cl_data.scene.obj);
    clSetKernelArg(scene->cl_data.kernels[10], 2, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[10], 3, sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[10], 4, sizeof(cl_mem), &scene->cl_data.scene.material_buf);
    clGetKernelWorkGroupInfo(scene->cl_data.kernels[10], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	printf("local get material buf == %ld\n", local);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[10], 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(scene->cl_data.commands);
/* 	int x;
	int y;
	int i;
	t_color col;
	cl_float3 t;

	y = -1;
	while (++y < HEI)
	{
		x = -1;
		while (++x < WID)
		{
			i = y * WID + x;
			if (scene->index_buf[i] != -1)
			{	
				if (scene->texts[scene->objs[scene->index_buf[i]]->texture_id] != NULL)
				{
					t = text_map_select(scene->objs[scene->index_buf[i]], scene->intersection_buf[i]);
					col = get_color_tex(scene->texts[scene->objs[scene->index_buf[i]]->texture_id], t.x, t.y);
				}
				else
					col = scene->objs[scene->index_buf[i]]->color;
				copy_color(&scene->material_buf[i].color,  &col);
				scene->material_buf[i].specular = scene->objs[scene->index_buf[i]]->specular;
			}
			else
			{
				set_color_zero(&scene->material_buf[i].color);
				scene->material_buf[i].specular = -1;
			}
		}
	} */
	clEnqueueReadBuffer(scene->cl_data.commands, scene->cl_data.scene.material_buf, CL_FALSE, 0, sizeof(t_material) * global, scene->material_buf, 0, NULL, NULL);
}

/* void	get_material_buf(t_scene *scene)
{
	int x;
	int y;
	int i;

	y = -1;
	while (++y < HEI)
	{
		x = -1;
		while (++x < WID)
		{
			i = y * WID + x;
			if (scene->index_buf[i] != -1)
			{
				copy_color(&scene->material_buf[i].color, \
							&scene->objs[scene->index_buf[i]]->color);
				scene->material_buf[i].specular = \
				scene->objs[scene->index_buf[i]]->specular;
			}
			else
			{
				set_color_zero(&scene->material_buf[i].color);
				scene->material_buf[i].specular = -1;
			}
		}
	}
} */
