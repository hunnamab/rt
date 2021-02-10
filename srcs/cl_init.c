#include "rt.h"

void	device_objects_init(t_scene *scene)
{
	t_object_d *buf;
	buf = (t_object_d *)malloc(sizeof(t_object_d) * scene->obj_nmb);
	int i = 0;
	int shift = 0;
	int l = 0;

	while (i < scene->obj_nmb)
	{
		if (scene->objs[i]->type == SPHERE)
		{
			t_sphere *s;
			s = (t_sphere *)scene->objs[i]->data;
			buf[i].type = SPHERE;
			buf[i].primitive.sphere.center = s->center;
			buf[i].primitive.sphere.radius = s->radius;
		}
		if (scene->objs[i]->type == CONE)
		{
			t_cone *cone;
			cone = (t_cone *)scene->objs[i]->data;
			buf[i].type = CONE;
			buf[i].primitive.cone.angle = cone->angle;
			buf[i].primitive.cone.position = cone->position;
			buf[i].primitive.cone.vec = cone->vec;
		}
		if (scene->objs[i]->type == CYLINDER)
		{
			t_cylinder *cyl;
			cyl = (t_cylinder *)scene->objs[i]->data;
			buf[i].type = CYLINDER;
			buf[i].primitive.cylinder.position.x = cyl->position.x;
			buf[i].primitive.cylinder.position.y = cyl->position.y;
			buf[i].primitive.cylinder.position.z = cyl->position.z;
			buf[i].primitive.cylinder.radius = cyl->radius;
			buf[i].primitive.cylinder.vec = cyl->vec;
		}
		if (scene->objs[i]->type == TRIANGLE)
		{
			t_triangle *t;
			t = (t_triangle *)scene->objs[i]->data;
			buf[i].type = TRIANGLE;
			buf[i].primitive.triangle.normal = t->normal;
			buf[i].primitive.triangle.vertex[0] = t->vertex[0];
			buf[i].primitive.triangle.vertex[1] = t->vertex[1];
			buf[i].primitive.triangle.vertex[2] = t->vertex[2];
		}
		if (scene->objs[i]->type == PLANE)
		{
			t_plane *p;
			p = (t_plane *)scene->objs[i]->data;
			buf[i].type = PLANE;
			buf[i].primitive.plane.normal = p->normal;
			buf[i].primitive.plane.point = p->point;
			buf[i].primitive.plane.d = p->d;
		}
		if (scene->objs[i]->type == ELLIPSOID)
		{
			t_ellipsoid *el;
			el = (t_ellipsoid *)scene->objs[i]->data;
			buf[i].type = ELLIPSOID;
			buf[i].primitive.ellipsoid.center = el->center;
			buf[i].primitive.ellipsoid.a = el->a;
			buf[i].primitive.ellipsoid.b = el->b;
			buf[i].primitive.ellipsoid.c = el->c;
		}
		if (scene->objs[i]->type == BOX)
		{
			t_box *box;
			box = (t_box *)scene->objs[i]->data;
			buf[i].type = BOX;
			buf[i].primitive.box.a = box->a;
			buf[i].primitive.box.b = box->b;
		}
		if (scene->objs[i]->type == PARABOLOID)
		{
			t_paraboloid *parab;
			parab = (t_paraboloid *)scene->objs[i]->data;
			buf[i].type = PARABOLOID;
			buf[i].primitive.paraboloid.center = parab->center;
			buf[i].primitive.paraboloid.k = parab->k;
		}
		if (scene->objs[i]->type == TORUS)
		{
			t_torus	*torus;
			torus = (t_torus *)scene->objs[i]->data;
			buf[i].type = TORUS;
			buf[i].primitive.torus.center = torus->center;
			buf[i].primitive.torus.radius1 = torus->radius1;
			buf[i].primitive.torus.radius2 = torus->radius2;
		}
		if (scene->objs[i]->type == HYPERBOLOID)
		{
			t_hyperboloid	*hyperboloid;
			hyperboloid = (t_hyperboloid *)scene->objs[i]->data;
			buf[i].type = HYPERBOLOID;
			buf[i].primitive.hyperboloid.center = hyperboloid->center;
			buf[i].primitive.hyperboloid.a = hyperboloid->a;
			buf[i].primitive.hyperboloid.b = hyperboloid->b;
			buf[i].primitive.hyperboloid.c = hyperboloid->c;
		}
		if (scene->objs[i]->text != NULL)
        {
			buf[i].texture_id = scene->objs[i]->texture_id;
			buf[i].texture_size = scene->objs[i]->text->size;
			buf[i].texture_width = scene->objs[i]->text->width;
			buf[i].texture_height =  scene->objs[i]->text->height;
			buf[i].l_size = scene->objs[i]->text->l_size;
		}
		else
		{
			buf[i].texture_id = -1;
			buf[i].texture_size = -1;
		}
		if(scene->objs[i]->normal_text != NULL)
		{
			buf[i].normal_map_id = scene->objs[i]->normal_map_id;
			buf[i].texture_size_nm = scene->objs[i]->normal_text->size;
			buf[i].texture_width_nm = scene->objs[i]->normal_text->width;
			buf[i].texture_height_nm = scene->objs[i]->normal_text->height;
			buf[i].l_size_nm = scene->objs[i]->normal_text->l_size;
		}
		else
		{
			buf[i].normal_map_id = -1;
			buf[i].texture_size_nm = -1;
		}
		buf[i].rotation = get_point(0,0,0);
		buf[i].specular = scene->objs[i]->specular;
		buf[i].color = scene->objs[i]->color;
		buf[i].roughness = 0;
		buf[i].color_disrupt = 0;
		buf[i].refraction = scene->objs[i]->refraction;
		buf[i].reflection = scene->objs[i]->reflection;
		if (scene->objs[i]->t_scale)
			buf[i].t_scale = scene->objs[i]->t_scale;
		else
			buf[i].t_scale = 1;
		buf[i].basis = scene->objs[i]->basis;
		i++;
	}
	scene->cl_data.scene.obj = clCreateBuffer(scene->cl_data.context, CL_MEM_READ_ONLY |
		CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(t_object_d) * scene->obj_nmb, buf, NULL);
	printf("t_object_d host = %lu\n", sizeof(t_object_d));
}

