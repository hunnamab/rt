#include "rt_host.h"
#include "rt_device.cuh"

t_object	*new_sphere(float3 center, float *rad_spec, t_color color, \
						float *rotation)
{
	t_sphere *new_sphere;
	t_object *new_object;

	new_object = protected_malloc(sizeof(t_object), 1);
	new_sphere = protected_malloc(sizeof(t_sphere), 1);
	new_sphere->center = center;
	new_sphere->radius = rad_spec[0];
	new_object->rotation[0] = rotation[0];
	new_object->rotation[1] = rotation[1];
	new_object->rotation[2] = rotation[2];
	new_object->specular = rad_spec[1];
	new_object->color = color;
	new_object->data = (void *)new_sphere;
	new_object->data_size = sizeof(t_sphere);
	new_object->tag = "sphere";
	new_object->type = SPHERE;
	new_object->clear_obj = &clear_default;
	new_object->intersect = &intersect_ray_sphere;
	return (new_object);
}

static void	init_norme(float *r, float3 *v, float *rt, float3 *vt)
{
	rt[0] = r[0];
	rt[1] = r[1];
	rt[2] = r[2];
	vt[0] = v[0];
	vt[1] = v[1];
	vt[2] = v[2];
}

t_object	*new_triangle(float3 *vertex, float specular, t_color color, \
							float *rotation)
{
	t_triangle	*new_triangle;
	t_object	*new_object;
	float3		edge1;
	float3		edge2;
	float		**matrix;

	new_object = protected_malloc(sizeof(t_object), 1);
	new_triangle = protected_malloc(sizeof(t_triangle), 1);
	new_triangle->vertex = protected_malloc(sizeof(float3), 3);
	init_norme(rotation, vertex, new_object->rotation, new_triangle->vertex);
	matrix = get_rotation_matrix(new_object->rotation);
	transform(new_triangle->vertex, matrix, 3);
	matr_free(matrix, 4);
	edge1 = vector_sub(&new_triangle->vertex[0], &new_triangle->vertex[1]);
	edge2 = vector_sub(&new_triangle->vertex[1], &new_triangle->vertex[2]);
	new_triangle->normal = vector_cross(&edge1, &edge2);
	normalize_vector(&new_triangle->normal);
	new_object->type = TRIANGLE;
	new_object->specular = specular;
	new_object->color = color;
	new_object->data = (void *)new_triangle;
	new_object->tag = "triangle";
	new_object->clear_obj = &clear_triangle;
	new_object->intersect = &intersect_ray_triangle;
	return (new_object);
}

void		clear_triangle(t_object *obj)
{
	t_triangle *t;

	t = (t_triangle *)obj->data;
	free(t->vertex);
	free(obj->data);
	free(obj);
}

t_object	*new_plane(float3 *poi_nor, float specular, t_color color, \
						float *rotation)
{
	t_plane		*new_plane;
	t_object	*new_object;
	float		**matrix;

	new_object = protected_malloc(sizeof(t_object), 1);
	new_plane = protected_malloc(sizeof(t_plane), 1);
	new_plane->normal = poi_nor[1];
	normalize_vector(&new_plane->normal);
	new_plane->point = poi_nor[0];
	new_object->rotation[0] = rotation[0];
	new_object->rotation[1] = rotation[1];
	new_object->rotation[2] = rotation[2];
	matrix = get_rotation_matrix(new_object->rotation);
	transform(&new_plane->normal, matrix, 1);
	matr_free(matrix, 4);
	new_object->specular = specular;
	new_object->color = color;
	new_object->data = (void *)new_plane;
	new_object->tag = "plane";
	new_object->type = PLANE;
	new_plane->d = -new_plane->normal.x * new_plane->point.x - new_plane->\
	normal.y * new_plane->point.y - new_plane->normal.z * new_plane->point.z;
	new_object->clear_obj = &clear_default;
	new_object->intersect = &intersect_ray_plane;
	return (new_object);
}

t_object	*new_cylinder(float3 *pos_vec, float *rad_spec, t_color color, \
							float *rotation)
{
	t_cylinder	*new_cylinder;
	t_object	*new_object;
	float		**matrix;

	new_object = protected_malloc(sizeof(t_object), 1);
	new_cylinder = protected_malloc(sizeof(t_cylinder), 1);
	new_cylinder->position = pos_vec[0];
	new_cylinder->radius = rad_spec[0];
	new_cylinder->vec = pos_vec[1];
	new_object->specular = rad_spec[1];
	new_object->rotation[0] = rotation[0];
	new_object->rotation[1] = rotation[1];
	new_object->rotation[2] = rotation[2];
	matrix = get_rotation_matrix(new_object->rotation);
	transform(&new_cylinder->vec, matrix, 1);
	matr_free(matrix, 4);
	new_object->color = color;
	new_object->data = (void *)new_cylinder;
	new_object->tag = "cylinder";
	new_object->type = CYLINDER;
	new_object->clear_obj = &clear_default;
	new_object->intersect = &intersect_ray_cylinder;
	return (new_object);
}

t_object	*new_cone(float3 *pos_vec, float *ang_spec, t_color color, \
						float *rotation)
{
	t_cone		*new_cone;
	t_object	*new_object;
	float		**matrix;

	new_object = malloc(sizeof(t_object));
	new_cone = malloc(sizeof(t_cone));
	new_cone->position = pos_vec[0];
	new_cone->vec = pos_vec[1];
	new_cone->angle = ang_spec[0];
	new_object->rotation[0] = rotation[0];
	new_object->rotation[1] = rotation[1];
	new_object->rotation[2] = rotation[2];
	matrix = get_rotation_matrix(new_object->rotation);
	transform(&new_cone->vec, matrix, 1);
	matr_free(matrix, 4);
	new_object->specular = ang_spec[1];
	new_object->color = color;
	new_object->data = (void *)new_cone;
	new_object->tag = "cone";
	new_object->type = CONE;
	new_object->intersect = &intersect_ray_cone;
	new_object->clear_obj = &clear_default;
	return (new_object);
}

t_object	*new_ellipsoid(float3 *pos_vec, float *abc, t_color color, \
						float *rotation)
{
	t_object	*new_object;
	t_ellipsoid *new_el;
	new_el = (t_ellipsoid *)malloc(sizeof(t_ellipsoid));
	new_el->a = abc[0];
	new_el->b = abc[1];
	new_el->c = abc[2];
	new_object->rotation[0] = rotation[0];
	new_object->rotation[1] = rotation[1];
	new_object->rotation[2] = rotation[2];
	new_el->center = pos_vec[0];
	new_object->data = (void *)new_el;
	new_object->color.red = 0;
	new_object->color.blue = 255;
	new_object->color.green = 0;
	new_object->color.alpha = 255;
	new_object->type = ELLIPSOID;
	new_object->specular = 100;
	new_object->intersect = &intersect_ray_ellipsoid;
	return(new_object);
}