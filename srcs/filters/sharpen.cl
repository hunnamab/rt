typedef struct 		s_color
{
	uchar	red;
	uchar	green;
	uchar	blue;
	uchar	alpha;
}					t_color;

__kernel void sharpen(__global t_color *pixels, __global t_color *buffer, int num)
{
    int i = get_global_id(0);
    int check = 0;
    t_color buf_y;
    float matrix2[9] = {-1.0f, -2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 1.0f};
    float matrix[9] = {-1.0f, 0.0f, 1.0f, -2.0f, 0.0f, 2.0f, -1.0f, 0.0f, 1.0f};
    uchar result;
    result = buffer[i].red * 0.3f + buffer[i].green * 0.59f + buffer[i].blue * 0.11f;
    buffer[i].red = result;
    buffer[i].green = result;
    buffer[i].blue = result;
        check = buffer[i - 1281].red * matrix[0] + \
                    buffer[i - 1280].red * matrix[1] + \
                    buffer[i - 1279].red * matrix[2] + \
                    buffer[i - 1].red * matrix[3] + \
                    buffer[i].red * matrix[4] + \
                    buffer[i + 1].red * matrix[5] + \
                    buffer[i + 1279].red * matrix[6] + \
                    buffer[i + 1280].red * matrix[7] + \
                    buffer[i + 1281].red * matrix[8];
        pixels[i].red = check > 255 ? 255 : check;
        check = buffer[i - 1281].green * matrix[0] + \
                    buffer[i - 1280].green * matrix[1] + \
                    buffer[i - 1279].green * matrix[2] + \
                    buffer[i - 1].green * matrix[3] + \
                    buffer[i].green * matrix[4] + \
                    buffer[i + 1].green * matrix[5] + \
                    buffer[i + 1279].green * matrix[6] + \
                    buffer[i + 1280].green * matrix[7] + \
                    buffer[i + 1281].green * matrix[8];
         pixels[i].green = check > 255 ? 255 : check;
        check = buffer[i - 1281].blue * matrix[0] + \
                    buffer[i - 1280].blue * matrix[1] + \
                    buffer[i - 1279].blue * matrix[2] + \
                    buffer[i - 1].blue * matrix[3] + \
                    buffer[i].blue * matrix[4] + \
                    buffer[i + 1].blue * matrix[5] + \
                    buffer[i + 1279].blue * matrix[6] + \
                    buffer[i + 1280].blue * matrix[7] + \
                    buffer[i + 1281].blue * matrix[8];
        pixels[i].blue = check > 255 ? 255 : check;
        check = buffer[i - 1281].red * matrix2[0] + \
                    buffer[i - 1280].red * matrix2[1] + \
                    buffer[i - 1279].red * matrix2[2] + \
                    buffer[i - 1].red * matrix2[3] + \
                    buffer[i].red * matrix2[4] + \
                    buffer[i + 1].red * matrix2[5] + \
                    buffer[i + 1279].red * matrix2[6] + \
                    buffer[i + 1280].red * matrix2[7] + \
                    buffer[i + 1281].red * matrix2[8];
       buf_y.red = check > 255 ? 255 : check;
        check = buffer[i - 1281].green * matrix2[0] + \
                    buffer[i - 1280].green * matrix2[1] + \
                    buffer[i - 1279].green * matrix2[2] + \
                    buffer[i - 1].green * matrix2[3] + \
                    buffer[i].green * matrix2[4] + \
                    buffer[i + 1].green * matrix2[5] + \
                    buffer[i + 1279].green * matrix2[6] + \
                    buffer[i + 1280].green * matrix2[7] + \
                    buffer[i + 1281].green * matrix2[8];
       buf_y.green = check > 255 ? 255 : check;
        check = buffer[i - 1281].blue * matrix2[0] + \
                    buffer[i - 1280].blue * matrix2[1] + \
                    buffer[i - 1279].blue * matrix[2] + \
                    buffer[i - 1].blue * matrix2[3] + \
                    buffer[i].blue * matrix2[4] + \
                    buffer[i + 1].blue * matrix2[5] + \
                    buffer[i + 1279].blue * matrix2[6] + \
                    buffer[i + 1280].blue * matrix2[7] + \
                    buffer[i + 1281].blue * matrix2[8];
        buf_y.blue = check > 255 ? 255 : check;
    float sq = buf_y.red * buf_y.red + buffer[i].red * buffer[i].red;
    pixels[i].red = sqrt(sq);
    sq = buf_y.green * buf_y.green + buffer[i].green * buffer[i].green;
    pixels[i].green = sqrt(sq);
    sq = buf_y.blue * buf_y.blue + buffer[i].blue * buffer[i].blue;
    pixels[i].blue = sqrt(sq);
}