int    cl_init(t_scene *scene)
{
	int err;
	unsigned int correct; // number of correct results returned
	unsigned int count = WID * HEI;
	size_t global; // global domain size for our calculation
	size_t local; // local domain size for our calculation
	int i;
	err = 0;
    cl_uint nP;
    cl_uint status = clGetPlatformIDs(0, NULL, &nP);
    cl_platform_id pfs;
    status = clGetPlatformIDs(nP, &pfs, NULL);
    size_t size;
    char *str;
    clGetPlatformInfo(pfs, CL_PLATFORM_NAME, 0, NULL, &size);
    str = malloc(sizeof(char) * size);
    clGetPlatformInfo(pfs, CL_PLATFORM_NAME, size, str, NULL);
    printf("%s\n", str);
	err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &scene->cl_data.device_id, NULL);

	// выделение памяти
	scene->cl_data.programs = malloc(sizeof(cl_program) * KERNEL_NUM);
	scene->cl_data.kernels = malloc(sizeof(cl_kernel) * KERNEL_NUM);
	scene->cl_data.context = clCreateContext(0, 1, &scene->cl_data.device_id, NULL, NULL, &err);
	scene->cl_data.commands = clCreateCommandQueue(scene->cl_data.context, scene->cl_data.device_id, 0, &err);

	int		ret1;
	char	*get_ray_arr;
	int fd1 = open("./kernels/get_ray_arr.cl", O_RDONLY);
	get_ray_arr = protected_malloc(sizeof(char), 256000);
	ret1 = read(fd1, get_ray_arr, 64000);
	get_ray_arr[ret1] = '\0';

	if ((scene->cl_data.programs[0] = clCreateProgramWithSource(scene->cl_data.context, 1, (const char **)&get_ray_arr, NULL, &err)))
		printf("created\n");
	if ((clBuildProgram(scene->cl_data.programs[0], 1, &scene->cl_data.device_id, "-I includes", NULL, &err)))
		printf("built\n");
	if (!(scene->cl_data.kernels[0] = clCreateKernel(scene->cl_data.programs[0], "get_ray_arr", &err)))
		printf("error %d\n", err);
	ft_strdel(&get_ray_arr);
	close(fd1);

	char info[1024];
	clGetDeviceInfo(scene->cl_data.device_id, CL_DEVICE_NAME, 1024, info, NULL);
	printf("%s\n", info);
	
	int		ret2;
	char	*intersect_ray_sphere_cl;
	int fd2 = open("./kernels/intersect_ray_sphere_cl.cl", O_RDONLY);
	intersect_ray_sphere_cl = protected_malloc(sizeof(char), 256000);
	ret2 = read(fd2, intersect_ray_sphere_cl, 64000);
	intersect_ray_sphere_cl[ret2] = '\0';
	
	if ((scene->cl_data.programs[1] = clCreateProgramWithSource(scene->cl_data.context, 1, (const char **)&intersect_ray_sphere_cl, NULL, &err)))
		printf("cоздана программа sphere\n");
	if ((clBuildProgram(scene->cl_data.programs[1], 0, NULL, "-I includes", NULL, &err)))
		printf("собрана программа sphere err == %lu\n", err);
	if (!(scene->cl_data.kernels[1] = clCreateKernel(scene->cl_data.programs[1], "intersect_ray_sphere_cl", &err)))
		printf("не собрана программа 1, error %d sphere\n", err);
	ft_strdel(&intersect_ray_sphere_cl);
	close(fd2);

	int		ret3;
	char	*intersect_ray_cone_cl;
	int fd3 = open("./kernels/intersect_ray_cone_cl.cl", O_RDONLY);
	intersect_ray_cone_cl = protected_malloc(sizeof(char), 256000);
	ret3 = read(fd3, intersect_ray_cone_cl, 64000);
	intersect_ray_cone_cl[ret3] = '\0';
	
	if ((scene->cl_data.programs[2] = clCreateProgramWithSource(scene->cl_data.context, 1, (const char **)&intersect_ray_cone_cl, NULL, &err)))
		printf("cоздана программа cone\n");
	if ((clBuildProgram(scene->cl_data.programs[2], 0, NULL, "-I includes", NULL, &err)))
		printf("собрана программа cone\n");
	if (!(scene->cl_data.kernels[2] = clCreateKernel(scene->cl_data.programs[2], "intersect_ray_cone_cl", &err)))
		printf("не собрана программа 1, error %d cone\n", err);
	ft_strdel(&intersect_ray_cone_cl);
	close(fd3);

	int		ret4;
	char	*intersect_ray_cylinder_cl;
	int fd4 = open("./kernels/intersect_ray_cylinder_cl.cl", O_RDONLY);
	intersect_ray_cylinder_cl = protected_malloc(sizeof(char), 256000);
	ret4 = read(fd4, intersect_ray_cylinder_cl, 64000);
	intersect_ray_cylinder_cl[ret4] = '\0';
	
	if ((scene->cl_data.programs[3] = clCreateProgramWithSource(scene->cl_data.context, 1, (const char **)&intersect_ray_cylinder_cl, NULL, &err)))
		printf("cоздана программа cylinder\n");
	if ((clBuildProgram(scene->cl_data.programs[3], 0, NULL, "-I includes", NULL, &err)))
		printf("собрана программа cylinder\n");
	if (!(scene->cl_data.kernels[3] = clCreateKernel(scene->cl_data.programs[3], "intersect_ray_cylinder_cl", &err)))
		printf("не собрана программа 1, error %d cylinder\n", err);
	ft_strdel(&intersect_ray_cylinder_cl);
	close(fd4);

	int		ret5;
	char	*intersect_ray_triangle_cl;
	int fd5 = open("./kernels/intersect_ray_triangle_cl.cl", O_RDONLY);
	intersect_ray_triangle_cl = protected_malloc(sizeof(char), 256000);
	ret5 = read(fd5, intersect_ray_triangle_cl, 64000);
	intersect_ray_triangle_cl[ret5] = '\0';
	
	if ((scene->cl_data.programs[4] = clCreateProgramWithSource(scene->cl_data.context, 1, (const char **)&intersect_ray_triangle_cl, NULL, &err)))
		printf("cоздана программа triangle\n");
	if ((clBuildProgram(scene->cl_data.programs[4], 0, NULL, "-I includes", NULL, &err)))
		printf("собрана программа triangle\n");
	if (!(scene->cl_data.kernels[4] = clCreateKernel(scene->cl_data.programs[4], "intersect_ray_triangle_cl", &err)))
		printf("не собрана программа 1, error %d triangle\n", err);
	ft_strdel(&intersect_ray_triangle_cl);
	close(fd5);

	int		ret6;
	char	*intersect_ray_plane_cl;
	int fd6 = open("./kernels/intersect_ray_plane_cl.cl", O_RDONLY);
	intersect_ray_plane_cl = protected_malloc(sizeof(char), 256000);
	ret6 = read(fd6, intersect_ray_plane_cl, 64000);
	intersect_ray_plane_cl[ret6] = '\0';
	
	if ((scene->cl_data.programs[5] = clCreateProgramWithSource(scene->cl_data.context, 1, (const char **)&intersect_ray_plane_cl, NULL, &err)))
		printf("cоздана программа plane\n");
	if ((clBuildProgram(scene->cl_data.programs[5], 0, NULL, "-I includes", NULL, &err)))
		printf("собрана программа plane\n");
	if (!(scene->cl_data.kernels[5] = clCreateKernel(scene->cl_data.programs[5], "intersect_ray_plane_cl", &err)))
		printf("не собрана программа 1, error %d plane\n", err);
	ft_strdel(&intersect_ray_plane_cl);
	close(fd6);

	int		ret7;
	char	*get_intersection_point;
	int fd7 = open("./kernels/get_intersection_point_cl.cl", O_RDONLY);
	get_intersection_point = protected_malloc(sizeof(char), 256000);
	ret7 = read(fd7, get_intersection_point, 64000);
	get_intersection_point[ret7] = '\0';
	
	if ((scene->cl_data.programs[6] = clCreateProgramWithSource(scene->cl_data.context, 1, (const char **)&get_intersection_point, NULL, &err)))
		printf("cоздана программа get_intersection_point\n");
	if ((clBuildProgram(scene->cl_data.programs[6], 0, NULL, "-I includes", NULL, &err)))
		printf("собрана программа get_intersection_point\n");
	if (!(scene->cl_data.kernels[6] = clCreateKernel(scene->cl_data.programs[6], "get_intersection_point", &err)))
		printf("не собрана программа 1, error %d get_intersection_point\n", err);
	ft_strdel(&get_intersection_point);
	close(fd7);

	int		ret8;
	char	*get_normal_buf_cl;
	int fd8 = open("./kernels/get_normal_buf_cl.cl", O_RDONLY);
	get_normal_buf_cl = protected_malloc(sizeof(char), 256000);
	ret8 = read(fd8, get_normal_buf_cl, 64000);
	get_normal_buf_cl[ret8] = '\0';
	
	if ((scene->cl_data.programs[7] = clCreateProgramWithSource(scene->cl_data.context, 1, (const char **)&get_normal_buf_cl, NULL, &err)))
		printf("cоздана программа get_normal_buf_cl\n");
	if ((clBuildProgram(scene->cl_data.programs[7], 0, NULL, "-I includes", NULL, &err)))
		printf("собрана программа get_normal_buf_cl\n");
	if (!(scene->cl_data.kernels[7] = clCreateKernel(scene->cl_data.programs[7], "get_normal_buf_cl", &err)))
		printf("не собрана программа 1, error %d get_normal_buf_cl\n", err);
	ft_strdel(&get_normal_buf_cl);
	close(fd8);

	int		ret9;
	char	*intersect_ray_ellipsoid_cl;
	int fd9 = open("./kernels/intersect_ray_ellipsoid_cl.cl", O_RDONLY);
	intersect_ray_ellipsoid_cl = protected_malloc(sizeof(char), 256000);
	ret9 = read(fd9, intersect_ray_ellipsoid_cl, 64000);
	intersect_ray_ellipsoid_cl[ret9] = '\0';
	
	if ((scene->cl_data.programs[8] = clCreateProgramWithSource(scene->cl_data.context, 1, (const char **)&intersect_ray_ellipsoid_cl, NULL, &err)))
		printf("cоздана программа intersect_ray_ellipsoid_cl\n");
	if ((clBuildProgram(scene->cl_data.programs[8], 0, NULL, "-I includes", NULL, &err)))
		printf("собрана программа intersect_ray_ellipsoid_cl\n");
	if (!(scene->cl_data.kernels[8] = clCreateKernel(scene->cl_data.programs[8], "intersect_ray_ellipsoid", &err)))
		printf("не собрана программа 1, error %d intersect_ray_ellipsoid_cl\n", err);
	ft_strdel(&intersect_ray_ellipsoid_cl);
	close(fd9);

	int		ret10;
	char	*get_frame_buf_cl;
	int fd10 = open("./kernels/get_frame_buf_cl.cl", O_RDONLY);
	get_frame_buf_cl = protected_malloc(sizeof(char), 256000);
	ret10 = read(fd10, get_frame_buf_cl, 64000);
	get_frame_buf_cl[ret10] = '\0';
	
	if ((scene->cl_data.programs[9] = clCreateProgramWithSource(scene->cl_data.context, 1, (const char **)&get_frame_buf_cl, NULL, &err)))
		printf("cоздана программа get_frame_buf_cl\n");
	if ((clBuildProgram(scene->cl_data.programs[9], 0, NULL, "-I includes", NULL, &err)))
		printf("собрана программа get_frame_buf_cl\n");
	if (!(scene->cl_data.kernels[9] = clCreateKernel(scene->cl_data.programs[9], "get_frame_buf_cl", &err)))
		printf("не собрана программа 1, error %d get_frame_buf_cl\n", err);
	ft_strdel(&get_frame_buf_cl);
	close(fd10);

	int		ret11;
	char	*get_material_buf_cl;
	int fd11 = open("./kernels/get_material_buf_cl.cl", O_RDONLY);
	get_material_buf_cl = protected_malloc(sizeof(char), 256000);
	ret11 = read(fd11, get_material_buf_cl, 64000);
	get_material_buf_cl[ret11] = '\0';
	
	if ((scene->cl_data.programs[10] = clCreateProgramWithSource(scene->cl_data.context, 1, (const char **)&get_material_buf_cl, NULL, &err)))
		printf("cоздана программа get_material_buf_cl\n");
	if ((clBuildProgram(scene->cl_data.programs[10], 0, NULL, "-I includes", NULL, &err)))
		printf("собрана программа get_material_buf_cl\n");
	if (!(scene->cl_data.kernels[10] = clCreateKernel(scene->cl_data.programs[10], "get_material_buf_cl", &err)))
		printf("не собрана программа 10, error %d get_material_buf_cl\n", err);
	ft_strdel(&get_material_buf_cl);
	close(fd11);

	int		ret12;
	char	*intersect_ray_box_cl;
	int fd12 = open("./kernels/intersect_ray_box_cl.cl", O_RDONLY);
	intersect_ray_box_cl = protected_malloc(sizeof(char), 256000);
	ret12 = read(fd12, intersect_ray_box_cl, 64000);
	intersect_ray_box_cl[ret12] = '\0';
	
	if ((scene->cl_data.programs[11] = clCreateProgramWithSource(scene->cl_data.context, 1, (const char **)&intersect_ray_box_cl, NULL, &err)))
		printf("cоздана программа intersect_ray_box_cl\n");
	if ((clBuildProgram(scene->cl_data.programs[11], 0, NULL, "-I includes", NULL, &err)))
		printf("собрана программа intersect_ray_box_cl\n");
	if (!(scene->cl_data.kernels[11] = clCreateKernel(scene->cl_data.programs[11], "intersect_ray_box", &err)))
		printf("не собрана программа 1, error %d intersect_ray_box_cl\n", err);
	ft_strdel(&intersect_ray_box_cl);
	close(fd12);

	int		ret13;
	char	*intersect_ray_paraboloid_cl;
	int fd13 = open("./kernels/intersect_ray_paraboloid_cl.cl", O_RDONLY);
	intersect_ray_paraboloid_cl = protected_malloc(sizeof(char), 256000);
	ret13 = read(fd13, intersect_ray_paraboloid_cl, 64000);
	intersect_ray_paraboloid_cl[ret13] = '\0';
	
	if ((scene->cl_data.programs[12] = clCreateProgramWithSource(scene->cl_data.context, 1, (const char **)&intersect_ray_paraboloid_cl, NULL, &err)))
		printf("cоздана программа intersect_ray_paraboloid_cl\n");
	if ((clBuildProgram(scene->cl_data.programs[12], 0, NULL, "-I includes", NULL, &err)))
		printf("собрана программа intersect_ray_paraboloid_cl\n");
	if (!(scene->cl_data.kernels[12] = clCreateKernel(scene->cl_data.programs[12], "intersect_ray_paraboloid", &err)))
		printf("не собрана программа 1, error %d intersect_ray_paraboloid_cl\n", err);
	ft_strdel(&intersect_ray_paraboloid_cl);
	close(fd13);

	int		ret14;
	char	*intersect_ray_torus_cl;
	int fd14 = open("./kernels/intersect_ray_torus_cl.cl", O_RDONLY);
	intersect_ray_torus_cl = protected_malloc(sizeof(char), 256000);
	ret14 = read(fd14, intersect_ray_torus_cl, 64000);
	intersect_ray_torus_cl[ret14] = '\0';
	
	if ((scene->cl_data.programs[13] = clCreateProgramWithSource(scene->cl_data.context, 1, (const char **)&intersect_ray_torus_cl, NULL, &err)))
		printf("cоздана программа intersect_ray_torus_cl\n");
	if ((clBuildProgram(scene->cl_data.programs[13], 0, NULL, "-I includes", NULL, &err)))
		printf("собрана программа intersect_ray_torus_cl\n");
	if (!(scene->cl_data.kernels[13] = clCreateKernel(scene->cl_data.programs[13], "intersect_ray_torus", &err)))
		printf("не собрана программа 1, error %d intersect_ray_torus_cl\n", err);
	ft_strdel(&intersect_ray_torus_cl);
	close(fd14);

	int		ret15;
	char	*intersect_ray_hyperboloid_cl;
	int fd15 = open("./kernels/intersect_ray_hyperboloid_cl.cl", O_RDONLY);
	intersect_ray_hyperboloid_cl = protected_malloc(sizeof(char), 256000);
	ret15 = read(fd15, intersect_ray_hyperboloid_cl, 64000);
	intersect_ray_hyperboloid_cl[ret15] = '\0';
	
	if ((scene->cl_data.programs[14] = clCreateProgramWithSource(scene->cl_data.context, 1, (const char **)&intersect_ray_hyperboloid_cl, NULL, &err)))
		printf("cоздана программа intersect_ray_hyperboloid_cl\n");
	if ((clBuildProgram(scene->cl_data.programs[14], 0, NULL, "-I includes", NULL, &err)))
		printf("собрана программа intersect_ray_hyperboloid_cl\n");
	if (!(scene->cl_data.kernels[14] = clCreateKernel(scene->cl_data.programs[14], "intersect_ray_hyperboloid", &err)))
		printf("не собрана программа 1, error %d intersect_ray_hyperboloid_cl\n", err);
	ft_strdel(&intersect_ray_hyperboloid_cl);
	close(fd15);

	int		ret16;
	char	*get_refraction_ray_cl;
	int fd16 = open("./kernels/get_refraction_ray_cl.cl", O_RDONLY);
	get_refraction_ray_cl = protected_malloc(sizeof(char), 256000);
	ret16 = read(fd16, get_refraction_ray_cl, 64000);
	get_refraction_ray_cl[ret16] = '\0';
	
	if ((scene->cl_data.programs[15] = clCreateProgramWithSource(scene->cl_data.context, 1, (const char **)&get_refraction_ray_cl, NULL, &err)))
		printf("cоздана программа get_refraction_ray_cl\n");
	if ((clBuildProgram(scene->cl_data.programs[15], 0, NULL, "-I includes", NULL, &err)))
		printf("собрана программа get_refraction_ray_cl\n");
	if (!(scene->cl_data.kernels[15] = clCreateKernel(scene->cl_data.programs[15], "get_refraction_ray_cl", &err)))
		printf("не собрана программа 1, error %d get_refraction_ray_cl\n", err);
	ft_strdel(&get_refraction_ray_cl);
	close(fd16);

	int		ret17;
	char	*get_reflection_ray_cl;
	int fd17 = open("./kernels/get_reflection_ray_cl.cl", O_RDONLY);
	get_reflection_ray_cl = protected_malloc(sizeof(char), 256000);
	ret17 = read(fd17, get_reflection_ray_cl, 64000);
	get_reflection_ray_cl[ret17] = '\0';
	
	if ((scene->cl_data.programs[16] = clCreateProgramWithSource(scene->cl_data.context, 1, (const char **)&get_reflection_ray_cl, NULL, &err)))
		printf("cоздана программа get_reflection_ray_cl\n");
	if ((clBuildProgram(scene->cl_data.programs[16], 0, NULL, "-I includes", NULL, &err)))
		printf("собрана программа get_reflection_ray_cl\n");
	if (!(scene->cl_data.kernels[16] = clCreateKernel(scene->cl_data.programs[16], "get_reflection_ray_cl", &err)))
		printf("не собрана программа 1, error %d get_reflection_ray_cl\n", err);
	ft_strdel(&get_reflection_ray_cl);
	close(fd17);

	//Создание буферов на гпу
	scene->cl_data.scene.ray_buf = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(cl_float3) * count, NULL, NULL);
	scene->cl_data.scene.viewport = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(cl_float3) * count, NULL, NULL);
	scene->cl_data.scene.intersection_buf = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(cl_float3) * count, NULL, NULL);
	scene->cl_data.scene.copy_intersec_buf = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(cl_float3) * count, NULL, NULL);
	scene->cl_data.scene.index_buf = clCreateBuffer(scene->cl_data.context,  0,  sizeof(int) * count, NULL, NULL);
	scene->cl_data.scene.orig_index_buf = clCreateBuffer(scene->cl_data.context,  0,  sizeof(int) * count, NULL, NULL);
	scene->cl_data.scene.exception_buf = clCreateBuffer(scene->cl_data.context,  0,  sizeof(int) * count, NULL, NULL);
	scene->cl_data.scene.depth_buf = clCreateBuffer(scene->cl_data.context,  0,  sizeof(float) * count, NULL, NULL);
	scene->cl_data.scene.normal_buf = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(cl_float3) * count, NULL, NULL);
	scene->cl_data.scene.copy_normal_buf = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(cl_float3) * count, NULL, NULL);
	scene->cl_data.scene.frame_buf = clCreateBuffer(scene->cl_data.context, CL_MEM_READ_WRITE, sizeof(t_color) * count, NULL, NULL);
	scene->cl_data.scene.light = clCreateBuffer(scene->cl_data.context, CL_MEM_READ_ONLY |
		CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(t_light) * scene->light_nmb, scene->light, NULL);
	scene->cl_data.scene.material_buf = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(t_material) * count, NULL, NULL);
	scene->cl_data.scene.prev_material_buf = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(t_material) * count, NULL, NULL);
	//передача указателей на переменные опенсл в тип, отвечающий за фильтры
	scene->filter_data.commands = scene->cl_data.commands;
	scene->filter_data.device_id = scene->cl_data.device_id;
	scene->filter_data.context = scene->cl_data.context;
	scene->filter_data.pixels = scene->cl_data.scene.frame_buf;
	load_textures(scene);
	device_objects_init(scene);
	clFinish(scene->cl_data.commands);
	return (0);
}
