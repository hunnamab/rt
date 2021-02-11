#include "kernel.h"

void  get_normal_hyperboloid(__global t_object_d *obj, \
                        __global float3 *ray_buf, \
                        __global int *index_buf, \
						__global float3 *normal_buf, \
                        __global float3 *intersection_buf, \
						float3 camera_position, \
						__global float *depth_buf)
{
	normal_buf[0].x = intersection_buf[0].x;
	normal_buf[0].y = intersection_buf[0].y;
	normal_buf[0].z = -intersection_buf[0].z;
	normal_buf[0] = normalize(normal_buf[0]);
    if (dot(ray_buf[0], normal_buf[0]) > 0.0001)
	{
		normal_buf[0].x = normal_buf[0].x * -1;
		normal_buf[0].y = normal_buf[0].y * -1;
		normal_buf[0].z = normal_buf[0].z * -1;
	}
}
 
void get_normal_torus(__global t_object_d *obj, \
                        __global float3 *ray_buf, \
                        __global int *index_buf, \
						__global float3 *normal_buf, \
                        __global float3 *intersection_buf, \
						float3 camera_position, \
						__global float *depth_buf)
{
	float3 V = {1.0f, 0.0f, 0.0f}; 
	obj[0].primitive.torus.radius2 = 5;
	obj[0].primitive.torus.radius1 = 10;
	float k = dot((intersection_buf[0] - obj[0].primitive.torus.center), V);
	float3 A =  intersection_buf[0] - V * k;
	float m = sqrt(obj[0].primitive.torus.radius2 * obj[0].primitive.torus.radius2 - k * k);
	float3 normal = intersection_buf[0] - A - (obj[0].primitive.torus.center - A) * m / (obj[0].primitive.torus.radius1 + m);
	normal_buf[0] = normalize(normal);
	if (dot(ray_buf[0], normal_buf[0]) > 0.0001)
    {
		normal_buf[0].x = normal_buf[0].x * -1;
		normal_buf[0].y = normal_buf[0].y * -1;
		normal_buf[0].z = normal_buf[0].z * -1;
	}
}

void  get_normal_paraboloid(__global t_object_d *obj, \
                        __global float3 *ray_buf, \
                        __global int *index_buf, \
						__global float3 *normal_buf, \
                        __global float3 *intersection_buf, \
						float3 camera_position, \
						__global float *depth_buf)
{
	float3 V = obj[0].primitive.paraboloid.vec;
	float3 p_c = intersection_buf[0] - obj[0].primitive.paraboloid.center;
	float m = dot(p_c, V);
	float3 normal = p_c - V * (m + obj[0].primitive.paraboloid.k);
	normal_buf[0] = normalize(normal);
}

void  get_normal_box(__global t_object_d *obj, \
                        __global float3 *ray_buf, \
                        __global int *index_buf, \
						__global float3 *normal_buf, \
                        __global float3 *intersection_buf, \
						float3 camera_position, \
						__global float *depth_buf)
{
	float3 n;
	if (obj[0].primitive.box.face_hit == 0)
	{
		n.x = -1.0f;
		n.y = 0.0f;
		n.z = 0.0f;
		normal_buf[0] = normalize(n);
	}
	else if (obj[0].primitive.box.face_hit == 1)
	{
		n.x = 0.0f;
		n.y = -1.0f;
		n.z = 0.0f;
		normal_buf[0] = normalize(n);
	}
	else if (obj[0].primitive.box.face_hit == 2)
	 {
		n.x = 0.0f;
		n.y = 0.0f;
		n.z = -1.0f;
		normal_buf[0] = normalize(n);
	}
	else if (obj[0].primitive.box.face_hit == 3)
	{
		n.x = 1.0f;
		n.y = 0.0f;
		n.z = 0.0f;
		normal_buf[0] = normalize(n);
	}
	else if (obj[0].primitive.box.face_hit == 4)
	{
		n.x = 0.0f;
		n.y = 1.0f;
		n.z = 0.0f;
		normal_buf[0] = normalize(n);
	}
	else if (obj[0].primitive.box.face_hit == 5)
	{
		n.x = 0.0f;
		n.y = 0.0f;
		n.z = 1.0f;
		normal_buf[0] = normalize(n);
	} 
 	if (dot(ray_buf[0], normal_buf[0]) > 0.0001)
	{
		normal_buf[0].x = normal_buf[0].x * -1.0f;
		normal_buf[0].y = normal_buf[0].y * -1.0f;
		normal_buf[0].z = normal_buf[0].z * -1.0f;
	}
	/* float3 n;
	if (fabs(intersection_buf[0].x - obj[0].primitive.box.a.x) < 0.01f)
	{
		n.x = -1.0f;
		n.y = 0.0f;
		n.z = 0.0f;
	}
	else if (fabs(intersection_buf[0].x - obj[0].primitive.box.b.x) < 0.01f)
	{
		n.x = 1.0f;
		n.y = 0.0f;
		n.z = 0.0f;
	}
	else if (fabs(intersection_buf[0].y - obj[0].primitive.box.a.y) < 0.01f)
	{
		n.x = 0.0f;
		n.y = -1.0f;
		n.z = 0.0f;
	}
	else if (fabs(intersection_buf[0].y - obj[0].primitive.box.b.y) < 0.01f)
	{
		n.x = 0.0f;
		n.y = 1.0f;
		n.z = 0.0f;
	}
	else if (fabs(intersection_buf[0].z - obj[0].primitive.box.a.z) < 0.01f)
	{
		n.x = 0.0f;
		n.y = 0.0f;
		n.z = -1.0f;
	}
	else if (fabs(intersection_buf[0].z - obj[0].primitive.box.b.z) < 0.01f)
	{
		n.x = 0;
		n.y = 0;
		n.z = 1;
	}
	normal_buf[0] = n;//normalize(n); */
 }

