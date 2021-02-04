//#include "kernel.h"

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
	float			refraction;
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

typedef	struct		s_box
{
	float3			a;
	float3			b;
	float			face_hit;
}					t_box;

typedef	struct		s_ellipsoid
{
	float3			center1;
	float3			center2;
	float			radius;
}					t_ellipsoid;

typedef struct		s_paraboloid
{
	float3			center;
	float3			vec;
	float			k;
}					t_paraboloid;

typedef struct		s_torus
{
	float3			center;
	float3			vec;
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
	t_box			box;
	t_paraboloid	paraboloid;
	t_torus			torus;
}					t_primitive;

typedef	struct		 s_cutting_surface
{
	t_primitive		primitive;
	int				type;
	int				is_negative;
}					t_cutting_surface;

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
	int				normal_map_id; //разметка частей текстурного буфера для поиска карты нормалей
	int				texture_size_nm;
	int				texture_width_nm;
	int				texture_height_nm;
	int				l_size_nm;
}					t_object_d;

/* float2		swap(float2 ab)
{
	float tmp;

	tmp = ab.x;
	ab.x = ab.y;
	ab.y = tmp;
	return ab;
} */

//=============================================================================
// _root3, root3 from http://prografix.narod.ru
//=============================================================================
/* float _root3 (float x)
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
} //! */

/* float root3 ( float x )
{
    if ( x > 0 ) return _root3 ( x ); else
    if ( x < 0 ) return ((_root3(-x)) * (-1.0f)); else
    return 0;
} //! */

