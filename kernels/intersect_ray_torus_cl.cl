//#include "kernel.h"

enum light_type{
	POINT,
	AMBIENT,
	DIRECTIONAL
};

typedef struct		s_basis
{
	float3			u;
	float3			v;
	float3			w;
}					t_basis;

typedef	struct		s_light
{
	float3			position;
	float3			direction;
	float			intensity;
	int				type;
}					t_light;

typedef struct		s_color
{
	uchar			red;
	uchar			green;
	uchar			blue;
	uchar			alpha;
}					t_color;

typedef	struct		s_material
{
	t_color			color;
	float			specular;
	float			reflection;
}					t_material;

typedef struct		s_sphere
{
	float3			center;
	float			radius;
}					t_sphere;

typedef struct		s_plane
{
	float3			normal;
	float3			point;
	float			d;
}					t_plane;

typedef struct		s_cylinder
{
	float3			position;
	float3			vec;
	float			radius;
}					t_cylinder;

typedef struct		s_cone
{
	float3			position;
	float3			vec;
	float			angle;
}					t_cone;

typedef struct		s_triangle
{
	float3			vertex[3];
	float3			normal;
}					t_triangle;

typedef	struct		s_ellipsoid
{
	float3			center1;
	float3			center2;
	float			radius;
}					t_ellipsoid;

typedef	struct		s_box
{
	float3			a;
	float3			b;
	int				face_hit;
}					t_box;

typedef struct		s_paraboloid
{
	float3			center;
	float			k;
}					t_paraboloid;

typedef struct		s_torus
{
	float3			center;
	float			radius1;
	float			radius2;
}					t_torus;

typedef	union		primitive
{
	t_cylinder		cylinder;
	t_cone			cone;
	t_sphere		sphere;
	t_plane			plane;
	t_triangle		triangle;
	t_ellipsoid		ellipsoid;
	t_paraboloid	paraboloid;
	t_box			box;
	t_torus			torus;
}					t_primitive;

typedef	struct		s_cutting_surface
{
	int 			type;
	t_sphere		sphere;
	t_plane			plane;
	t_triangle		triangle;
	t_cone			cone;
	t_cylinder		cylinder;
}					t_cutting_surface;

enum object_type {
	SPHERE,
	CONE,
	TRIANGLE,
	CYLINDER,
	PLANE,
	ELLIPSOID,
	HYPERBOLOID,
	PARABOLOID,
	BOX,
	TORUS
};

typedef struct		s_object3d_d
{
	t_primitive		primitive;
	t_basis			basis;
	float3			rotation;
	t_color			color;
	float			specular;
	float			roughness;
	float			refraction;
	float			reflection;
	int				color_disrupt;
	int				type;
	int				texture_id;
	int				texture_size;
	int				texture_width;
	int				texture_height;
	int				l_size;
}					t_object_d;

float2		swap(float2 ab)
{
	float tmp;

	tmp = ab.x;
	ab.x = ab.y;
	ab.y = tmp;
	return ab;
}

//=============================================================================
// _root3, root3 from http://prografix.narod.ru
//=============================================================================
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
} //!

float root3 ( float x )
{
    if ( x > 0 ) return _root3 ( x ); else
    if ( x < 0 ) return ((_root3(-x)) * (-1)); else
    return 0;
} //!

