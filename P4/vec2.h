#ifndef VEC2_H
#define VEC2_H

struct vec2 {
	float x, y;

	vec2(){}
	vec2(const float ixy){
		x = y = ixy;
	}
	vec2(const float ix, const float iy){
		x = ix;
		y = iy;
	}
	//operator const float *() const { return &x; }
	operator float *() const { return (float *) &x; }
//	float &operator[](unsigned int index){ return *(((float *) &x) + index); }

	void operator += (const float s);
	void operator += (const vec2 &v);
	void operator -= (const float s);
	void operator -= (const vec2 &v);
	void operator *= (const float s);
	void operator *= (const vec2 &v);
	void operator /= (const float s);
	void operator /= (const vec2 &v);
};

vec2 operator + (const vec2 &u, const vec2 &v);
vec2 operator + (const vec2 &v, const float s);
vec2 operator + (const float s, const vec2 &v);

vec2 operator - (const vec2 &u, const vec2 &v);
vec2 operator - (const vec2 &v, const float s);
vec2 operator - (const float s, const vec2 &v);

vec2 operator - (const vec2 &v);

vec2 operator * (const vec2 &u, const vec2 &v);
vec2 operator * (const float s, const vec2 &v);
vec2 operator * (const vec2 &v, const float s);

vec2 operator / (const vec2 &u, const vec2 &v);
vec2 operator / (const vec2 &v, const float s);
vec2 operator / (const float s, const vec2 &v);

bool operator == (const vec2 &u, const vec2 &v);

#endif
