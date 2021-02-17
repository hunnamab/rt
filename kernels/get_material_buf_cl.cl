#include "kernel.h"

float3  change_basis(float3 vec, t_basis basis)
{
    float3  tmp;
	tmp.x = dot(vec, basis.v);
	tmp.y = dot(vec, basis.u);
	tmp.z = dot(vec, basis.w);
    return (tmp);
}

float3  mapping_plane(float3 t, t_object_d obj)
{
    float3 p;
    t -= obj.primitive.plane.point;
    t = change_basis(t, obj.basis);
    t.x /= obj.t_scale;
    t.y /= obj.t_scale;
    t.z /= obj.t_scale;
    p.x = t.x;
    p.y = t.z;
    p.z = 0;
    return (p);
}

float3		mapping_triangle(float3 t, t_object_d obj)
{
	float3 p;

	t -= obj.primitive.triangle.vertex[0];
	t = change_basis(t, obj.basis);
	t.x /= obj.t_scale;
	t.y /= obj.t_scale;
	t.z /= obj.t_scale;
	p.x = t.x;
	p.y = t.z;
	p.z = 0;
	return (p);
}

float3		mapping_cone(float3 t, t_object_d obj)
{
	float3 p;
	float3 tmp;

	t -= obj.primitive.cone.position;
	t = change_basis(t, obj.basis);
	tmp.x = t.x;
	tmp.y = t.z;
	tmp = normalize(tmp);
	float phi = acos(tmp.x) / 1.5707963267948;
	phi = tmp.y > 0 ? 1.f - phi : phi;
	t.x /= obj.t_scale;
	t.y /= obj.t_scale;
	t.z /= obj.t_scale;
	p.x = fabs(-phi);
	p.y = fabs(t.y);
	p.z = 0;
	return (p);
}

float3		mapping_cylinder(float3 t, t_object_d obj)
{
	float3 p;

	t -= obj.primitive.cylinder.position;
	t = change_basis(t, obj.basis);
	float phi = acos(t.x / obj.primitive.cylinder.radius) / 1.5707963267948;
	phi = t.z > 0 ? 1.f - phi : phi;
	t /= obj.t_scale;
	p.x = phi;
	p.y = -t.y;
	p.z = 0;
	return (p);
}

float3		mapping_sphere(float3 t, t_object_d obj)
{
 	float3 p;
	float3 tmp;

	t -= obj.primitive.sphere.center;
	t = change_basis(t, obj.basis);
	t /= obj.t_scale;
	float theta = acos(t.y) / 3.14159265358979;
	tmp.x = t.x;
	tmp.y = t.z;
	tmp = normalize(tmp);
	float phi = acos(tmp.x) / 1.5707963267948;
	phi = t.z > 0 ? 1.f - phi : phi;
	p.x = phi;
	p.y = theta;
	p.z = 0;
    return(p);
}

float3 text_map_select(t_object_d obj, float3 t)
{
    float3 p;

    if (obj.type == SPHERE)
		p = mapping_sphere(t, obj);
 	if (obj.type == PLANE)
		p = mapping_plane(t, obj);
	if (obj.type == CYLINDER)
		p = mapping_cylinder(t, obj);
	if (obj.type == CONE)
		p = mapping_cone(t, obj);
	if (obj.type == TRIANGLE)
		p = mapping_triangle(t, obj);
    return (p);
}

t_color	get_color_tex(__global uchar  *texture, float x, float y, t_object_d obj, int key)
{
 	if (x < 0)
		x = obj.texture_width + x;
	if (y < 0)
		y = obj.texture_height + y;
	int fx = obj.texture_width - (int)(obj.texture_width * x) % obj.texture_width;
	int fy = (int)( obj.texture_height * y) % obj.texture_height;
 	t_color c;
	c.red = texture[obj.texture_id + 4 * fx + obj.l_size * fy + 2];
	c.green = texture[obj.texture_id + 4 * fx + obj.l_size * fy + 1];
	c.blue = texture[obj.texture_id + 4 * fx + obj.l_size * fy];
	c.alpha = texture[obj.texture_id + 4 * fx + obj.l_size * fy + 4];
    return(c);
}

void		set_color_zero(__private t_color *color)
{
	color->red = 0;
	color->green = 0;
	color->blue = 0;
	color->alpha = 0;
}

void		copy_color(__private t_color *dst, __private t_color *src)
{
	dst->red = src->red;
	dst->blue = src->blue;
	dst->green = src->green;
	dst->alpha = src->alpha;
}

__kernel void    get_material_buf_cl(__global uchar *texture_data,\
                                     __global t_object_d *obj,\
                                    __global int *index_buf, \
                                    __global float3 *intersection_buf,\
                                    __global t_material *material_buf, \
									int bounce_cnt, __global int *orig_index_buf, \
									int is_refractive)
{
    int i = get_global_id(0);
	float3 t;
    if (index_buf[i] != -1)
	{
		if (obj[index_buf[i]].texture_id != -1)
		{
			t = text_map_select(obj[index_buf[i]], intersection_buf[i]);
			material_buf[i].color = get_color_tex(texture_data, t.x, t.y, obj[index_buf[i]], i);
		}
		else
			material_buf[i].color = obj[index_buf[i]].color;
		material_buf[i].specular = obj[index_buf[i]].specular;
		material_buf[i].reflection = obj[index_buf[i]].reflection;
		if (bounce_cnt == 0 && !is_refractive)
			material_buf[i].refraction = obj[index_buf[i]].refraction; // ???
		material_buf[i].transparency = obj[orig_index_buf[i]].transparency;
    }
 	else
	{
		if (bounce_cnt == 0)
		{
			material_buf[i].color.red = 0;
			material_buf[i].color.green = 0;
			material_buf[i].color.blue = 0;
			material_buf[i].color.alpha = 0;
			material_buf[i].specular = -1;
			material_buf[i].reflection = 0.0;
			material_buf[i].refraction = 0.0;
			material_buf[i].transparency = 0.0;
		}
		material_buf[i].reflection = 0.0;
		//material_buf[i].refraction = 0.0;
		material_buf[i].transparency = 0.0;
	}
}