//---------------------------------------------------------------------------
// x - array of size 3
// In case 3 real roots: => x[0], x[1], x.s2, return 3
//         2 real roots: x[0], x[1],          return 2
//         1 real root : x[0], x[1] ± i*x.s2, return 1
/* float8 SolveP3(float8 x,float a,float b,float c)
{	// solve cubic equation x^3 + a*x^2 + b*x + c = 0
	float a2 = a*a;
    float q  = (a2 - 3.0f*b)/9.0f; 
	float r  = (a*(2.0f*a2-9.0f*b) + 27.0f*c)/54.0f;
	// equation x^3 + q*x + r = 0
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
} */
// SolveP3(float *x,float a,float b,float c) {	
//---------------------------------------------------------------------------
// a>=0!
/* float2  CSqrt( float x, float y, float2 ab) // returns:  a+i*s = sqrt(x+i*y)
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
} */
//---------------------------------------------------------------------------
/* float8   SolveP4Bi(float8 x, float b, float d)	// solve equation x^4 + b*x^2 + d = 0
{
	float D = b*b-4.0f*d;
	if( D>=0 ) 
	{
		float sD = sqrt(D);
		float x1 = (-b+sD)/2.0f;
		float x2 = (-b-sD)/2.0f;	// x2 <= x1
		if( x2>=0 )				// 0 <= x2 <= x1, 4 real roots
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
			x.s4 = 2.0f;
			return x;
	} else { // if( D < 0 ), two pair of compex roots
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
	} // if( D>=0 ) 
} */ // SolveP4Bi(float *x, float b, float d)	// solve equation x^4 + b*x^2 d
//---------------------------------------------------------------------------
/* float8  dblSort3(float8 abc) // make: a <= b <= c
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
} */
//---------------------------------------------------------------------------
/* float8   SolveP4De(float8 x, float b, float c, float d)	// solve equation x^4 + b*x^2 + c*x + d
{
	//if( c==0 ) return SolveP4Bi(x,b,d); // After that, c!=0
	if( fabs(c)< FLT_EPSILON * (fabs(b)+fabs(d)) )
	{
		x = SolveP4Bi(x,b,d);
		return x; // After that, c!=0
	}
	x = SolveP3( x, 2.0f*b, b*b-4.0f*d, -c*c);
	int res3 = x.s4; // solve resolvent
	// by Viet theorem:  x1*x2*x3=-c*c not equals to 0, so x1!=0, x2!=0, x3!=0
	if( res3>1.0f )	// 3 real roots, 
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
				x.s0 = (-sz1 -sz2 -sz3)/2.0f;
				x.s1 = (-sz1 +sz2 +sz3)/2.0f;
				x.s2 = (+sz1 -sz2 +sz3)/2.0f;
				x.s3 = (+sz1 +sz2 -sz3)/2.0f;
				x.s4 = 4.0f;
				return x;
			}
			// now: c<0
			x.s0 = (-sz1 -sz2 +sz3)/2.0f;
			x.s1 = (-sz1 +sz2 -sz3)/2.0f;
			x.s2 = (+sz1 -sz2 -sz3)/2.0f;
			x.s3 = (+sz1 +sz2 +sz3)/2.0f;
			x.s4 = 4.0f;
			return x;
		} // if( x[0] > 0) // all roots are positive
		// now x[0] <= x[1] < 0, x.s2 > 0
		// two pair of comlex roots
		float sz1 = sqrt(-x.s0);
		float sz2 = sqrt(-x.s1);
		float sz3 = sqrt( x.s2);

		if( c>0 )	// sign = -1
		{
			x.s0 = -sz3/2.0f;					
			x.s1 = ( sz1 -sz2)/2.0f;		// x[0]±i*x[1]
			x.s2 =  sz3/2.0f;
			x.s3 = (-sz1 -sz2)/2.0f;		// x.s2±i*x.s3
			x.s4 = 0;
			return x;
		}
		// now: c<0 , sign = +1
		x.s0 =   sz3/2.0f;
		x.s1 = (-sz1 +sz2)/2.0f;
		x.s2 =  -sz3/2.0f;
		x.s3 = ( sz1 +sz2)/2.0f;
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
		x.s0 = -sz1/2.0f-szr;			// 1st real root
		x.s1 = -sz1/2.0f+szr;			// 2nd real root
		x.s2 = sz1/2.0f; 
		x.s3 = szi;
		x.s4 = 2.0f;
		return x;
	}
	// now: c<0 , sign = +1
	x.s0 = sz1/2-szr;			// 1st real root
	x.s1 = sz1/2+szr;			// 2nd real root
	x.s2 = -sz1/2;
	x.s3 = szi;
	x.s4 = 2;
	return x;
} */ // SolveP4De(float *x, float b, float c, float d)	// solve equation x^4 + b*x^2 + c*x + d
//-----------------------------------------------------------------------------
/* float N4Step(float x, float a,float b,float c,float d)	// one Newton step for x^4 + a*x^3 + b*x^2 + c*x + d
{
	float fxs= ((4.0f*x+3.0f*a)*x+2.0f*b)*x+c;	// f'(x)
	if (fxs == 0) return x;	//return 1e99; <<-- FIXED!
	float fx = (((x+a)*x+b)*x+c)*x+d;	// f(x)
	return (x - fx / fxs);
}  */
//-----------------------------------------------------------------------------
// x - array of size 4
// return 4: 4 real roots x[0], x[1], x.s2, x.s3, possible multiple roots
// return 2: 2 real roots x[0], x[1] and complex x.s2±i*x.s3, 
// return 0: two pair of complex roots: x[0]±i*x[1],  x.s2±i*x.s3, 
/* float8   SolveP4(float8 x,float a,float b,float c,float d) {	// solve equation x^4 + a*x^3 + b*x^2 + c*x + d by Dekart-Euler method
	// move to a=0:
	float d1 = d + 0.25f * a * (0.25f * b * a - 3.0f/64.0f * a * a * a - c);
	float c1 = c + 0.5f * a *(0.25f * a * a - b);
	float b1 = b - 0.375f * a * a;


	x = SolveP4De(x, b1, c1, d1);
	int res = x.s4;
	if( res==4) { x.s0 -= a/4.0f; x.s1 -= a/4.0f; x.s2 -= a/4.0f; x.s3 -= a/4.0f; }
	else if (res==2) { x.s0-= a/4.0f; x.s1-= a/4.0f; x.s2-= a/4.0f; }
	else             { x.s0-= a/4.0f; x.s2-= a/4.0f; }
	// one Newton step for each real root:
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
	//-----------------------------
 	float3 V = {1.0f, 0.0f, 0.0f}; 
	torus.radius2 = 5.0f;
	torus.radius1 = 10.0f;
	//-----------------------------
	float m = dot(ray_dir, ray_dir);
	float n = dot(ray_dir, (ray_start - torus.center));
	float o = dot((ray_start - torus.center), (ray_start - torus.center));
	float p = dot(ray_dir, V);
	float q = dot((ray_start - torus.center), V);
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
} */

