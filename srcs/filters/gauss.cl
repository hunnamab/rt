typedef struct 		s_color
{
	uchar	red;
	uchar	green;
	uchar	blue;
	uchar	alpha;
}					t_color;

__kernel void gauss(__global t_color *pixels, __global t_color *buffer, int type)
{
    int i = get_global_id(0);
    float matrix[5] = {0.028087f, 0.23431f, 0.475207f, 0.23431f, 0.028087f};
    if (type == 0)
    {
        pixels[i].red = buffer[i - 2].red * matrix[0] + \
                        buffer[i - 1].red * matrix[1] + \
                        buffer[i].red * matrix[2] + \
                        buffer[i + 1].red * matrix[3] + \
                        buffer[i + 2].red * matrix[4]; 
        pixels[i].blue = buffer[i - 2].blue * matrix[0] + \
                        buffer[i - 1].blue * matrix[1] + \
                        buffer[i].blue * matrix[2] + \
                        buffer[i + 1].blue * matrix[3] + \
                        buffer[i + 2].blue * matrix[4];
        pixels[i].green = buffer[i - 2].green * matrix[0] + \
                        buffer[i - 1].green * matrix[1] + \
                        buffer[i].green * matrix[2] + \
                        buffer[i + 1].green * matrix[3] + \
                        buffer[i + 2].green * matrix[4];
    }
    else
    {
        pixels[i].red = buffer[i - 1440].red * matrix[0] + \
                        buffer[i - 720].red * matrix[1] + \
                        buffer[i].red * matrix[2] + \
                        buffer[i + 720].red * matrix[3] + \
                        buffer[i + 1440].red * matrix[4]; 
        pixels[i].blue = buffer[i - 1440].blue * matrix[0] + \
                        buffer[i - 720].blue * matrix[1] + \
                        buffer[i].blue * matrix[2] + \
                        buffer[i + 720].blue * matrix[3] + \
                        buffer[i + 1440].blue * matrix[4];
        pixels[i].green = buffer[i - 2].green * matrix[0] + \
                        buffer[i - 720].green * matrix[1] + \
                        buffer[i].green * matrix[2] + \
                        buffer[i + 720].green * matrix[3] + \
                        buffer[i + 1440].green * matrix[4];
    }
}