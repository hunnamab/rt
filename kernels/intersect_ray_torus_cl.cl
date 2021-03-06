#include "kernel.h"
float	get_sum(float4 matrix1[4], float4 matrix2[4], int i, int j)
{
	float	sum;
	int		k;

	k = 0;
	sum = 0;
	while (k < 4)
	{
		sum += matrix1[i][k] * matrix2[k][j];
		k++;
	}
	return (sum);
}

void	matr_mul_void(float4 *matrix1, float4 matrix2[4])
{
	int		i;
	int		j;
	float	buf;
	float4	m_buf;

	i = -1;
	while (i++ < 4)
		m_buf[i] = matrix1[0][i];
	i = 0;
	j = 0;
	buf = 0;
	while (i < 4)
	{
		while (j < 4)
		{
			buf += matrix2[i][j] * m_buf[j];
			j++;
		}
		matrix1[0][i] = buf;
		buf = 0;
		j = 0;
		i++;
	}
}

void    matr_mul(float4 matrix1[4], float4 matrix2[4], float4 result[4])
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (i < 4)
	{
		while (j < 4)
		{
			result[i][j] = get_sum(matrix1, matrix2, i, j);
			j++;
		}
		j = 0;
		i++;
	}
}
 
 void    rotate_x(float x, float4 matrix[4])
{
	matrix[0][0] = 1;
	matrix[0][2] = 0;
	matrix[1][1] = cos(x);
	matrix[1][2] = -sin(x);
	matrix[2][1] = sin(x);
	matrix[2][2] = cos(x);
	matrix[3][3] = 1;
}

void    rotate_z(float z, float4 matrix[4])
{
	matrix[0][0] = cos(z);
	matrix[0][1] = -sin(z);
	matrix[1][0] = sin(z);
	matrix[1][1] = cos(z);
	matrix[2][2] = 1;
	matrix[3][3] = 1;
}

void    rotate_y(float y, float4 matrix[4])
{
	matrix[0][0] = cos(y);
	matrix[0][2] = sin(y);
	matrix[1][1] = 1;
	matrix[2][0] = -sin(y);
	matrix[2][2] = cos(y);
	matrix[3][3] = 1;
}

 void	get_rotation_matrix(float3 angle, float4 *vec)
{
	float4 rotation_matrix[4];
	float4 rotation_buf[4];
	float4 rotation_buf_second[4];
    int i = 0;

    while(i < 4)
    {
        rotation_matrix[i] = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
        rotation_buf[i] = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
        rotation_buf_second[i] = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
        i++;
    }
    i++;
	rotate_z(angle.z, rotation_buf);
	rotate_y(angle.y, rotation_buf_second);
	matr_mul(rotation_buf, rotation_buf_second, rotation_matrix);
    while(i < 4)
    {
        rotation_buf[i] = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
        rotation_buf_second[i] = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
        i++;
    }
	rotate_x(angle.x, rotation_buf);
	matr_mul(rotation_matrix, rotation_buf, rotation_buf_second);
    matr_mul_void(vec, rotation_buf_second);
} 

int cut(float3 point, __global t_cutting_surface *cs, int cs_nmb)
{
    int i;
    float result;
    i = 0;
    while(i < cs_nmb)
    {
        if (cs[i].type == PLANE)
        {
            result = cs[i].param1.x * point.x + cs[i].param1.y * point.y + cs[i].param1.z * point.z + cs[i].param3;
            if (result >= 0)
                return (0);
        }
		if(cs[i].type == SPHERE)
		{
			float3 buf;
			buf = point - cs[i].param1;
			result = length(buf);
			if (result >= cs[i].param3 && !cs[i].is_negative)
				return(0);
			if(result <= cs[i].param3 && cs[i].is_negative)
				return(0);
		}
        if(cs[i].type == CYLINDER)
        {
            float4 buf_point;
            buf_point.x = point.x;
            buf_point.y = point.y;
            buf_point.z = point.z;
            buf_point.w = 1;
            get_rotation_matrix(cs[i].param2, &buf_point);
            result = (buf_point.x * buf_point.x - 2 * buf_point.x * cs[i].param1.x + cs[i].param1.x * cs[i].param1.x) / cs[i].param3 + \
            (buf_point.y * buf_point.y - 2 * buf_point.y * cs[i].param1.y + cs[i].param1.y * cs[i].param1.y) / cs[i].param3 - 1;
            if (result < 0 && cs[i].is_negative)
                return(0);
            if (result > 0 && !cs[i].is_negative)
                return(0);
        }
        i++;
    }
    return (1);
}

