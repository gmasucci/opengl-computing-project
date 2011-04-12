#ifndef VEC3_H
#define VEC3_H
#include <cmath>

class Vec3
{
public:
	//access to members public fttb;
	double x;
	double y;
	double z;

public:
	//init list;
	Vec3(double a=0.0, double b=0.0, double c=0.0): x(a), y(b), z(c) {}
	Vec3(Vec3 &vec){x = vec.x;y = vec.y;z = vec.z;}
	double length(){return std::sqrt(x*x+y*y+z*z);}
	const Vec3 operator+(const Vec3 &rhs) const;
	const Vec3& operator=(const Vec3 &rhs);
	const Vec3& operator+=(const Vec3 &rhs);
	const Vec3 operator*(const double rhs) const;
};

#endif