typedef struct 		s_color
{
	uchar	red;
	uchar	green;
	uchar	blue;
	uchar	alpha;
}					t_color;

__kernel void sepia(__global t_color *pixels)
{
    int i = get_global_id(0);
    t_color buf;
    int check;

    buf.red = pixels[i].red;
    buf.green = pixels[i].green;
    buf.blue = pixels[i].blue;
    check = buf.red * 0.393 + buf.green * 0.769 + buf.blue * 0.189;
    pixels[i].red = check > 255 ? 255 : check;
    check = buf.red * 0.349 + buf.green * 0.686 + buf.blue * 0.168;
    pixels[i].green = check > 255 ? 255 : check;
    check = buf.red * 0.272 + buf.green * 0.534 + buf.blue * 0.131;
    pixels[i].blue = check > 255 ? 255 : check;
}