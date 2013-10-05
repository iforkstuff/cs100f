#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <iostream>

struct vec3 {
    float x, y, z;

    vec3() : x(0), y(0), z(0) { }
    vec3(float s) : x(s), y(s), z(s) { }
    vec3(float xVal, float yVal, float zVal) : x(xVal), y(yVal), z(zVal) { }
    vec3(float* values) : x(values[0]), y(values[1]), z(values[2]) { }

    // Insert the contents of this vector3 into a 3-float array
    inline void toFloatArray(float* ary) const
    {
        ary[0] = x;
        ary[1] = y;
        ary[2] = z;
    }
    
    //! Assignment operator.
    /*!
        Assigns the values from \a a to this Vec3.
    */
    const vec3 & operator=(const vec3& a) {
        x = a.x;
        y = a.y;
        z = a.z;
        return *this;
    }
    
    //! Assignment operator.
    /*!
        Sets all components of this Vec3 to \a a.
    */
    const vec3 & operator=(float a) { x = y = z = a; return *this; }
    void set(float a) { x = y = z = a; }
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    void set(const vec3 & v) { x = v.x; y = v.y; z = v.z; }
    
    //! Access operator.        
    /*!
        Returns the ith component of the vector.
        \param i The component to return.
        \warning i must be either 0, 1, or 2 in order to get expected results.
    */
    float & operator[](int i) { return (&x)[i]; }
    
    //! Constant access operator.
    /*!
        Returns the ith component of a constant vector.
        \param i The component to return.
        \warning i must be either 0, 1, or 2 in order to get expected results.
    */
    const float & operator[](int i) const { return (&x)[i]; }


    //! Component-wise vector addition operator.
    vec3 operator+(const vec3& v) const {
        return vec3(x + v.x, y + v.y, z + v.z);
    }
    
    //! Component-wise vector addition-assignment operator.
    const vec3 & operator+=(const vec3& v) {
        x += v.x; y += v.y; z += v.z; return *this;
    }

    //! Scalar addition-assignment operator.
    const vec3 & operator+=(float a) {
        x += a; y += a; z += a; return *this;
    }

    //! Component-wise vector subtraction operator.
    vec3 operator-(const vec3& v) const {
        return vec3(x - v.x, y - v.y, z - v.z);
    }
    
    //! Component-wise vector subtraction-assignment operator.
    const vec3 & operator-=(const vec3& v) {
        x -= v.x; y -= v.y; z -= v.z; return *this;
    }
    
    //! Component-wise scalar subtraction assignment operator.
    const vec3 & operator-=(float a) {
        x -= a; y -= a; z -= a; return *this;
    }

    //! Scalar multiplication operator.
    vec3 operator*(float a) const {
        return vec3(x * a, y * a, z * a);
    }
    
    //! Component-wise vector multiplication operator.
    vec3 operator*(const vec3& v) const {
        return vec3(x * v.x, y * v.y, z * v.z);
    }
    
    //! Scalar multiplication-assignment operator.
    const vec3 & operator*=(float a) {
        x *= a; y *= a; z *= a; return *this;
    }
    
    //! Component-wise vector multiplication-assignment operator.
    const vec3 & operator*=(const vec3& v) {
        x *= v.x; y *= v.y; z *= v.z; return *this;
    }
    
    //! Negation operator.
    vec3 operator-() const {
        return vec3(-x, -y, -z);
    }
    const vec3 & negate() { x = -x; y = -y; z = -z; return *this; }


    //! Scalar division operator.
    vec3 operator/(float a) const {
        float inv = float(1) / a;
        return vec3(x * inv, y * inv, z * inv);
    }
    
    //! Component-wise vector division operator.
    vec3 operator/(const vec3 & v) const {
        return vec3(x / v.x, y / v.y, z / v.z);
    }
    
    //! Scalar division-assignment operator.
    const vec3 & operator/=(float a) {
        float inv = float(1) / a;
        x *= inv; y *= inv; z *= inv;
        return *this;
    }
    
    //! Component-wise vector division-assignment operator.
    const vec3 & operator/=(const vec3 & v) {
        x /= v.x; y /= v.y; z /= v.z; return *this;
    }

    //! Vector equivalence operator.
    /*!
        Tests to see if each component of \a v is equal to each component of
        this vec3.
    */
    bool operator==(const vec3 & v) const {
        return(v.x == x && v.y == y && v.z == z);
    }

    //! Vector difference operator.
    /*!
        Tests to see if any component is different between the two Vec3s.
    */
    bool operator!=(const vec3 & v) const {
        return(v.x != x || v.y != y || v.z != z);
    }

    //! Length<sup>2</sup>.
    /*!
        Returns the geometric length<sup>2</sup> of the vector.
    */
    float length2() const;
    
    //! Length.
    /*!
        Returns the geometric length of the vector.
    */
    float length() const { return sqrtf(length2()); }
    
    //! Normalizes the vector and return its length.
    /*!
        Scales each component of the vector in order to get unit
        length without changing direction.
    
        \return The length of the vector prior to normalization.
    */
    float unitize() {
        float l = length();
        *this /= l;
        return l;
    }
    
    //! Normalize a vector and return a reference to it.
    /*!
        Scales each component of the vector in order to get unit
        length without changing direction.
    
        \return A reference to the vector.
    */
    const vec3 & normalize() { return (*this /= length()); }
    
    //! Return a normalized copy of the vector.
    vec3 normalized() const { return( *this / length()); }
    
    //! Return a rotated copy of the vector
    vec3 rotated(float theta, const vec3 & w) const;
    
    //! Rotate this vector about another vector, w, by theta radians.
    const vec3 & rotate(float theta, const vec3 & w) {
	return *this = rotated(theta, w);
    }
};


//! Multiply a scalar by a Vec3.
inline vec3 operator*(float s, const vec3& v) {
    return vec3(v.x * s, v.y * s, v.z * s);
}


//! The dot product of two Vec3s.
inline float dot(const vec3 & a, const vec3 & b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}


//! The cross product of two Vec3s.
inline vec3 cross(const vec3 & a, const vec3 & b) {
    return vec3(a.y * b.z - a.z * b.y,
                   a.z * b.x - a.x * b.z,
                   a.x * b.y - a.y * b.x);
}

inline vec3 multiplyComponents (const vec3 & a, const vec3 & b) {
    return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

inline float vec3::length2() const {
    return dot(*this, *this);
}

//! Return a rotated copy of the vector
inline vec3 vec3::rotated(float theta, const vec3 & w) const {
    float c = cosf(theta);
    float s = sinf(theta);
    vec3 v0 = dot(*this, w) * w;
    vec3 v1 = *this - v0;
    vec3 v2 = cross(w, v1);
    return v0 + c * v1 + s * v2;
}

inline std::ostream & operator<<(std::ostream& out, const vec3& v) {
    return out << v.x << " " << v.y << " " << v.z ;
}

inline static float sqr(const float x)  { return x * x; }

#endif // CSE168_VECTOR3_H_INCLUDED