void  get_normal_ellipsoid(__global t_object_d *obj, \
                        __global float3 *ray_buf, \
                        __global int *index_buf, \
						__global float3 *normal_buf, \
                        __global float3 *intersection_buf, \
						float3 camera_position, \
						__global float *depth_buf)
{
	float a = obj[0].primitive.ellipsoid.a;
	float b = obj[0].primitive.ellipsoid.b;
	float c = obj[0].primitive.ellipsoid.c;
 	normal_buf[0].x = 2.0f * intersection_buf[0].x / (a * a);
	normal_buf[0].y = 2.0f * intersection_buf[0].y / (b * b);
	normal_buf[0].z = 2.0f * intersection_buf[0].z / (c * c);
	normal_buf[0] = native_divide(normal_buf[0], length(normal_buf[0]));
    if (dot(ray_buf[0], normal_buf[0]) > 0.0001)
	{
		normal_buf[0].x = normal_buf[0].x * -1.0f;
		normal_buf[0].y = normal_buf[0].y * -1.0f;
		normal_buf[0].z = normal_buf[0].z * -1.0f;
	}
}

void  get_normal_cylinder(__global t_object_d *obj, \
                        __global float3 *ray_buf, \
                        __global int *index_buf, \
						__global float3 *normal_buf, \
                        __global float3 *intersection_buf, \
						float3 camera_position, \
						__global float *depth_buf)
{
	float	m;
    float3 buf1;
	float3 buf2;
	float3 p;
	float3 normal;
    buf1 = camera_position - obj[0].primitive.cylinder.position;
    m = dot(ray_buf[0],  obj[0].primitive.cylinder.vec) * depth_buf[0] + dot(buf1, obj[0].primitive.cylinder.vec);
    buf1 = ray_buf[0] * depth_buf[0];
	p = camera_position + buf1;
	buf1 = p - obj[0].primitive.cylinder.position;
	buf2 = obj[0].primitive.cylinder.vec * m;
	normal_buf[0] = buf1 - buf2;
 	normal_buf[0] /= length(normal_buf[0]);
    if (dot(ray_buf[0], normal_buf[0]) > 0.0001)
		normal_buf[0] *= -1.0f;
}

void get_normal_cone(__global t_object_d *obj, \
                        __global float3 *ray_buf, \
                        __global int *index_buf, \
                        __global float3 *normal_buf, \
                        __global float3 *intersection_buf, \
						float3 camera_position, \
						__global float *depth_buf)
{
	float	m;
    float3 buf;
	float n;
    buf = camera_position - obj[0].primitive.cone.position;
    m = dot(ray_buf[0],  obj[0].primitive.cone.vec) * depth_buf[0] + dot(buf, obj[0].primitive.cone.vec);
    buf = obj[0].primitive.cone.vec * m;
	n = 1 +  obj[0].primitive.cone.angle * obj[0].primitive.cone.angle;
    normal_buf[0].x = buf.x * n;
	normal_buf[0].y = buf.y * n;
	normal_buf[0].z = buf.z * n;
    buf = intersection_buf[0] - obj[0].primitive.cone.position;
    normal_buf[0] = buf - normal_buf[0];
    normal_buf[0] = native_divide(normal_buf[0], length(normal_buf[0]));
    if (dot(ray_buf[0], normal_buf[0]) > 0.0001)
    {
		normal_buf[0].x = normal_buf[0].x * -1;
		normal_buf[0].y = normal_buf[0].y * -1;
		normal_buf[0].z = normal_buf[0].z * -1;
	}
}