float2		swap(float2 ab)
{
	float tmp;

	tmp = ab.x;
	ab.x = ab.y;
	ab.y = tmp;
	return ab;
}

float _root3 (float x)
{
    float s = 1.0f;
    while ( x < 1.0f)
    {
        x *= 8.0f;
        s *= 0.5f;
    }
    while ( x > 8)
    {
        x *= 0.125f;
        s *= 2;
    }
    float r = 1.5f;
    r -= 1/3 * ( r - x / ( r * r ) );
    r -= 1/3 * ( r - x / ( r * r ) );
    r -= 1/3 * ( r - x / ( r * r ) );
    r -= 1/3 * ( r - x / ( r * r ) );
    r -= 1/3 * ( r - x / ( r * r ) );
    r -= 1/3 * ( r - x / ( r * r ) );
    return r * s;
}

float root3 ( float x )
{
    if ( x > 0 ) return _root3 ( x ); else
    if ( x < 0 ) return ((_root3(-x)) * (-1.0f)); else
    return 0;
}

float8 SolveP3(float8 x,float a,float b,float c)
{
	float a2 = a*a;
    float q  = (a2 - 3.0f*b)/9.0f; 
	float r  = (a*(2.0f*a2-9.0f*b) + 27.0f*c)/54.0f;
    float r2 = r*r;
	float q3 = q*q*q;
	float A,B;
	if (r2 <= (q3 + FLT_EPSILON)) {
		float t=r/sqrt(q3);
		if( t<-1.0f) t=-1.0f;
		if( t> 1.0f) t= 1.0f;
        t=acos(t);
        a/=3.0f; q=-2.0f*sqrt(q);
        x.s0=q*cos(t/3.0f)-a;
        x.s1=q*cos((t + M_PI * 2.0f)/3.0f)-a;
        x.s2=q*cos((t-  M_PI * 2.0f)/3.0f)-a;
		x.s4 = 3.0f;
        return(x);
    } else {
        A =-root3(fabs(r)+sqrt(r2-q3)); 
		if( r<0 ) A=-A;
		B = (A==0? 0 : q/A);

		a/=3;
		x.s0 =(A+B)-a;
        x.s1 = -0.5f * (A+B)-a;
        x.s2 = 0.5f * sqrt(3.0f) * (A-B);
		if (fabs(x.s2) < FLT_EPSILON )
		{
			x.s2 = x.s1;
			x.s4 = 2;
			return x;
		}
		x.s4 = 1;
        return (x);
    }
	return (x);
}

float2  CSqrt( float x, float y, float2 ab)
{
	float r  = sqrt(x*x+y*y);
	if( y==0 ) { 
		r = sqrt(r);
		if(x>=0) { ab.x=r; ab.y=0; } else { ab.x=0; ab.y=r; }
	} else {		// y != 0
		ab.x = sqrt(0.5f*(x+r));
		ab.y = 0.5f * y / ab.x;
	}
	return ab;
}

float8   SolveP4Bi(float8 x, float b, float d)
{
	float D = b*b-4.0f*d;
	if( D>=0 ) 
	{
		float sD = sqrt(D);
		float x1 = (-b+sD)/2.0f;
		float x2 = (-b-sD)/2.0f;
		if( x2>=0 )			
		{
			float sx1 = sqrt(x1);
			float sx2 = sqrt(x2);
			x.s0 = -sx1;
			x.s1 =  sx1;
			x.s2 = -sx2;
			x.s3 =  sx2;
			x.s4 = 4.0f;
			return x;
		}
		if( x1 < 0 )			
		{
			float sx1 = sqrt(-x1);
			float sx2 = sqrt(-x2);
			x.s0 =    0;
			x.s1 =  sx1;
			x.s2 =    0;
			x.s3 =  sx2;
			x.s4 = 0;
			return x;
		}
			float sx1 = sqrt( x1);
			float sx2 = sqrt(-x2);
			x.s0 = -sx1;
			x.s1 = sx1;
			x.s2 = 0;
			x.s3 = sx2;
			x.s4 = 2.0f;
			return x;
	} else {
		float sD2 = 0.5f*sqrt(-D);
		float2 ab;
		ab.x = x.s0;
		ab.y = x.s1;
		ab = CSqrt(-0.5f*b, sD2, ab);
		x.s0 = ab.x;
		x.s1 = ab.y;
		ab.x = x.s2;
		ab.y = x.s3;
		ab = CSqrt(-0.5f*b,-sD2, ab);
		x.s2 = ab.x;
		x.s3 = ab.y;
		x.s4 = 0;
		return x;
	}
}