float paraboloid_intersection(t_paraboloid parab, float3 ray_start, float3 ray_dir)
{
    float3 parab_dir;
    float a;
    float b;
    float c;
	float t1;
    float t2;

    parab_dir = ray_start - parab.center;
	//dir_norm = normalize(parab.center);
 	float3 dir_norm = parab.vec;
	//dir_norm = normalize(dir_norm);
    a = dot(ray_dir, ray_dir) - pow(dot(ray_dir, dir_norm), 2);
    b = 2.0f * dot(ray_dir, parab_dir) - 2.0f * dot(ray_dir, dir_norm) * (dot(parab_dir, dir_norm) + 2.0f * parab.k);
    c = dot(parab_dir, parab_dir) - dot(parab_dir, dir_norm) * (dot(parab_dir, dir_norm) + 4.0f * parab.k);
    c = b * b - 4 * a * c;
	if (c >= 0)
	{
		c = sqrt(c);
		t1 = (-b + c) / (2.0f * a);
        t2 = (-b - c) / (2.0f * a);
        if ((t1 < t2 && t1 > 0) || (t2 < 0 && t1 >= 0))
            return (t1);
        if ((t2 < t1 && t2 > 0) || (t1 < 0 && t2 >= 0))
            return (t2);
        if (t2 == t1 && t2 >= 0)
            return (t2);
	}
    return (0);
}

float box_intersection(__global t_object_d *box, float3 ray_start, float3 ray_dir)
{
	/* float 	t_min;
	float 	t_max;
	float3		a;
	float		b;
	float		c;
	float		t0;
	float		t1;
	int		face_in;
	int		face_out;
	float		tmin;

	float lo = a.x*(box[0].primitive.box.a.x - ray_start.x);
	float hi = a.x*(box[0].primitive.box.b.x - ray_start.x);
	t_min  = min(lo, hi);
	t_max = max(lo, hi);
	float lo1 = a.y*(box[0].primitive.box.a.y - ray_start.y);
	float hi1 = a.y*(box[0].primitive.box.b.y - ray_start.y);
	t_min  = max(t_min, min(lo1, hi1));
	t_max = min(t_max, max(lo1, hi1));
	float lo2 = a.z*(box[0].primitive.box.a.z - ray_start.z);
	float hi2 = a.z*(box[0].primitive.box.b.z - ray_start.z);
	t_min  = max(t_min, min(lo2, hi2));
	t_max = min(t_max, max(lo2, hi2));
	if (t_min <= t_max && t_max > 0.0f)
		return (1);
	return (0); */
	float3 	t_min;
	float3 	t_max;
	float		a;
	float		b;
	float		c;
	float		t0;
	float		t1;
	int		face_in;
	int		face_out;
	float		tmin;

	a = 1.0f / ray_dir.x;
	if (a >= 0)
	{
		t_min.x = (box[0].primitive.box.a.x - ray_start.x) * a;
		t_max.x = (box[0].primitive.box.b.x - ray_start.x) * a;
	}
	else
	{
		t_min.x = (box[0].primitive.box.b.x - ray_start.x) * a;
		t_max.x = (box[0].primitive.box.a.x - ray_start.x) * a;
	}
	b = 1.0f / ray_dir.y;
	if (b >= 0)
	{
		t_min.y = (box[0].primitive.box.a.y - ray_start.y) * b;
		t_max.y = (box[0].primitive.box.b.y - ray_start.y) * b;
	}
	else
	{
		t_min.y = (box[0].primitive.box.b.y - ray_start.y) * b;
		t_max.y = (box[0].primitive.box.a.y - ray_start.y) * b;
	}
	c = 1.0f / ray_dir.z;
	if (c >= 0)
	{
		t_min.z = (box[0].primitive.box.a.z - ray_start.z) * c;
		t_max.z = (box[0].primitive.box.b.z - ray_start.z) * c;
	}
	else
	{
		t_min.z = (box[0].primitive.box.b.z - ray_start.z) * c;
		t_max.z = (box[0].primitive.box.a.z - ray_start.z) * c;
	}
	//find largest entering t value
	if (t_min.x > t_min.y)
	{
		t0 = t_min.x;
		face_in = (a >= 0.0f) ? 0 : 3;
	}
	else
	{
		t0 = t_min.y;
		face_in = (b >= 0.0f) ? 1 : 4;
	}
	if (t_min.z > t0)
	{
		t0 = t_min.z;
		face_in = (c >= 0.0f) ? 2 : 5;
	}
	//find smallest exiting t value
	if (t_max.x < t_max.y)
	{
		t1 = t_max.x;
		face_out = (a >= 0.0f) ? 3 : 0;
	}
	else
	{
		t1 = t_max.y;
		face_out = (b >= 0.0f) ? 4 : 1;
	}
	if (t_max.z < t1)
	{
		t1 = t_max.z;
		face_out = (c >= 0.0f) ? 5 : 2;
	}
	if (t0 < t1 && t1 > 0.1f)
	{
		if (t0 > 0.1f)
		{
			tmin = t0;
			box[0].primitive.box.face_hit = face_in;
		}
		else
		{
			tmin =  t1;
			box[0].primitive.box.face_hit = face_out;
		}
		return (tmin);
	}
	return (0);
}

