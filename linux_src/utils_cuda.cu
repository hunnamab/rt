extern "C"{
	#include "rt_host.h"
	}
	#include "rt_device.cuh"
void	print_gpu_info(void)
{
	int val;
	cudaDeviceProp *prop;

	prop = (cudaDeviceProp *)malloc(sizeof(cudaDeviceProp));
	cudaGetDeviceCount(&val);
	printf("device count %d\n", val);
	cudaGetDevice(&val);
	printf("current dev %d\n", val);
	cudaGetDeviceProperties(prop, 0);
	printf("device name %s\n", prop->name);
	printf("max threads per block %d\n", prop->maxThreadsPerBlock);
	printf("multiProcessorCount %d\n", prop->multiProcessorCount);
	printf("regsPerBlock %d\n", prop->regsPerBlock);
	printf("warp size %d\n", prop->warpSize);
	printf("canMapHostMemory %d\n", prop->canMapHostMemory);
	printf("maxGridSize[3] (%d,%d,%d)\n", prop->maxGridSize[0],prop->maxGridSize[1],prop->maxGridSize[2]);
}
//тут утечка пока что
__host__ void	device_objects_init(t_scene *scene)
{
	t_object_d *buf;
	int i = 0;

	cudaMalloc(&scene->device_data->obj, sizeof(t_object_d) * scene->obj_nmb);
	buf = (t_object_d *)malloc(sizeof(t_object_d) * scene->obj_nmb);
	while(i < scene->obj_nmb)
	{
		buf[i].type = scene->objs[i]->type;
		if(scene->objs[i]->type == SPHERE)
		{
			t_sphere *s;
			s = reinterpret_cast<t_sphere *>(scene->objs[i]->data);
			buf[i].sphere.center = s->center;
			buf[i].sphere.radius = s->radius;
		}
		if(scene->objs[i]->type == TRIANGLE)
		{
			t_triangle *t;
			t = reinterpret_cast<t_triangle *>(scene->objs[i]->data);
			buf[i].triangle.vertex[0] = t->vertex[0];
			buf[i].triangle.vertex[1] = t->vertex[1];
			buf[i].triangle.vertex[2] = t->vertex[2];
			buf[i].triangle.normal = t->normal;
		}
		if(scene->objs[i]->type == CONE)
		{
			t_cone *cone;
			cone = reinterpret_cast<t_cone *>(scene->objs[i]->data);
			buf[i].cone.position = cone->position;
			buf[i].cone.vec = cone->vec;
			buf[i].cone.angle = cone->angle;
		}
		if(scene->objs[i]->type == CYLINDER)
		{
			t_cylinder *cylinder;
			cylinder = reinterpret_cast<t_cylinder *>(scene->objs[i]->data);
			buf[i].cylinder.position = cylinder->position;
			buf[i].cylinder.vec = cylinder->vec;
			buf[i].cylinder.radius = cylinder->radius;
		}
		if(scene->objs[i]->type == PLANE)
		{
			t_plane *p;
			p = reinterpret_cast<t_plane *>(scene->objs[i]->data);
			buf[i].plane.normal = p->normal;
			buf[i].plane.point = p->point;
			buf[i].plane.d = p->d;
		}
		if(scene->objs[i]->type == ELLIPSOID)
		{
			t_ellipsoid *el;
			el = reinterpret_cast<t_ellipsoid *>(scene->objs[i]->data);
			buf[i].ellipsoid.a = el->a;
			buf[i].ellipsoid.b = el->b;
			buf[i].ellipsoid.c = el->c;
			buf[i].ellipsoid.center = el->center;
		}
		if(scene->objs[i]->type == HYPERBOLOID)
		{
			t_hyperboloid *hr;
			hr = reinterpret_cast<t_hyperboloid *>(scene->objs[i]->data);
			buf[i].hyperboloid.a = hr->a;
			buf[i].hyperboloid.b = hr->b;
			buf[i].hyperboloid.c = hr->c;
			buf[i].hyperboloid.center = hr->center;
		}
		if(scene->objs[i]->type == PARABOLOID)
		{
			t_paraboloid *pr;
			pr = reinterpret_cast<t_paraboloid *>(scene->objs[i]->data);
			buf[i].paraboloid.p = pr->p;
			buf[i].paraboloid.q = pr->q;
			buf[i].paraboloid.center = pr->center;
		}
		buf[i].color.red = scene->objs[i]->color.red;
		buf[i].color.blue = scene->objs[i]->color.blue;
		buf[i].color.green = scene->objs[i]->color.green;
		buf[i].color.alpha = scene->objs[i]->color.alpha;
		buf[i].specular = scene->objs[i]->specular;
		i++;
	}
	cudaMemcpy(scene->device_data->obj, buf, sizeof(t_object_d) * scene->obj_nmb, cudaMemcpyHostToDevice);
}

