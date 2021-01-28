typedef struct 		s_color
{
	uchar	red;
	uchar	green;
	uchar	blue;
	uchar	alpha;
}					t_color;

__kernel void magic(__global t_color *pixels, __global t_color *buffer)
{
    int i = get_global_id(0);

    float matrix[9] = {-1.0f, -1.0f, -1.0f, -1.0f, 9.0f, -1.0f, -1.0f, -1.0f, -1.0f};

     pixels[i].red = buffer[i - 721].red * matrix[0] + \
                    buffer[i - 720].red * matrix[1] + \
                    buffer[i - 719].red * matrix[2] + \
                    buffer[i - 1].red * matrix[3] + \
                    buffer[i].red * matrix[4] + \
                    buffer[i + 1].red * matrix[5] + \
                    buffer[i + 719].red * matrix[6] + \
                    buffer[i + 720].red * matrix[7] + \
                    buffer[i + 721].red * matrix[8];
    pixels[i].green = buffer[i - 721].green * matrix[0] + \
                    buffer[i - 720].green * matrix[1] + \
                    buffer[i - 719].green * matrix[2] + \
                    buffer[i - 1].green * matrix[3] + \
                    buffer[i].green * matrix[4] + \
                    buffer[i + 1].green * matrix[5] + \
                    buffer[i + 719].green * matrix[6] + \
                    buffer[i + 720].green * matrix[7] + \
                    buffer[i + 721].green * matrix[8];
    pixels[i].blue = buffer[i - 721].blue * matrix[0] + \
                    buffer[i - 720].blue * matrix[1] + \
                    buffer[i - 719].blue * matrix[2] + \
                    buffer[i - 1].blue * matrix[3] + \
                    buffer[i].blue * matrix[4] + \
                    buffer[i + 1].blue * matrix[5] + \
                    buffer[i + 719].blue * matrix[6] + \
                    buffer[i + 720].blue * matrix[7] + \
                    buffer[i + 721].blue * matrix[8];
}