void get_normal_sphere(__global t_object_d *obj, \
                        __global float3 *ray_buf, \
                        __global int *index_buf, \
                        __global float3 *normal_buf, \
                        __global float3 *intersection_buf)
{
	float l;

	normal_buf[0] = intersection_buf[0] - obj[0].primitive.sphere.center;
	l = length(normal_buf[0]);
	normal_buf[0] = native_divide(normal_buf[0], l);
    if (dot(ray_buf[0], normal_buf[0]) > 0.0001)
	{
		normal_buf[0].x = normal_buf[0].x * -1;
		normal_buf[0].y = normal_buf[0].y * -1;
		normal_buf[0].z = normal_buf[0].z * -1;
	}
}

void get_normal_triangle(__global t_object_d *obj, \
                        __global float3 *ray_buf, \
                        __global float3 *normal_buf)
{
	normal_buf[0] = obj[0].primitive.triangle.normal;
    if (dot(ray_buf[0], normal_buf[0]) > 0.0001)
	{
		normal_buf[0].x = normal_buf[0].x * -1;
		normal_buf[0].y = normal_buf[0].y * -1;
		normal_buf[0].z = normal_buf[0].z * -1;
	}
}

void get_normal_plane(__global t_object_d *obj, \
                        __global float3 *ray_buf, \
                        __global float3 *normal_buf)
{
	normal_buf[0] = obj[0].primitive.plane.normal;
    if (dot(ray_buf[0], normal_buf[0]) > 0.0001)
	{
		normal_buf[0].x = normal_buf[0].x * -1;
		normal_buf[0].y = normal_buf[0].y * -1;
		normal_buf[0].z = normal_buf[0].z * -1;
	}
}

float3  change_basis(float3 vec, t_basis basis)
{
    float3  tmp;
	tmp.x = dot(vec, basis.v);
	tmp.y = dot(vec, basis.u);
	tmp.z = dot(vec, basis.w);
    return (tmp);
} 

float3  mapping_plane(float3 t, t_object_d obj)
{
    float3 p;
    t -= obj.primitive.plane.point;
    t = change_basis(t, obj.basis);
    t.x /= obj.t_scale;
    t.y /= obj.t_scale;
    t.z /= obj.t_scale;
    p.x = t.x;
    p.y = t.z;
    p.z = 0;
    return (p);
}

float3		mapping_triangle(float3 t, t_object_d obj)
{
	float3 p;

	t -= obj.primitive.triangle.vertex[0];
	t = change_basis(t, obj.basis);
	t.x /= obj.t_scale;
	t.y /= obj.t_scale;
	t.z /= obj.t_scale;
	p.x = t.x;
	p.y = t.z;
	p.z = 0;
	return (p);
}

float3		mapping_cone(float3 t, t_object_d obj)
{
	float3 p;
	float3 tmp;

	t -= obj.primitive.cone.position;
	t = change_basis(t, obj.basis);
	tmp.x = t.x;
	tmp.y = t.z;
	tmp = normalize(tmp);
	float phi = acos(tmp.x) / 1.5707963267948;
	phi = tmp.y > 0 ? 1.f - phi : phi;
	t.x /= obj.t_scale;
	t.y /= obj.t_scale;
	t.z /= obj.t_scale;
	p.x = fabs(-phi);
	p.y = fabs(t.y);
	p.z = 0;
	return (p);
}

float3		mapping_cylinder(float3 t, t_object_d obj)
{
	float3 p;

	t -= obj.primitive.cylinder.position;
	t = change_basis(t, obj.basis);
	float phi = acos(t.x / obj.primitive.cylinder.radius) / 1.5707963267948;
	phi = t.z > 0 ? 1.f - phi : phi;
	t /= obj.t_scale;
	p.x = phi;
	p.y = -t.y;
	p.z = 0;
	return (p);
}

float3		mapping_sphere(float3 t, t_object_d obj)
{
 	float3 p;
	float3 tmp;

	t -= obj.primitive.sphere.center;
	t = change_basis(t, obj.basis);
	t /= obj.t_scale;
	float theta = acos(t.y) / 3.14159265358979;
	tmp.x = t.x;
	tmp.y = t.z;
	tmp = normalize(tmp);
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
 	if (obj.type == PLANE)
		p = mapping_plane(t, obj);
	if (obj.type == CYLINDER)
		p = mapping_cylinder(t, obj);
	if (obj.type == CONE)
		p = mapping_cone(t, obj);
	if (obj.type == TRIANGLE)
		p = mapping_triangle(t, obj);
    return (p);
} 


