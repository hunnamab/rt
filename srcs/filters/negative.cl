typedef struct 		s_color
{
	uchar	red;
	uchar	green;
	uchar	blue;
	uchar	alpha;
}					t_color;

__kernel void negative(__global t_color *pixels)
{
    int i = get_global_id(0);

    pixels[i].red = 255 - pixels[i].red;
    pixels[i].blue = 255 - pixels[i].blue;
    pixels[i].green = 255 - pixels[i].green;
}