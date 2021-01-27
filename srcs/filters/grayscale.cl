typedef struct 		s_color
{
	uchar	red;
	uchar	green;
	uchar	blue;
	uchar	alpha;
}					t_color;

__kernel void grayscale(__global t_color *pixels)
{
    int i = get_global_id(0);
    uchar result;

    result = pixels[i].red * 0.3f +pixels[i].green * 0.59f +pixels[i].blue * 0.11f;
    pixels[i].red = result;
    pixels[i].green = result;
    pixels[i].blue = result;
}