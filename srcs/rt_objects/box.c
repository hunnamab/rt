#include "rt.h"

void	one_argument_box(char **description, t_scene *scene, int *snmi)
{
	t_object	*box;
	cl_float3	cen_buf[3];
	float		rotation[3];
	float		specular;
	t_color		color;

	cen_buf[0] = get_points(description[1]);
	cen_buf[1] = get_points(description[2]);
	cen_buf[2] = get_points(description[3]);
	rotation[0] = cen_buf[2].x;
	rotation[1] = cen_buf[2].y;
	rotation[2] = cen_buf[2].z;
	color = get_color(description[4]);
	specular = ftoi(get_coordinates(description[5]));
	box = new_box(cen_buf, color, specular);
	box->text = tex_new_bmp(get_file(description[6]));
	scene->objs[snmi[1]] = box;
	snmi[1]++;
}

t_object 	*multiple_boxes(char **description, t_scene *scene, int *snmi, int i)
{
	t_object	*box;
	cl_float3	cen_buf[3];
	float		rotation[3];
	float		specular;
	t_color 	color;

	cen_buf[0] = get_points(description[i + 1]);
	cen_buf[1] = get_points(description[i + 2]);
	cen_buf[2] = get_points(description[i + 3]);
	rotation[0] = cen_buf[2].x;
	rotation[1] = cen_buf[2].y;
	rotation[2] = cen_buf[2].z;
	color = get_color(description[i + 4]);
	specular = ftoi(get_coordinates(description[i + 5]));
	box = new_box(cen_buf, color, specular);
	return (box);
}

void	get_box(char **description, t_scene *scene, int *snmi)
{
	t_object	*box;
	int i;

	i = 1;
	//printf("center %c\n", description[0][0]);
	if (description[0][0] == '[')
	{
		while (description[i][1] != ']')
		{
			//printf("text %c\n", description[i][2]);
			if (description[i][2] == '{')
			{
				box = multiple_boxes(description, scene, snmi, i);
				box->text = tex_new_bmp(get_file(description[i + 6]));
				scene->objs[snmi[1]] = box;
				snmi[1]++;
				i += 8;
			}
		}
	}
	if (description[0][0] == '{')
		one_argument_box(description, scene, snmi);
}

t_object    *new_box(cl_float3 *buf, t_color color, float specular)
{
    t_box *box;
	t_object	*new_object;
	float		**matrix;

	new_object = malloc(sizeof(t_object));
	box = malloc(sizeof(t_box));
	box->a = buf[0];
    box->b = buf[1];
	new_object->rotation[0] = buf[2].x;
	new_object->rotation[1] = buf[2].y;
	new_object->rotation[2] = buf[2].z;
	new_object->specular = specular;
	new_object->reflection = 0.0;
	new_object->color = color;
	new_object->text = NULL;
	new_object->data = (void *)box;
	new_object->type = BOX;
	new_object->cs_nmb = 0;
	new_object->intersect = &intersect_ray_box;
	//new_object->get_normal = &get_box_normal;
	new_object->clear_obj = &clear_default;
	return (new_object);
}

void        intersect_ray_box(t_scene *scene, int index)
{
    size_t global = WID * HEI;
	size_t local;
	cl_mem cs;
	if (scene->objs[index]->cs_nmb > 0)
	{
		cs = clCreateBuffer(scene->cl_data.context, CL_MEM_READ_ONLY |
		CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(t_cutting_surface) * scene->objs[index]->cs_nmb, scene->objs[index]->cutting_surfaces, NULL);
	}
	else
		cs = NULL;
	clSetKernelArg(scene->cl_data.kernels[11], 0, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[11], 1, sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
    clSetKernelArg(scene->cl_data.kernels[11], 2, sizeof(t_box), scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[11], 3, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[11], 4, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[11], 5, sizeof(cl_int), (void*)&index);
	clSetKernelArg(scene->cl_data.kernels[11], 6, sizeof(cl_float), (void*)&scene->objs[index]->reflection);
	clSetKernelArg(scene->cl_data.kernels[11], 7, sizeof(cl_int), (void*)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[11], 8, sizeof(cl_mem), &cs);
	clSetKernelArg(scene->cl_data.kernels[11], 9, sizeof(cl_int), (void*)&scene->objs[index]->cs_nmb);

    clGetKernelWorkGroupInfo(scene->cl_data.kernels[11], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[11], 1, NULL, &global, &local, 0, NULL, NULL);
}