float ellipsoid_intersection(t_ellipsoid el, float3 ray_start, float3 ray_dir)
{
    float k1;
    float k2;
    float k3;
	float dist;
 
	dist = distance(el.center2, el.center1);
    float3 el_dir = ray_start - el.center1;
    float3 center_norm = normalize((el.center2 - el.center1) / dist);
    k1 = 4 * pow(el.radius, 2) * dot(ray_dir, ray_dir) - 4 * pow(dist, 2) * pow(dot(ray_dir, center_norm), 2);
    k2 = 8 * pow(el.radius, 2) * dot(ray_dir, el_dir) - 4 * dot(ray_dir, center_norm) * dist * (pow(el.radius, 2) + 2 * dot(el_dir, center_norm) * dist - dist);
    k3 = 4 * pow(el.radius, 2) * dot(el_dir, el_dir) - pow((pow(el.radius, 2) + 2 * dot(el_dir, center_norm) * dist - dist), 2);
    float d = k2 * k2 - 4 * k1 * k3;
    if (d >= 0)
    {
        float t1 = (-k2 + sqrt(d)) / (2 * k1);
        float t2 = (-k2 - sqrt(d)) / (2 * k1);
        if ((t1 < t2 && t1 > 0) || (t2 < 0 && t1 >= 0))
            return (t1);
        if ((t2 < t1 && t2 > 0) || (t1 < 0 && t2 >= 0))
            return (t2);
        if (t2 == t1 && t2 >= 0)
            return (t2);
    }
    return (0);
}

float cone_intersection(t_cone cone, float3 ray_start, float3 ray_dir)
{
	float t1;
    float t2;
    float b;
    float c;

    float3 dist = ray_start - cone.position;
	float a = dot(ray_dir, cone.vec);
	a = dot(ray_dir, ray_dir) - (1 + cone.angle * cone.angle) * a * a;
    b = 2 * (dot(ray_dir, dist) - (1 + cone.angle * cone.angle) * \
		dot(ray_dir, cone.vec) * dot(dist, cone.vec));
    c = dot(dist, cone.vec);
	c = dot(dist, dist) - (1 + cone.angle * cone.angle) * c * c;
	c = b * b - 4 * a * c;
	if (c >= 0)
	{
		c = sqrt(c);
		t1 = (-b + c) / (2 * a);
        t2 = (-b - c) / (2 * a);
        if ((t1 < t2 && t1 > 0) || (t2 < 0 && t1 >= 0))
            return (t1);
        if ((t2 < t1 && t2 > 0) || (t1 < 0 && t2 >= 0))
            return (t2);
        if (t2 == t1 && t2 >= 0)
            return (t2);
	}
	return (0);
}

float cylinder_intersection(t_cylinder cyl, float3 ray_start, float3 ray_dir)
{
	float t1;
    float t2;
    float b;
    float c;

    float3 dist = ray_start - cyl.position;
	float a = dot(ray_dir, cyl.vec);
	a = dot(ray_dir, ray_dir) - a * a;
    b = 2 * (dot(ray_dir, dist) - dot(ray_dir, cyl.vec) * \
		dot(dist, cyl.vec));
    c = dot(dist, cyl.vec);
	c = dot(dist, dist) - c * c - cyl.radius * cyl.radius;
    c = b * b - 4 * a * c;
	if (c >= 0)
	{
		c = sqrt(c);
		t1 = (-b + c) / (2 * a);
        t2 = (-b - c) / (2 * a);
        if ((t1 < t2 && t1 > 0) || (t2 < 0 && t1 >= 0))
            return (t1);
        if ((t2 < t1 && t2 > 0) || (t1 < 0 && t2 >= 0))
            return (t2);
        if (t2 == t1 && t2 >= 0)
            return (t2);
	}
	return (0);
}

