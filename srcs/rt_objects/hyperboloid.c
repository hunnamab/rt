#include "rt.h"


void	one_argument_hyperboloid(char **description, t_scene *scene, int *snmi)
{
	t_object	*hyperboloid;
	cl_float3	cen_buf[2];
	float		specular[6];
	t_color		color;
	int			surface_id;

	cen_buf[0] = get_points(description[1]);
	specular[0] = ftoi(get_coordinates(description[2]));
	specular[1] = ftoi(get_coordinates(description[3]));
	specular[2] = ftoi(get_coordinates(description[4]));
	cen_buf[1] = get_points(description[5]);
	color = get_color(description[6]);
	specular[3] = ftoi(get_coordinates(description[7]));
	specular[4] = ftoi(get_coordinates(description[8]));
	specular[5] = ftoi(get_coordinates(description[9]));
	surface_id = ftoi(get_coordinates(description[10]));
	hyperboloid = new_hyperboloid(cen_buf, specular, color, surface_id);
	hyperboloid->text = tex_new_bmp(get_file(description[11]));
	scene->objs[snmi[1]] = hyperboloid;
	snmi[1]++;
}

t_object 	*multiple_hyperboloids(char **description, int i)
{
	t_object	*hyperboloid;
	cl_float3	cen_buf[2];
	float		specular[6];
	t_color 	color;
	int 		surface_id;

	cen_buf[0] = get_points(description[i + 1]);
	specular[0] = ftoi(get_coordinates(description[i + 2]));
	specular[1] = ftoi(get_coordinates(description[i + 3]));
	specular[2] = ftoi(get_coordinates(description[i + 4]));
	cen_buf[1] = get_points(description[i + 5]);
	color = get_color(description[i + 6]);
	specular[3] = ftoi(get_coordinates(description[i + 7]));
	specular[4] = ftoi(get_coordinates(description[i + 8]));
	specular[5] = ftoi(get_coordinates(description[i + 9]));
	surface_id = ftoi(get_coordinates(description[i + 10]));
	hyperboloid = new_hyperboloid(cen_buf, specular, color, surface_id);
	return (hyperboloid);
}

void	get_hyperboloid(char **description, t_scene *scene, int *snmi)
{
	t_object	*hyperboloid;
	int i;

	i = 1;
	if (description[0][0] == '[')
	{
		while (description[i][1] != ']')
		{
			if (description[i][2] == '{')
			{
				hyperboloid = multiple_hyperboloids(description, i);
				hyperboloid->text = tex_new_bmp(get_file(description[i + 11]));
				scene->objs[snmi[1]] = hyperboloid;
				snmi[1]++;
				i += 13;
			}
		}
	}
	if (description[0][0] == '{')
		one_argument_hyperboloid(description, scene, snmi);
}

t_object    *new_hyperboloid(cl_float3 *cen_buf, float *specular, t_color color, int surface_id)
{
    t_hyperboloid *hyper;
	t_object	*new_object;
	float		**matrix;

	new_object = malloc(sizeof(t_object));
	hyper = malloc(sizeof(t_hyperboloid));
	hyper->center = cen_buf[0]; 
	hyper->a = specular[0];  
	hyper->b = specular[1];
	hyper->c = specular[2];
	new_object->rotation[0] = cen_buf[1].x;
	new_object->rotation[1] = cen_buf[1].y;
	new_object->rotation[2] = cen_buf[1].z;
	new_object->specular = specular[3];
	new_object->reflection = specular[4];
	new_object->refraction = specular[5];
	new_object->color = color;
	new_object->cs_nmb = 0;
	new_object->surface_id = surface_id;
	new_object->cutting_surfaces = NULL;
	new_object->text = NULL;
	new_object->normal_text = NULL;
	new_object->data = (void *)hyper;
	new_object->type = HYPERBOLOID;
	new_object->intersect = &intersect_ray_hyperboloid;
	//new_object->get_normal = &get_hyperboloid_normal;
	new_object->clear_obj = &clear_default;
    //printf("abc == (%f,%f,%f)\n", hyper->abc.x,hyper->abc.y,hyper->abc.z);
	return (new_object);
}

void        intersect_ray_hyperboloid(t_scene *scene, int index)
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
    printf("\n\n%hhu\n\n", scene->objs[index]->color.red);
	clSetKernelArg(scene->cl_data.kernels[14], 0, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[14], 1, sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
    clSetKernelArg(scene->cl_data.kernels[14], 2, sizeof(t_hyperboloid), scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[14], 3, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[14], 4, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[14], 5, sizeof(cl_int), (void*)&index);
	clSetKernelArg(scene->cl_data.kernels[14], 6, sizeof(cl_int), (void*)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[14], 7, sizeof(cl_mem), &cs);
	clSetKernelArg(scene->cl_data.kernels[14], 8, sizeof(cl_int), (void*)&scene->objs[index]->cs_nmb);
	clSetKernelArg(scene->cl_data.kernels[14], 9, sizeof(cl_mem), &scene->cl_data.scene.material_buf);

    clGetKernelWorkGroupInfo(scene->cl_data.kernels[14], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	printf("hyperboloid local == %ld\n", local);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[14], 1, NULL, &global, &local, 0, NULL, NULL);
}