void	get_color_tex(__global uchar  *texture, float x, float y, t_object_d obj, int key, __global float3 *normal)
{
 	if (x < 0)
		x = obj.texture_width_nm + x;
	if (y < 0)
		y = obj.texture_height_nm + y;
	int fx = obj.texture_width_nm - (int)(obj.texture_width_nm * x) % obj.texture_width_nm;
	int fy = (int)( obj.texture_height_nm * y) % obj.texture_height_nm;
	t_color c;
	c.red = texture[obj.normal_map_id + 4 * fx + obj.l_size_nm * fy + 2];
	c.green = texture[obj.normal_map_id + 4 * fx + obj.l_size_nm * fy + 1];
	c.blue = texture[obj.normal_map_id + 4 * fx + obj.l_size_nm * fy];
	c.alpha = texture[obj.normal_map_id + 4 * fx + obj.l_size_nm * fy + 3];
	float3 new_norm;
	new_norm.x = c.red / 255.0f;
	new_norm.y = c.green / 255.0f;
	new_norm.z = c.blue / 255.0f;
	new_norm = normalize(new_norm);
	float3 buf;
	buf.x = 0.0f;
	buf.y = 1.0f;
	buf.z = 0.0f;
 	float3 t = cross(normal[0], buf);
	float3 b;
 	if (!length(t))
	{
		buf.y = 0.0f;
		buf.z = 1.0f;
		t = normal[0] * buf;
	}
	t = normalize(t);
	buf = cross(normal[0], t);
	b = normalize(buf);
	new_norm *= 2.0f;
	buf.x = 1.0f;
	buf.y = 1.0f;
	buf.z = 1.0f;
	new_norm -= buf;
	normal[0].x = t.x * new_norm.x + b.x * new_norm.x + new_norm.x * normal[0].x;
	normal[0].y = t.y * new_norm.y + b.y * new_norm.y + new_norm.y * normal[0].y;
	normal[0].z = t.z * new_norm.z + b.z * new_norm.z + new_norm.z * normal[0].z;
	normal[0] = normalize(normal[0]);
}

__kernel void get_normal_buf_cl(__global t_object_d *obj, \
                                __global float3 *ray_buf, \
                                __global int *index_buf, \
                                __global float3 *normal_buf, \
                                __global float3 *intersection_buf, \
								__global float *depth_buf, \
								float3 camera_position, int bounce_cnt, __global uchar *texture)
{
    int i = get_global_id(0);
	int j = index_buf[i];
	float l;
	float3 buf_camera;
	float3 text_point;
	if (bounce_cnt == 0)
		buf_camera = camera_position;
	else
	{
		buf_camera.x = intersection_buf[i].x + ray_buf[i].x * -1 * depth_buf[i];
		buf_camera.y = intersection_buf[i].y + ray_buf[i].y * -1 * depth_buf[i];
		buf_camera.z = intersection_buf[i].z + ray_buf[i].z * -1 * depth_buf[i];
	}
	if (j != -1)
	{
		if (obj[j].type == SPHERE)
			get_normal_sphere(&obj[j], &ray_buf[i], &index_buf[i], &normal_buf[i], &intersection_buf[i]);
		else if (obj[j].type == PLANE)
			get_normal_plane(&obj[j], &ray_buf[i], &normal_buf[i]);
		else if (obj[j].type == TRIANGLE)
			get_normal_triangle(&obj[j], &ray_buf[i], &normal_buf[i]);
		else if (obj[j].type == CONE)
			get_normal_cone(&obj[j], &ray_buf[i], &index_buf[i], &normal_buf[i], &intersection_buf[i], buf_camera, &depth_buf[i]);
		else if (obj[j].type == CYLINDER)
			get_normal_cylinder(&obj[j], &ray_buf[i], &index_buf[i], &normal_buf[i], &intersection_buf[i], buf_camera, &depth_buf[i]);
		else if (obj[j].type == ELLIPSOID)
			get_normal_ellipsoid(&obj[j], &ray_buf[i], &index_buf[i], &normal_buf[i], &intersection_buf[i], buf_camera, &depth_buf[i]);
		else if (obj[j].type == BOX)
			get_normal_box(&obj[j], &ray_buf[i], &index_buf[i], &normal_buf[i], &intersection_buf[i], buf_camera, &depth_buf[i]);
		else if (obj[j].type == PARABOLOID)
			get_normal_paraboloid(&obj[j], &ray_buf[i], &index_buf[i], &normal_buf[i], &intersection_buf[i], buf_camera, &depth_buf[i]);
		else if (obj[j].type == TORUS)
			get_normal_torus(&obj[j], &ray_buf[i], &index_buf[i], &normal_buf[i], &intersection_buf[i], buf_camera, &depth_buf[i]);
		else  if (obj[j].type == HYPERBOLOID)
			get_normal_hyperboloid(&obj[j], &ray_buf[i], &index_buf[i], &normal_buf[i], &intersection_buf[i], buf_camera, &depth_buf[i]);
		if(obj[j].normal_map_id != -1)
		{
			text_point = text_map_select(obj[j], intersection_buf[i]);
			get_color_tex(texture, text_point.x, text_point.y, obj[j], i, &normal_buf[i]);
		}
	}
	else
		normal_buf[i] = 0;
}