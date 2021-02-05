#include "rt.h"

void	one_argument_ellipsoid(char **description, t_scene *scene, int *snmi)
{
	t_object	*ellipsoid;
	cl_float3	cen_buf[3];
	float		rotation[3];
	float		specular[4];
	t_color		color;
	int			surface_id;

	cen_buf[0] = get_points(description[1]);
	cen_buf[1] = get_points(description[2]);
	specular[0] = ftoi(get_coordinates(description[3]));
	cen_buf[2] = get_points(description[4]);
	rotation[0] = cen_buf[2].x;
	rotation[1] = cen_buf[2].y;
	rotation[2] = cen_buf[2].z;
	color = get_color(description[5]);
	specular[1] = ftoi(get_coordinates(description[6]));
	specular[2] = ftoi(get_coordinates(description[7]));
	specular[3] = ftoi(get_coordinates(description[8]));
	surface_id = ftoi(get_coordinates(description[9]));
	ellipsoid = new_ellipsoid(cen_buf, color, specular, surface_id);
	ellipsoid->text = tex_new_bmp(get_file(description[10]));
	scene->objs[snmi[1]] = ellipsoid;
	snmi[1]++;
}

t_object 	*multiple_ellipsoids(char **description, t_scene *scene, int *snmi, int i)
{
	t_object	*ellipsoid;
	cl_float3	cen_buf[3];
	float		rotation[3];
	float		specular[4];
	t_color 	color;
	int			surface_id;

	printf("{} %s\n", description[i + 1]);
	cen_buf[0] = get_points(description[i + 1]);
	cen_buf[1] = get_points(description[i + 2]);
	specular[0] = ftoi(get_coordinates(description[i + 3]));
	cen_buf[2] = get_points(description[i + 4]);
	rotation[0] = cen_buf[2].x;
	rotation[1] = cen_buf[2].y;
	rotation[2] = cen_buf[2].z;
	color = get_color(description[i + 5]);
	specular[1] = ftoi(get_coordinates(description[i + 6]));
	specular[2] = ftoi(get_coordinates(description[i + 7]));
	specular[3] = ftoi(get_coordinates(description[i + 8]));
	surface_id = ftoi(get_coordinates(description[i + 9]));
	ellipsoid = new_ellipsoid(cen_buf, color, specular, surface_id);
	return (ellipsoid);
}


t_object    *new_ellipsoid(cl_float3 *buf, t_color color, float *specular, int surface_id)
{
    t_ellipsoid *el;
	t_object	*new_object;
	float		**matrix;

	new_object = malloc(sizeof(t_object));
	el = malloc(sizeof(t_ellipsoid));
	el->center = buf[0]; 
	el->a = 3;//buf[1];  
	el->b = 4; //specular[0];
	el->c = 5;//specular[0];
	new_object->rotation[0] = buf[2].x;
	new_object->rotation[1] = buf[2].y;
	new_object->rotation[2] = buf[2].z;
	new_object->specular = specular[1];
	new_object->reflection = specular[2];
	new_object->refraction = specular[3];
	new_object->color = color;
	new_object->cs_nmb = 0;
	new_object->surface_id = surface_id;
	new_object->cutting_surfaces = NULL;
	new_object->text = NULL;
	new_object->normal_text = NULL;
	new_object->data = (void *)el;
	new_object->type = ELLIPSOID;
	new_object->intersect = &intersect_ray_ellipsoid;
	//new_object->get_normal = &get_ellipsoid_normal;
	new_object->clear_obj = &clear_default;
    //printf("abc == (%f,%f,%f)\n", el->abc.x,el->abc.y,el->abc.z);
	return (new_object);
}

void	get_ellipsoid(char **description, t_scene *scene, int *snmi)
{
	t_object	*ellipsoid;
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
				ellipsoid = multiple_ellipsoids(description, scene, snmi, i);
				ellipsoid->text = tex_new_bmp(get_file(description[i + 10]));
				scene->objs[snmi[1]] = ellipsoid;
				snmi[1]++;
				i += 12;
			}
		}
	}
	if (description[0][0] == '{')
		one_argument_ellipsoid(description, scene, snmi);
}

void        intersect_ray_ellipsoid(t_scene *scene, int index, int is_refractive)
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
	clSetKernelArg(scene->cl_data.kernels[8], 0, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[8], 1, sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
    clSetKernelArg(scene->cl_data.kernels[8], 2, sizeof(t_ellipsoid), scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[8], 3, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[8], 4, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[8], 5, sizeof(cl_int), (void*)&index);
	clSetKernelArg(scene->cl_data.kernels[8], 6, sizeof(cl_int), (void*)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[8], 7, sizeof(cl_mem), &cs);
	clSetKernelArg(scene->cl_data.kernels[8], 8, sizeof(cl_int), (void*)&scene->objs[index]->cs_nmb);
	clSetKernelArg(scene->cl_data.kernels[8], 9, sizeof(cl_mem), &scene->cl_data.scene.material_buf);

    clGetKernelWorkGroupInfo(scene->cl_data.kernels[8], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	printf("ellipsoid local == %ld\n", local);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[8], 1, NULL, &global, &local, 0, NULL, NULL);
}