float8  dblSort3(float8 abc) // make: a <= b <= c
{
	float2 k;
	if (abc.s0 > abc.s1 )
	{
		k.x = abc.s0;
		k.y = abc.s1;
		k = swap(k);	
		abc.s0 = k.x;
		abc.s1 = k.y;
	}
	if( abc.s2 < abc.s1 )
	{
		k.x = abc.s1;
		k.y = abc.s2;
		k = swap(k);		
		abc.s1 = k.x;
		abc.s2 = k.y;
		if (abc.s0 > abc.s1)
		{
			k.x = abc.s0;
			k.y = abc.s1;
			k = swap(k);	
			abc.s0 = k.x;
			abc.s1 = k.y;
		}
	}
	return abc;
}

float8   SolveP4De(float8 x, float b, float c, float d)	
{
	if( fabs(c)< FLT_EPSILON * (fabs(b)+fabs(d)) )
	{
		x = SolveP4Bi(x,b,d);
		return x;
	}
	x = SolveP3( x, 2.0f*b, b*b-4.0f*d, -c*c);
	int res3 = x.s4;
	if( res3>1.0f )
	{				
		x = dblSort3(x);
		if( x.s0 > 0)
		{
			float sz1 = sqrt(x.s0);
			float sz2 = sqrt(x.s1);
			float sz3 = sqrt(x.s2);
			if( c>0 )
			{
				x.s0 = (-sz1 -sz2 -sz3)/2.0f;
				x.s1 = (-sz1 +sz2 +sz3)/2.0f;
				x.s2 = (+sz1 -sz2 +sz3)/2.0f;
				x.s3 = (+sz1 +sz2 -sz3)/2.0f;
				x.s4 = 4.0f;
				return x;
			}
			x.s0 = (-sz1 -sz2 +sz3)/2.0f;
			x.s1 = (-sz1 +sz2 -sz3)/2.0f;
			x.s2 = (+sz1 -sz2 -sz3)/2.0f;
			x.s3 = (+sz1 +sz2 +sz3)/2.0f;
			x.s4 = 4.0f;
			return x;
		} 
		float sz1 = sqrt(-x.s0);
		float sz2 = sqrt(-x.s1);
		float sz3 = sqrt( x.s2);

		if( c>0 )
		{
			x.s0 = -sz3/2.0f;					
			x.s1 = ( sz1 -sz2)/2.0f;	
			x.s2 =  sz3/2.0f;
			x.s3 = (-sz1 -sz2)/2.0f;	
			x.s4 = 0;
			return x;
		}
		x.s0 =   sz3/2.0f;
		x.s1 = (-sz1 +sz2)/2.0f;
		x.s2 =  -sz3/2.0f;
		x.s3 = ( sz1 +sz2)/2.0f;
		x.s4 = 0;
		return x;
	} 
	if (x.s0 < 0) x.s0 = 0;
	float sz1 = sqrt(x.s0);
	float szr, szi;
	float2 ab;
	ab.x = szr;
	ab.y = szi;
	ab = CSqrt(x.s1, x.s2, ab); 
	szr = ab.x;
	szi = ab.y;
	if( c>0 )	
	{
		x.s0 = -sz1/2.0f-szr;			
		x.s1 = -sz1/2.0f+szr;			
		x.s2 = sz1/2.0f; 
		x.s3 = szi;
		x.s4 = 2.0f;
		return x;
	}
	x.s0 = sz1/2-szr;		
	x.s1 = sz1/2+szr;		
	x.s2 = -sz1/2;
	x.s3 = szi;
	x.s4 = 2;
	return x;
} 

float N4Step(float x, float a,float b,float c,float d)
{
	float fxs= ((4.0f*x+3.0f*a)*x+2.0f*b)*x+c;
	if (fxs == 0) return x;
	float fx = (((x+a)*x+b)*x+c)*x+d;
	return (x - fx / fxs);
} 