float sphere_intersection(t_sphere sphere, float3 ray_start, float3 ray_dir)
{
    float a = dot(ray_dir, ray_dir);
    float b;
    float c;
    float t1;
    float t2;
    float3 dist = ray_start - sphere.center;
    b = 2 * dot(dist, ray_dir);
    c = dot(dist, dist) - (sphere.radius * sphere.radius);
    c = b * b - 4 * a * c;
    if (c >= 0)
    {
        c = sqrt(c);
        t1 = (-b + c) / (2 * a);
        t2 = (-b - c) / (2 * a);
        if ((t1 < t2 && t1 > 0) || (t2 < 0 && t1 >= 0))
            return (t1);
        if ((t2 < t1 && t2 > 0) || (t1 < 0 && t2 >= 0))
            return (t2);
        if (t2 == t1 && t2 >= 0)
            return (t2);
    }
    return (0);
}

float triangle_intersection(t_triangle triangle, float3 ray_start, float3 ray_dir)
{
	float3 edge1;
	float3 edge2;
    float3 vec1;
	float3 vec2;
	float3 vec3;
    float det;
    float uv1;
	float uv2;

    edge1 = triangle.vertex[1] - triangle.vertex[0];
	edge2 = triangle.vertex[2] - triangle.vertex[0];
	vec1 = cross(ray_dir, edge2);
	det = dot(edge1, vec1);
	if (det < 1e-8 && det > -1e-8)
		return (0);
	det = 1 / det;
	vec2 = ray_start - triangle.vertex[0];
	uv1 = dot(vec2, vec1) * det;
	if (uv1 < 0 || uv1 > 1)
		return (0);
	vec3 = cross(vec2, edge1);
	uv2 = dot(ray_dir, vec3) * det;
	if (uv2 < 0 || uv1 + uv2 > 1)
		return (0);
	float res;
	res = dot(edge2, vec3) * det;
	return (res);
}

float plane_intersection(t_plane plane, float3 ray_start, float3 ray_dir)
{
	float k1;
	float k2;

    if ((dot(ray_dir, plane.normal)) == 0)
		return (0);
	k1 = dot(ray_start, plane.normal) + plane.d;
	k2 = dot(ray_dir, plane.normal);
	if (k1 == 0 || (k1 < 0 && k2 < 0) || (k1 > 0 && k2 > 0))
		return (0);
	k1 = -k1 / k2;
	return (k1);
}

int			in_shadow(int index, float3 l, __global float3 *intersection_buf, \
						int obj_nmb, __global t_object_d *obj)
{
	float3	ray_start;
	int		i;
	float	t;

	i = 0;
	ray_start = l * 0.001f;
	ray_start += intersection_buf[index];
	while (i < obj_nmb)
	{
		if (obj[i].type == SPHERE)
			t = sphere_intersection(obj[i].primitive.sphere, ray_start, l);
		if (obj[i].type == TRIANGLE)
			t = triangle_intersection(obj[i].primitive.triangle, ray_start, l);
		if (obj[i].type == PLANE)
			t = plane_intersection(obj[i].primitive.plane, ray_start, l);
		if (obj[i].type == CONE)
			t = cone_intersection(obj[i].primitive.cone, ray_start, l);
		if (obj[i].type == CYLINDER)
			t = cylinder_intersection(obj[i].primitive.cylinder, ray_start, l);
		if (obj[i].type == ELLIPSOID)
			t = ellipsoid_intersection(obj[i].primitive.ellipsoid, ray_start, l);
		if (obj[i].type == BOX)
			t = box_intersection(&obj[i].primitive.box, ray_start, l);
		if (obj[i].type == PARABOLOID)
			t = paraboloid_intersection(obj[i].primitive.paraboloid, ray_start, l);
		/* if (obj[i].type == TORUS)
			t = torus_intersection(obj[i].primitive.torus, ray_start, l); */
		if (t < 1.0f && t > 0.00001f)
			break ;
		i++;
	}
	if (t < 1.0f && t > 0.0001f)
		return (1);
	return (0);
}