//---------------------------------------------------------------------------
// x - array of size 3
// In case 3 real roots: => x[0], x[1], x.s2, return 3
//         2 real roots: x[0], x[1],          return 2
//         1 real root : x[0], x[1] ± i*x.s2, return 1
float8 SolveP3(float8 x,float a,float b,float c)
{	// solve cubic equation x^3 + a*x^2 + b*x + c = 0
	float a2 = a*a;
    float q  = (a2 - 3*b)/9; 
	float r  = (a*(2*a2-9*b) + 27*c)/54;
	// equation x^3 + q*x + r = 0
     float r2 = r*r;
	float q3 = q*q*q;
	float A,B;
	if (r2 <= (q3 + (1e-14))) {
		float t=r/sqrt(q3);
		if( t<-1) t=-1;
		if( t> 1) t= 1;
        t=acos(t);
        a/=3; q=-2*sqrt(q);
        x.s0=q*cos(t/3)-a;
        x.s1=q*cos((t + M_PI * 2)/3)-a;
        x.s2=q*cos((t-  M_PI * 2)/3)-a;
		x.s4 = 3;
        return(x);
    } else {
        A =-root3(fabs(r)+sqrt(r2-q3)); 
		if( r<0 ) A=-A;
		B = (A==0? 0 : q/A);

		a/=3;
		x.s0 =(A+B)-a;
        x.s1 = -0.5f * (A+B)-a;
        x.s2 = 0.5f * sqrt(3.0f) * (A-B);
		if (fabs(x.s2) < (1e-14) )
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
// SolveP3(float *x,float a,float b,float c) {	
//---------------------------------------------------------------------------
// a>=0!
float2  CSqrt( float x, float y, float2 ab) // returns:  a+i*s = sqrt(x+i*y)
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
//---------------------------------------------------------------------------
float8   SolveP4Bi(float8 x, float b, float d)	// solve equation x^4 + b*x^2 + d = 0
{
	float D = b*b-4*d;
	if( D>=0 ) 
	{
		float sD = sqrt(D);
		float x1 = (-b+sD)/2;
		float x2 = (-b-sD)/2;	// x2 <= x1
		if( x2>=0 )				// 0 <= x2 <= x1, 4 real roots
		{
			float sx1 = sqrt(x1);
			float sx2 = sqrt(x2);
			x.s0 = -sx1;
			x.s1 =  sx1;
			x.s2 = -sx2;
			x.s3 =  sx2;
			x.s4 = 4;
			return x;
		}
		if( x1 < 0 )				// x2 <= x1 < 0, two pair of imaginary roots
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
		// now x2 < 0 <= x1 , two real roots and one pair of imginary root
			float sx1 = sqrt( x1);
			float sx2 = sqrt(-x2);
			x.s0 = -sx1;
			x.s1 = sx1;
			x.s2 = 0;
			x.s3 = sx2;
			x.s4 = 2;
			return x;
	} else { // if( D < 0 ), two pair of compex roots
		float sD2 = 0.5*sqrt(-D);
		float2 ab;
		ab.x = x.s0;
		ab.y = x.s1;
		ab = CSqrt(-0.5*b, sD2, ab);
		x.s0 = ab.x;
		x.s1 = ab.y;
		ab.x = x.s2;
		ab.y = x.s3;
		ab = CSqrt(-0.5*b,-sD2, ab);
		x.s2 = ab.x;
		x.s3 = ab.y;
		x.s4 = 0;
		return x;
	} // if( D>=0 ) 
} // SolveP4Bi(float *x, float b, float d)	// solve equation x^4 + b*x^2 d
//---------------------------------------------------------------------------
float8  dblSort3(float8 abc) // make: a <= b <= c
{
	float2 k;
	if (abc.s0 > abc.s1 )
	{
		k.x = abc.s0;
		k.y = abc.s1;
		k = swap(k);	// now a<=b
		abc.s0 = k.x;
		abc.s1 = k.y;
	}
	if( abc.s2 < abc.s1 )
	{
		k.x = abc.s1;
		k.y = abc.s2;
		k = swap(k);			// now a<=b, b<=c
		abc.s1 = k.x;
		abc.s2 = k.y;
		if (abc.s0 > abc.s1)
		{
			k.x = abc.s0;
			k.y = abc.s1;
			k = swap(k);	// now a<=b
			abc.s0 = k.x;
			abc.s1 = k.y;
		}
	}
	return abc;
}
//---------------------------------------------------------------------------
float8   SolveP4De(float8 x, float b, float c, float d)	// solve equation x^4 + b*x^2 + c*x + d
{
	//if( c==0 ) return SolveP4Bi(x,b,d); // After that, c!=0
	if( fabs(c)< 1e-14 * (fabs(b)+fabs(d)) )
	{
		x = SolveP4Bi(x,b,d);
		return x; // After that, c!=0
	}
	x = SolveP3( x, 2*b, b*b-4*d, -c*c);
	int res3 = x.s4; // solve resolvent
	// by Viet theorem:  x1*x2*x3=-c*c not equals to 0, so x1!=0, x2!=0, x3!=0
	if( res3>1 )	// 3 real roots, 
	{				
		x = dblSort3(x);	// sort roots to x[0] <= x[1] <= x.s2
		// Note: x[0]*x[1]*x.s2= c*c > 0
		if( x.s0 > 0) // all roots are positive
		{
			float sz1 = sqrt(x.s0);
			float sz2 = sqrt(x.s1);
			float sz3 = sqrt(x.s2);
			// Note: sz1*sz2*sz3= -c (and not equal to 0)
			if( c>0 )
			{
				x.s0 = (-sz1 -sz2 -sz3)/2;
				x.s1 = (-sz1 +sz2 +sz3)/2;
				x.s2 = (+sz1 -sz2 +sz3)/2;
				x.s3 = (+sz1 +sz2 -sz3)/2;
				x.s4 = 4;
				return x;
			}
			// now: c<0
			x.s0 = (-sz1 -sz2 +sz3)/2;
			x.s1 = (-sz1 +sz2 -sz3)/2;
			x.s2 = (+sz1 -sz2 -sz3)/2;
			x.s3 = (+sz1 +sz2 +sz3)/2;
			x.s4 = 4;
			return x;
		} // if( x[0] > 0) // all roots are positive
		// now x[0] <= x[1] < 0, x.s2 > 0
		// two pair of comlex roots
		float sz1 = sqrt(-x.s0);
		float sz2 = sqrt(-x.s1);
		float sz3 = sqrt( x.s2);

		if( c>0 )	// sign = -1
		{
			x.s0 = -sz3/2;					
			x.s1 = ( sz1 -sz2)/2;		// x[0]±i*x[1]
			x.s2 =  sz3/2;
			x.s3 = (-sz1 -sz2)/2;		// x.s2±i*x.s3
			x.s4 = 0;
			return x;
		}
		// now: c<0 , sign = +1
		x.s0 =   sz3/2;
		x.s1 = (-sz1 +sz2)/2;
		x.s2 =  -sz3/2;
		x.s3 = ( sz1 +sz2)/2;
		x.s4 = 0;
		return x;
	} // if( res3>1 )	// 3 real roots, 
	// now resoventa have 1 real and pair of compex roots
	// x[0] - real root, and x[0]>0, 
	// x[1]±i*x.s2 - complex roots, 
	// x[0] must be >=0. But one times x[0]=~ 1e-17, so:
	if (x.s0 < 0) x.s0 = 0;
	float sz1 = sqrt(x.s0);
	float szr, szi;
	float2 ab;
	ab.x = szr;
	ab.y = szi;
	ab = CSqrt(x.s1, x.s2, ab);  // (szr+i*szi)^2 = x[1]+i*x.s2
	szr = ab.x;
	szi = ab.y;
	if( c>0 )	// sign = -1
	{
		x.s0 = -sz1/2-szr;			// 1st real root
		x.s1 = -sz1/2+szr;			// 2nd real root
		x.s2 = sz1/2; 
		x.s3 = szi;
		x.s4 = 2;
		return x;
	}
	// now: c<0 , sign = +1
	x.s0 = sz1/2-szr;			// 1st real root
	x.s1 = sz1/2+szr;			// 2nd real root
	x.s2 = -sz1/2;
	x.s3 = szi;
	x.s4 = 2;
	return x;
} // SolveP4De(float *x, float b, float c, float d)	// solve equation x^4 + b*x^2 + c*x + d
//-----------------------------------------------------------------------------
float N4Step(float x, float a,float b,float c,float d)	// one Newton step for x^4 + a*x^3 + b*x^2 + c*x + d
{
	float fxs= ((4*x+3*a)*x+2*b)*x+c;	// f'(x)
	if (fxs == 0) return x;	//return 1e99; <<-- FIXED!
	float fx = (((x+a)*x+b)*x+c)*x+d;	// f(x)
	return (x - fx / fxs);
} 
//-----------------------------------------------------------------------------
// x - array of size 4
// return 4: 4 real roots x[0], x[1], x.s2, x.s3, possible multiple roots
// return 2: 2 real roots x[0], x[1] and complex x.s2±i*x.s3, 
// return 0: two pair of complex roots: x[0]±i*x[1],  x.s2±i*x.s3, 
float8   SolveP4(float8 x,float a,float b,float c,float d) {	// solve equation x^4 + a*x^3 + b*x^2 + c*x + d by Dekart-Euler method
	// move to a=0:
	float d1 = d + 0.25f * a * (0.25f * b * a - 3/64 * a * a * a - c);
	float c1 = c + 0.5f * a *(0.25f * a * a - b);
	float b1 = b - 0.375f * a * a;


	x = SolveP4De(x, b1, c1, d1);
	int res = x.s4;
	if( res==4) { x.s0 -= a/4; x.s1 -= a/4; x.s2 -= a/4; x.s3 -= a/4; }
	else if (res==2) { x.s0-= a/4; x.s1-= a/4; x.s2-= a/4; }
	else             { x.s0-= a/4; x.s2-= a/4; }
	// one Newton step for each real root:
	if( res>0 )
	{
		x.s0 = N4Step(x.s0, a,b,c,d);
		x.s1 = N4Step(x.s1, a,b,c,d);
	}
	if( res>2 )
	{
		x.s2 = N4Step(x.s2, a,b,c,d);
		x.s3 = N4Step(x.s3, a,b,c,d);
	}
	return x;
}

float torus_intersection(t_torus torus, float3 ray_start, float3 ray_dir)
{
    float c_4;
    float c_3;
    float c_2;
    float c_1;
    float c_0;

	//printf("%f", torus.radius1);
	float sum_dir = ray_dir.x * ray_dir.x + ray_dir.y * ray_dir.y + ray_dir.z * ray_dir.z;
    float e = ray_start.x * ray_start.x + ray_start.y * ray_start.y + ray_start.z * ray_start.z - torus.radius1 * torus.radius1 - torus.radius2 * torus.radius2;
    float f = ray_start.x * ray_dir.x + ray_start.y * ray_dir.y + ray_start.z * ray_dir.z;
	float four = 4.0f * torus.radius1 * torus.radius1;
	//printf("%f", ray_start.x);
    c_4 = sum_dir * sum_dir;
    c_3 = 4 * sum_dir * f;
    c_2 = 2 * sum_dir * e + 4 * f * f + four * ray_dir.y * ray_dir.y;
    c_1 = 4 * f * e + 2 * four * ray_start.y * ray_dir.y;
    c_0 = e * e - four * (torus.radius2 * torus.radius2 - ray_start.y * ray_start.y);

	float8 roots;
	float t = 1.0E10f;
	float a =  c_3 / c_4;
	float b =  c_2 / c_4;
	float c =  c_1 / c_4;
	float d =  c_0 / c_4;

	roots = SolveP4(roots, a, b, c, d);
	int num_roots = roots.s4;
	if (num_roots == 0)
		return 0;
	float root[4];
	root[0] = roots.s0;
	root[1] = roots.s1;
	root[2] = roots.s2;
	root[3] = roots.s3;
	int intersected = 0;
	float tmin;
	for (int i = 0; i < num_roots; i++)
	{
		if (root[i] > 0.0001f)
		{
			intersected = 1;
			if (root[i] < t)
			{
				t = root[i];
			}
		}
	}
	if (intersected == 0)
		return (0);
	tmin = t;
	return (1);
}

__kernel  void    intersect_ray_torus(__global float3 *ray_arr, \
											__global float3 *camera_start, \
											t_torus torus, \
											__global float *depth_buf, \
											__global int *index_buf, int index)
{
    int i = get_global_id(0);
    float res;
	float3 ray;
    ray = camera_start[i] + ray_arr[i] + 0.001f;
 	res = torus_intersection(torus, ray, ray_arr[i]);
	if (res > 0.01 && res < depth_buf[i])
	{
		depth_buf[i] = res;
		index_buf[i] = index;
	}
}
