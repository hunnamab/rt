#include "kernel.h"

/* float3 		change_basis(float3 vec)
{
	float3
	tmp;

	tmp.x = vec.x * 1.0 + vec.y * 0.0 + vec.z * 0.0;
	tmp.y = vec.x * 0.0 + vec.y * 1.0 + vec.z * 0.0;
	tmp.z = vec.x * 0.0 + vec.y * 0.0 + vec.z * 1.0;
	return (tmp);
}


float3 	mapping_plane(float3 t, t_object obj)
{
	float3 p;

	p.x = fabs(fmod(t.x, 1.0));
	p.y = fabs(fmod(t.z, 1.0));
	return (p);
}

float3		mapping_triangle(float3 t, t_object obj)
{
	float3 p;

	t.x -= obj.primitive.tringle.vertex[0].x;
	t.y -= obj.primitive.tringle.vertex[0].y;
	t.z -= obj.primitive.tringle.vertex[0].z;
	t = change_basis(t, obj.basis);
	t.x /= 10;
	t.y /= 10;
	t.z /= 10;
	p.x = (t.x);
	p.y = (t.z);
	p.z = 0;
	return (p);
}

float3		mapping_cone(float3 t, t_object obj)
{
	float3 p;
	float3 tmp;

	t.x -= obj.primitive.cone.position.x;
	t.y -= obj.primitive.cone.position.y;
	t.z -= obj.primitive.cone.position.z;
	t = change_basis(t, obj.basis);
	tmp.x = t.x;
	tmp.y = t.z;
	tmp = normalize(tmp);
	float phi = acos(tmp.x) / M_PI_2;
	phi = tmp.y > 0 ? 1.f - phi : phi;
	t.x /= 20;
	t.y /= 20;
	t.z /= 20;
	p.x = fabs(-phi);
	p.y = fabs(t.y);
	p.z = 0;
	return (p);
	}
*/
/* float3		mapping_cylinder(float3 t, t_object obj)
{
	float3 p;

	t.x -= obj.primitive.position.cylinder.x;
	t.y -= obj.primitive.position.cylinder.y;
	t.z -= obj.primitive.position.cylinder.z;
	//t = change_basis(t, obj.basis);
	float	phi = acos(t.x / obj.primitive.cylinder.radius) / 1.5707963267948;
	phi = t.z > 0 ? 1.f - phi : phi;
	t.x /= obj.primitive.cylinder.radius;
	t.y /= obj.primitive.cylinder.radius;
	t.z /= obj.primitive.cylinder.radius;	
	p.x = (phi);
	p.y = (-t.y);
	p.z = 0;
	return (p);
} */

float3		mapping_sphere(float3 t, t_object_d obj)
{
 	float3 p;
	float3 tmp;

	//printf("t(%f,%f,%f)\n", t.x, t.y, t.z);
	t.x -= obj.primitive.sphere.center.x;
	t.y -= obj.primitive.sphere.center.y;
	t.z -= obj.primitive.sphere.center.z;
	//t = change_basis(t);
	t.x /= obj.primitive.sphere.radius;
	t.y /= obj.primitive.sphere.radius;
	t.z /= obj.primitive.sphere.radius;
	float theta = acos(t.y) / 3.14159265358979;
	tmp.x = t.x;
	tmp.y = t.z;
	tmp = normalize(tmp);
	//printf("tmp.x %f\n", tmp.x);
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
/* 	if (obj.type == PLANE)
		p = mapping_plane(t, obj);
	if (obj.type == CYLINDER)
		p = mapping_cylinder(t, obj);
	if (obj.type == CONE)
		p = mapping_cone(t, obj);
	if (obj.type == TRIANGLE)
		p = mapping_triangle(t, obj); */
    return (p);
}

t_color	get_color_tex(__global uchar  *texture, float x, float y, t_object_d obj, int key)
{
 	if (x < 0)
		x = obj.texture_width + x;
	if (y < 0)
		y =  obj.texture_height + y;
	int fx =  obj.texture_width - (int)(obj.texture_width * x) % obj.texture_width;
	int fy = (int)( obj.texture_height * y) % obj.texture_height;
 	t_color c;
	c.red = texture[obj.texture_id + 4 * fx + obj.l_size * fy + 2];
	c.green = texture[obj.texture_id + 4 * fx + obj.l_size * fy + 1];
	c.blue = texture[obj.texture_id + 4 * fx + obj.l_size * fy];
	c.alpha = texture[obj.texture_id + 4 * fx + obj.l_size * fy + 4];
	if (key == 1280 * 360 - 640)
	{
		printf("fx %d fy %d l_size %d width %d height %d id %d\n", fx, fy, obj.l_size, obj.texture_width, obj.texture_height, obj.texture_id);
		printf("color (%hhu,%hhu,%hhu)\n", c.red, c.green, c.blue);
		printf("texture device (%hhu,%hhu,%hhu,%hhu)\n", texture[key], texture[key + 1], texture[key + 2], texture[key + 3]);
	}
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
                                    __global *index_buf, \
                                    __global float3 *intersection_buf,\
                                    __global t_material *material_buf)
{
    int i = get_global_id(0);
	float3 t;
/*      if (index_buf[i] != -1)
	{
            printf("t_object_d %lu\n", sizeof(t_object_d));
            printf("obj texture_id %d\n", obj[index_buf[i]].texture_id);
    }  */
    if (index_buf[i] != -1)
	{	
		if (obj[index_buf[i]].texture_id != -1)
		{
			t = text_map_select(obj[index_buf[i]], intersection_buf[i]);
			material_buf[i].color = get_color_tex(texture_data, t.x, t.y, obj[index_buf[i]], i);
			material_buf[i].specular = obj[index_buf[i]].specular;
		}
		else
			material_buf[i].color = obj[index_buf[i]].color; 
    }
 	else
	{
		material_buf[i].color.red = 0;
		material_buf[i].color.green = 0;
		material_buf[i].color.blue = 0;
		material_buf[i].color.alpha = 0;
		material_buf[i].specular = -1;
	}
	if (i == 1280 * 360 - 640)
	{
			printf("material_color device (%hhd,%hhd,%hhd)\n", material_buf[1280 * 360 - 640].color.red,\
		material_buf[1280 * 360 - 640].color.green,material_buf[1280 * 360 - 640].color.blue);
		printf("sizeof(t_material) device %lu\n", sizeof(t_material));
	}
}