#include "rt.h"
#include "types.h"

void	get_material_buf(t_scene *scene)
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
				if (scene->objs[scene->index_buf[i]]->text != NULL)
				{
					t = text_map_select(scene->objs[scene->index_buf[i]], scene->intersection_buf[i]);
					col = get_color_tex(scene->objs[scene->index_buf[i]]->text, t.x, t.y);
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
	clEnqueueWriteBuffer(scene->cl_data.commands, scene->cl_data.scene.material_buf, CL_FALSE, 0, sizeof(t_material) * global, scene->material_buf, 0, NULL, NULL);
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
