#ifndef VEC3_H_
#define VEC3_H_
#include <iostream>
#include <cmath>
#include <cstdlib>
using std::istream;
using std::ostream;

class vec3 {
public:
    vec3() : x(e[0]), y(e[1]), z(e[2]), r(e[0]), g(e[1]), b(e[2]) {}
    vec3(float e0, float e1, float e2) :
        x(e[0]), y(e[1]), z(e[2]), r(e[0]), g(e[1]), b(e[2])
        { e[0] = e0; e[1] = e1; e[2] = e2; }
    vec3(const vec3& v) : // Copy constructor is needed because there is a custom copy-assignment operator
        x(e[0]), y(e[1]), z(e[2]), r(e[0]), g(e[1]), b(e[2])
        { e[0] = v.e[0]; e[1] = v.e[1]; e[2] = v.e[2]; }

    vec3& operator=(const vec3 &v) { // Default copy assignment is deleted due to reference type member
        e[0] = v.e[0]; e[1] = v.e[1]; e[2] = v.e[2]; return *this;
    }
    ////////////////////////////////////////////
    // Obsolete: change function to reference //
    // inline float x() const { return e[0]; }//
    // inline float y() const { return e[1]; }//
    // inline float z() const { return e[2]; }//
    // inline float r() const { return e[0]; }//
    // inline float g() const { return e[1]; }//
    // inline float b() const { return e[2]; }//
    ////////////////////////////////////////////
    inline float operator[](int i) const { return e[i]; }
    inline float& operator[](int i) { return e[i]; }

    inline const vec3& operator+() const { return *this; }
    inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    inline vec3& operator+=(const vec3& v2);
    inline vec3& operator-=(const vec3& v2);
    inline vec3& operator*=(const vec3& v2);
    inline vec3& operator/=(const vec3& v2);
    inline vec3& operator+=(const float t);
    inline vec3& operator-=(const float t);
    inline vec3& operator*=(const float t);
    inline vec3& operator/=(const float t);

    inline float length_sqaure() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }
    inline float length() const { return sqrt(length_sqaure()); }

    friend istream& operator>>(istream& is, vec3& v);
    friend ostream& operator<<(ostream& os, const vec3& v);
    friend vec3 max(const vec3& v1, const vec3& v2);
    friend vec3 min(const vec3& v1, const vec3& v2);
    friend vec3 sqrt(const vec3& v);
    
    inline void normalize();
    float e[3];
    float &x;
    float &y;
    float &z;
    float &r;
    float &g;
    float &b;
};

inline vec3& vec3::operator+=(const vec3& v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}

inline vec3& vec3::operator-=(const vec3& v) {
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    return *this;
}

inline vec3& vec3::operator*=(const vec3& v) {
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    return *this;
}

inline vec3& vec3::operator/=(const vec3& v) {
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    return *this;
}

inline vec3& vec3::operator+=(const float t) {
    e[0] += t;
    e[1] += t;
    e[2] += t;
    return *this;
}

inline vec3& vec3::operator-=(const float t) {
    e[0] -= t;
    e[1] -= t;
    e[2] -= t;
    return *this;
}

inline vec3& vec3::operator*=(const float t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

inline vec3& vec3::operator/=(const float t) {
    e[0] /= t;
    e[1] /= t;
    e[2] /= t;
    return *this;
}

inline void vec3::normalize() {
    *this /= this->length();
}

inline istream& operator>>(istream& is, vec3& v) {
    is >> v.e[0] >> v.e[1] >> v.e[2];
    return is;
}

inline ostream& operator<<(ostream& os, const vec3& v) {
    os << v.e[0] << " " << v.e[1] << " " << v.e[2];
    return os;
}

inline vec3 operator+(const vec3& v1, const vec3& v2) {
    return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3& v1, const vec3& v2) {
    return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(const vec3& v1, const vec3& v2) {
    return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator/(const vec3& v1, const vec3& v2) {
    return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline vec3 operator+(const vec3& v, const float t) {
    return vec3(v.e[0] + t, v.e[1] + t, v.e[2] + t);
}

inline vec3 operator+(const float t, const vec3& v) {
    return vec3(t + v.e[0], t + v.e[1], t + v.e[2]);
}

inline vec3 operator-(const vec3& v, const float t) {
    return vec3(v.e[0] - t, v.e[1] - t, v.e[2] - t);
}

inline vec3 operator-(const float t, const vec3& v) {
    return vec3(t - v.e[0], t - v.e[1], t - v.e[2]);
}

inline vec3 operator*(const vec3& v, const float t) {
    return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}

inline vec3 operator*(const float t, const vec3& v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator/(const vec3& v, const float t) {
    return vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline vec3 operator/(const float t, const vec3& v) {
    return vec3(t / v.e[0], t / v.e[1], t / v.e[2]);
}

inline float dot(const vec3& v1, const vec3& v2) {
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3& v1, const vec3& v2) {
    return vec3( v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
                -v1.e[0] * v2.e[2] + v1.e[2] * v2.e[0],
                 v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}

inline vec3 normalized(const vec3 v) {
    return v / v.length();
}

inline
vec3 max(const vec3& v1, const vec3& v2) {
    return vec3(fmax(v1.e[0], v2.e[0]), fmax(v1.e[1], v2.e[1]), fmax(v1.e[2], v2.e[2]));
}

inline
vec3 min(const vec3& v1, const vec3& v2) {
    return vec3(fmin(v1.e[0], v2.e[0]), fmin(v1.e[1], v2.e[1]), fmin(v1.e[2], v2.e[2]));
}

inline
vec3 sqrt(const vec3& v) {
    return vec3(sqrt(v.e[0]), sqrt(v.e[1]), sqrt(v.e[2]));
}

class vec3i
{
public:
    vec3i();
    vec3i(int e0, int e1, int e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
    vec3i(const vec3& v) { e[0] = int(v.e[0]); e[1] = int(v.e[1]); e[2] = int(v.e[2]); }

    friend istream& operator>>(istream& is, vec3i& v);
    friend ostream& operator<<(ostream& os, const vec3i& v);
    
    int e[3];
};

inline istream& operator>>(istream& is, vec3i& v) {
    is >> v.e[0] >> v.e[1] >> v.e[2];
    return is;
}

inline ostream& operator<<(ostream& os, const vec3i& v) {
    os << v.e[0] << " " << v.e[1] << " " << v.e[2];
    return os;
}

#endif