float8   SolveP4(float8 x,float a,float b,float c,float d) {
	float d1 = d + 0.25f * a * (0.25f * b * a - 3.0f/64.0f * a * a * a - c);
	float c1 = c + 0.5f * a *(0.25f * a * a - b);
	float b1 = b - 0.375f * a * a;


	x = SolveP4De(x, b1, c1, d1);
	int res = x.s4;
	if( res==4) { x.s0 -= a/4.0f; x.s1 -= a/4.0f; x.s2 -= a/4.0f; x.s3 -= a/4.0f; }
	else if (res==2) { x.s0-= a/4.0f; x.s1-= a/4.0f; x.s2-= a/4.0f; }
	else             { x.s0-= a/4.0f; x.s2-= a/4.0f; }
	if( res>0 )
	{
		x.s0 = N4Step(x.s0, a,b,c,d);
		x.s1 = N4Step(x.s1, a,b,c,d);
	}
	if( res>2.0f )
	{
		x.s2 = N4Step(x.s2, a,b,c,d);
		x.s3 = N4Step(x.s3, a,b,c,d);
	}
	return x;
}

float*	sort(private float *roots)
{
	for (int i = 3; i >= 0; i--)
	{	for (int j = 1; j <= i; j++)
			if (isgreater(roots[j - 1] ,roots[i]))
			{
				float tmp;
				tmp = roots[j - 1];
				roots[j - 1] = roots[j];
				roots[j] = tmp;
			}
	}
}

float torus_intersection(t_torus torus, float3 ray_start, float3 ray_dir)
{
    float c_4;
    float c_3;
    float c_2;
    float c_1;
    float c_0;

	float m = dot(ray_dir, ray_dir);
	float n = dot(ray_dir, (ray_start - torus.center));
	float o = dot((ray_start - torus.center), (ray_start - torus.center));
	float p = dot(ray_dir, torus.vec);
	float q = dot((ray_start - torus.center), torus.vec);
    c_4 = pow(m, 2);
    c_3 = 4.0f * m * n;
    c_2 = 4.0f * pow(m, 2) + 2.0f*m*o - 2.0f*(pow(torus.radius1, 2) + pow(torus.radius2, 2))*m + 4.0f*pow(torus.radius1, 2) *p * p;
    c_1 = 4.0f*n*o - 4.0f*(pow(torus.radius1, 2) + pow(torus.radius2, 2))*n + 8.0f*pow(torus.radius1, 2) *p*q;
    c_0 = o*o - 2.0f*(pow(torus.radius1, 2) + pow(torus.radius2, 2))*o + 4.0f*pow(torus.radius1, 2) *q * q + (pow(torus.radius1, 2) -pow(torus.radius2, 2)) * (pow(torus.radius1, 2) -pow(torus.radius2, 2));
	float8 roots;	
	float a =  c_3 / c_4;
	float b =  c_2 / c_4;
	float c =  c_1 / c_4;
	float d =  c_0 / c_4;

	roots = SolveP4(roots, a, b, c, d);
	int num_roots = roots.s4;
	if (num_roots == 0)
		return 0.0f;
	float root[4];
	root[0] = roots.s0;
	root[1] = roots.s1;
	root[2] = roots.s2;
	root[3] = roots.s3;
	for (int i = 0; i < 4; i++)
	{
		if (isless(root[i],0.0f))
		{
			root[i] = 1e5;
		}
	}
	sort(root);
	if (isless(root[0], 1e5))
	{
		return (root[0]);
	}
	return (0.0f);
} 
 
__kernel  void    intersect_ray_torus(__global float3 *ray_arr, \
                                __global float3 *camera_start, \
                                t_torus torus, \
                                __global float *depth_buf, \
                                __global int *index_buf, \
                                int index, __global t_cutting_surface *cs, \
								int cs_nmb, int bounce_cnt, \
								__global t_material *material_buf, \
								int is_refractive, __global float3 *normal_buf, \
								int refraction, __global int *exception_buf)
{
    int i = get_global_id(0);
    float res;
	float3 ray;
 	float t1, t2;
    int cut1, cut2;
	if (exception_buf[i] == index)
	{
		exception_buf[i] = -1;
		return ;
	}
	if (bounce_cnt > 0 || is_refractive)
    	camera_start[i] = camera_start[i] + ray_arr[i] * 0.00001f;
	if ((bounce_cnt == 0 && !is_refractive) || (bounce_cnt == 0 && is_refractive && material_buf[i].refraction > 0.0f && material_buf[i].kr < 1.0f) || material_buf[i].reflection > 0.0f)
		res = torus_intersection(torus, camera_start[i], ray_arr[i]);
	else
		return ;
	if (res > 0.0f && res < depth_buf[i])
    {
        float3 intersection_point;
        intersection_point = ray_arr[i] * res;
        intersection_point = intersection_point + camera_start[i];
		depth_buf[i] = res;
        index_buf[i] = index;
    }
}