__host__ void	device_light_init(t_scene *scene)
{
	t_light *buf;
	int i = 0;
	buf = (t_light *)malloc(sizeof(t_light) * scene->light_nmb);
	cudaMalloc(&scene->device_data->light, sizeof(t_light) * scene->light_nmb);
	while(i < scene->light_nmb)
	{
		buf[i].type = scene->light[i]->type;
		buf[i].intensity = scene->light[i]->intensity;
		if(scene->light[i]->type == DIRECTIONAL)
		{
			buf[i].direction = scene->light[i]->direction;
			buf[i].position = scene->light[i]->position;
		}
		if(scene->light[i]->type == POINT)
			buf[i].position = scene->light[i]->position;
		i++;
	}
	if((cudaMemcpy(scene->device_data->light, buf, sizeof(t_light) * scene->light_nmb, cudaMemcpyHostToDevice)) == CUDA_SUCCESS)
		printf("light device mem copy success\n");
}

__host__ void 	intersect_ray_sphere(t_scene *scene, int index)
{
    dim3     gridSize;
    dim3     blockSize;
	t_sphere *sphere;
	gridSize = WID * HEI / 1024;
	blockSize = 1024;
	sphere = reinterpret_cast<t_sphere *>(scene->objs[index]->data);
    intersect_ray_sphere_c<<<gridSize,blockSize>>>(scene->device_data->ray_buf,scene->camera.position,sphere->center,sphere->radius,scene->device_data->depth_buf, scene->device_data->index_buf, index);
}

__host__ void	intersect_ray_triangle(t_scene *scene, int index)
{
	t_triangle	*t;

	t = reinterpret_cast<t_triangle *>(scene->objs[index]->data);
	dim3     gridSize;
    dim3     blockSize;

	gridSize = WID * HEI / 1024;
	blockSize = 1024;
	float3 *vertex;
	cudaMalloc(&vertex, sizeof(float3) * 3);
	cudaMemcpy(vertex, t->vertex,sizeof(float3) * 3, cudaMemcpyHostToDevice);
    intersect_ray_triangle_c<<<gridSize,blockSize>>>(scene->device_data->ray_buf,scene->camera.position,scene->device_data->depth_buf, vertex, scene->device_data->index_buf, index);
	cudaFree(vertex);
}


__host__ void	intersect_ray_cone(t_scene *scene, int index)
{
	t_cone *c;
	dim3     gridSize;
    dim3     blockSize;
	c = reinterpret_cast<t_cone *>(scene->objs[index]->data);
	gridSize = WID * HEI / 1024;
	blockSize = 1024;
    intersect_ray_cone_c<<<gridSize,blockSize>>>(scene->device_data->ray_buf,scene->camera.position,c->position, c->vec, c->angle, scene->device_data->depth_buf, scene->device_data->index_buf, index);
}

__host__ void	intersect_ray_cylinder(t_scene *scene, int index)
{
	t_cylinder *c;

	dim3     gridSize;
    dim3     blockSize;
	c = reinterpret_cast<t_cylinder *>(scene->objs[index]->data);
	gridSize = WID * HEI / 1024;
	blockSize = 1024;
    intersect_ray_cylinder_c<<<gridSize,blockSize>>>(scene->device_data->ray_buf,scene->camera.position,c->position, scene->device_data->depth_buf, c->vec, c->radius, scene->device_data->index_buf, index);
}

__host__ void	intersect_ray_plane(t_scene *scene, int index)
{
	t_plane *p;
	dim3     gridSize;
	dim3     blockSize;
	
	p = reinterpret_cast<t_plane *>(scene->objs[index]->data);
	gridSize = WID * HEI / 1024;
	blockSize = 1024;
    intersect_ray_plane_c<<<gridSize,blockSize>>>(scene->device_data->ray_buf,scene->camera.position, scene->device_data->depth_buf, p->normal, p->d, scene->device_data->index_buf, index);
}

__host__ void 	intersect_ray_ellipsoid(t_scene *scene, int index)
{
    dim3     gridSize;
    dim3     blockSize;
	t_ellipsoid *el;

	gridSize = WID * HEI / 1024;
	blockSize = 1024;
	el = reinterpret_cast<t_ellipsoid *>(scene->objs[index]->data);
    intersect_ray_ellipsoid_c<<<gridSize,blockSize>>>(scene->device_data->ray_buf,scene->camera.position,el->center,el->a, el->b,el->c,scene->device_data->depth_buf, scene->device_data->index_buf, index);
}

__host__ void 	intersect_ray_hyperboloid(t_scene *scene, int index)
{
    dim3     gridSize;
    dim3     blockSize;
	t_hyperboloid *hr;

	gridSize = WID * HEI / 1024;
	blockSize = 1024;
	hr = reinterpret_cast<t_hyperboloid *>(scene->objs[index]->data);
    intersect_ray_hyperboloid_c<<<gridSize,blockSize>>>(scene->device_data->ray_buf,scene->camera.position,hr->center,hr->a, hr->b,hr->c,scene->device_data->depth_buf, scene->device_data->index_buf, index);
}

__host__ void 	intersect_ray_paraboloid(t_scene *scene, int index)
{
    dim3     gridSize;
    dim3     blockSize;
	t_paraboloid *pr;

	gridSize = WID * HEI / 1024;
	blockSize = 1024;
	pr = reinterpret_cast<t_paraboloid *>(scene->objs[index]->data);
    intersect_ray_paraboloid_c<<<gridSize,blockSize>>>(scene->device_data->ray_buf,scene->camera.position,pr->center,pr->p,pr->q,scene->device_data->depth_buf, scene->device_data->index_buf, index);
}