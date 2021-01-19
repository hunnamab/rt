#include "kernel.h"

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
	float theta = acos(t.y) / M_PI;
	tmp.x = t.x;
	tmp.y = t.z;
	tmp = normalize(tmp);
	//printf("tmp.x %f\n", tmp.x);
	double phi = acos(tmp.x) / M_PI_2;
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
	/* if (ft_strequ(obj->tag, "plane"))
		p = mapping_plane(t, obj);
	if (ft_strequ(obj->tag, "cylinder"))
		p = mapping_cylinder(t, obj);
	if (ft_strequ(obj->tag, "cone"))
		p = mapping_cone(t, obj);
	if (ft_strequ(obj->tag, "triangle"))
		p = mapping_triangle(t, obj);*/
    return (p);
}

t_color	get_color_tex(__global char  *texture, float x, float y, t_object_d obj)
{
 	if (x < 0)
		x = obj.texture_width + x;
	if (y < 0)
		y =  obj.texture_height + y;
	int fx =  obj.texture_width - (int)(obj.texture_width * x) % obj.texture_width;
	int fy = (int)( obj.texture_height * y) %  obj.texture_height;
    int index = obj.texture_id + 32 * fx + obj.l_size * fy;
    printf("t_id %d  l_size %d, fx fy %d %d\n", obj.texture_id, obj.l_size, fx, fy);
    uint p = *(uint *)(((uchar *)texture[index]));
 	t_color c;
    c.red = p >> 16;
    c.green = p >> 8;
    c.blue = p;
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

__kernel void    get_material_buf_cl(__global char *texture_data,\
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
			material_buf[i].color = get_color_tex(texture_data, t.x, t.y, obj[index_buf[i]]);
		}
		else
			material_buf[i].color = obj[index_buf[i]].color; 
		material_buf[i].specular = obj[index_buf[i]].specular;
    }
 	/* else
	{
		set_color_zero(&material_buf[i].color);
		material_buf[i].specular = -1;
	} */
}