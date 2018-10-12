#ifndef __SMVECTORF
#define __SMVECTORF

#include <stdint.h>

#ifdef __WINDOWS__
#include <cmath>
#else
#include <math.h>
#endif

/*  Vectorf contains the following floating point functions:-

	vec2f		x,y vector
	vec3f		x,y,z vector
	vec4f		x,y,z,w vector

*/

///// vec2f class ///////////////////////////////////////////////////////////////////////////////////////

class vec2f {
public:
	vec2f() { x = 0.f; y = 0.f; }
	vec2f(float _x, float _y) { x = _x; y = _y; }

	void Normalise()
	{
		float v = sqrtf(x*x + y*y);
		x = x / v;
		y = y / v;
	}

	vec2f operator - () const
	{
		return vec2f(-x, -y);
	}

	vec2f operator - (vec2f rhs)
	{
		return vec2f(x - rhs.x, y - rhs.y);
	}

	vec2f operator + (vec2f rhs)
	{
		return vec2f(x + rhs.x, y + rhs.y);
	}

	bool operator == (vec2f rhs)
	{
		return (x == rhs.x && y == rhs.y);
	}

	bool operator!=(vec2f rhs)
	{
		return (x != rhs.x || y != rhs.y);
	}

	vec2f operator*(float scale)
	{
		return vec2f(x * scale, y * scale);
	}

	vec2f operator/(float div)
	{
		return vec2f(x / div, y / div);
	}

	vec2f & operator += (vec2f const & v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	vec2f & operator -= (vec2f const & v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	vec2f & operator *= (float v)
	{
		x *= v;
		y *= v;
		return *this;
	}

	float GetLength() const
	{
		return sqrtf(x*x + y*y);
	}


	static vec2f g_Zero;

	float x;
	float y;
};

///// vec3f class ///////////////////////////////////////////////////////////////////////////////////////

class vec3f {
public:
	vec3f() : x(0.f), y(0.f), z(0.f) {
	}

	vec3f(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }

	inline vec3f operator - ()
	{
		return vec3f(-x, -y, -z);
	}

	inline vec3f operator - (vec3f rhs)
	{
		return vec3f(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	inline vec3f operator + (vec3f rhs)
	{
		return vec3f(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	inline vec3f & operator += (vec3f const & v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	inline vec3f & operator -= (vec3f const & v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	inline bool operator==(vec3f rhs)
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z);
	}

	inline vec3f operator * (float scale)
	{
		return vec3f(x * scale, y * scale, z * scale);
	}

	inline vec3f operator * (vec3f rhs)
	{
		return vec3f(x * rhs.x, y * rhs.y, z * rhs.z);
	}

	//inline float operator * (vec3f t) // scalar product
	//{
	//	return x*t.x + y*t.y + z*t.z;
	//}
	
	inline vec3f operator/(float div)
	{
		return vec3f(x / div, y / div, z / div);
	}

	inline vec3f operator ^ (vec3f t) // cross product
	{
		return vec3f(y*t.z - z*t.y, t.x*z - x*t.z, x*t.y - y*t.x);
	}

	inline float length()
	{
		return sqrt(x*x + y*y + z*z);
	}

	inline vec3f unit() // normalized to a length of 1
	{
		float l = length();
		if (l == 0.0) return vec3f(0.0, 0.0, 0.0);
		return vec3f(x / l, y / l, z / l);
	}

	inline bool IsZero() const
	{
		return ((x == 0.f) && (y == 0.f) && (z == 0.f));
	}

	//static vec3f g_Zero;

	float x;
	float y;
	float z;
};

///// vec3f class ///////////////////////////////////////////////////////////////////////////////////////

class vec4f {
public:
	vec4f() : x(0.f), y(0.f), z(0.f), w(0.f) {
	}

	vec4f(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; }

	inline vec4f operator - ()
	{
		return vec4f(-x, -y, -z, -w);
	}

	inline vec4f operator - (vec4f rhs)
	{
		return vec4f(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	inline vec4f operator + (vec4f rhs)
	{
		return vec4f(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	inline vec4f & operator += (vec4f const & v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	inline bool operator==(vec4f rhs)
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
	}

	inline vec4f operator*(float scale)
	{
		return vec4f(x * scale, y * scale, z * scale, w * scale);
	}

	inline float operator * (vec4f t) // scalar product
	{
		return x*t.x + y*t.y + z*t.z + w*t.w;
	}

	inline vec4f operator/(float div)
	{
		return vec4f(x / div, y / div, z / div, w / div);
	}

	inline float length()
	{
		return sqrtf(x*x + y*y + z*z + w*w);
	}

	inline vec4f unit() // normalized to a length of 1
	{
		float l = length();
		if (l == 0.0f) return vec4f(0.0f, 0.0f, 0.0f, 0.0f);
		return vec4f(x / l, y / l, z / l, w / l);
	}

	inline bool IsZero() const
	{
		return ((x == 0.f) && (y == 0.f) && (z == 0.f) && (w == 0.f));
	}

	//static vec4f g_Zero;

	float x;
	float y;
	float z;
	float w;
};
#endif