float		get_specular(int index, int j, float3 l, \
						__global float3 *normal_buf, \
						__global t_light *light, \
 						__global t_material *material_buf, \
						__global float3 *ray_buf)
{
	float		n;
	float		r;
	float		i;
 	float3		rad;
	float3		d;
	float3		lb;

	lb = l / length(l);
	i = 0;
	n = dot(normal_buf[index], lb);
	rad = normal_buf[index] * 2.0f;
	rad = rad * n;
	rad = rad - lb;
	d.x = -ray_buf[index].x;
	d.y = -ray_buf[index].y;
	d.z = -ray_buf[index].z;
	r = dot(rad, d);
	if (r > 0)
		i += light[j].intensity * pow((float)r / \
		(length(rad) * length(d)), \
		material_buf[index].specular);
	return (i);
}

float3		get_light_vec(int index, int j, __global float3 *intersection_buf, __global t_light *light)
{
	float3 light_vec;

	light_vec = 0;
	if (light[j].type == POINT)
		light_vec = light[j].position - intersection_buf[index];
	if (light[j].type == DIRECTIONAL)
		light_vec = light[j].direction;
	return (light_vec);
}

t_color		reflection_color(__global t_color *frame_buf, \
							__global float3 *ray_buf, \
                            __global float3 *intersection_buf, \
                            __global float3 *normal_buf, \
                            __global int *index_buf, \
                            __global t_material *material_buf, \
                            __global t_object_d *obj, \
                            __global t_light *light, \
							int light_nmb, \
							int index, int obj_nmb, int bounce_cnt, \
							__global t_material *prev_material_buf)
{
	float		i;
	float3		l;
	float		n_dot_l;
	int			j;

	j = -1;
	i = 0;
	while (++j < light_nmb)
	{
		if (light[j].type == AMBIENT)
			i += light[j].intensity;
		else
		{
			l = get_light_vec(index, j, intersection_buf, light);
			n_dot_l = dot(normal_buf[index], l);
			if (!(in_shadow(index, l, intersection_buf, obj_nmb, obj)) && n_dot_l > 0)
			{
				if (material_buf[index].specular != -1)
					i += get_specular(index, j, l, normal_buf, light, material_buf, ray_buf);
				i += (light[j].intensity * n_dot_l) / length(l);
			}
		}
	}
	i = i > 1 ? 1 : i;
	t_color result;
	result.red = material_buf[index].color.red * i;
	result.green = material_buf[index].color.green * i;
	result.blue = material_buf[index].color.blue * i;
	if (bounce_cnt > 0)
	{
		result.red = (1 - prev_material_buf[index].reflection) * result.red + prev_material_buf[index].reflection * frame_buf[index].red;
		result.green = (1 - prev_material_buf[index].reflection) * result.green + prev_material_buf[index].reflection * frame_buf[index].green;
		result.blue = (1 - prev_material_buf[index].reflection) * result.blue + prev_material_buf[index].reflection * frame_buf[index].blue;
	}
	if (index == 1280 * 360 + 640 * 11)
		printf("result color device in reflection_color (%hhu, %hhu, %hhu)\n", result.red, result.green, result.blue);
	return (result);
}

void reflect(__global float3 *normal_buf, int i, __global float3 *ray_buf)
{
	float buf;
	float3 buf2;

	buf = dot(ray_buf[i], normal_buf[i]);
	buf2.x = 2 * normal_buf[i].x * buf;
	buf2.y = 2 * normal_buf[i].y * buf;
	buf2.z = 2 * normal_buf[i].z * buf;
	normal_buf[i] = ray_buf[i] - buf2;
}

__kernel void get_frame_buf_cl(__global t_color *frame_buf, \
                            __global float3 *ray_buf, \
                            __global float3 *intersection_buf, \
                            __global float3 *normal_buf, \
                            __global int *index_buf, \
                            __global t_material *material_buf, \
                            __global t_object_d *obj, \
                            __global t_light *light, \
                            int light_nmb,\
							int obj_nmb, int bounce_cnt, \
							__global t_material *prev_material_buf, \
							int is_refractive)
{
    int i = get_global_id(0);
	int j = index_buf[i];
	if (j != -1)
	{
		frame_buf[i] = reflection_color(frame_buf, ray_buf, intersection_buf, \
										normal_buf, index_buf, material_buf, \
										obj, light, light_nmb, i, obj_nmb, bounce_cnt, prev_material_buf);
	}
	else if (bounce_cnt == 0)
	{
		frame_buf[i].red = 0;
		frame_buf[i].green = 0;
		frame_buf[i].blue = 0;
		frame_buf[i].alpha = 255;
	}
	reflect(normal_buf, i, ray_buf);
}
