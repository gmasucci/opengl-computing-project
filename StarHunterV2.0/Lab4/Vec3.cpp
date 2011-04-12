#include "Vec3.h"

const Vec3 Vec3::operator+(const Vec3 &rhs) const{
	Vec3 tmp(x+rhs.x,y+rhs.y,z+rhs.z);
	return tmp;
}

const Vec3& Vec3::operator=(const Vec3 &rhs){
	
	Vec3 tmp(x=rhs.x,y=rhs.y,z=rhs.z);
	return tmp;

}

const Vec3& Vec3::operator+=(const Vec3 &rhs){
	
	x+=rhs.x;
	y+=rhs.y;
	z+=rhs.z;
	return *this;

}


const Vec3 Vec3::operator*(const double rhs) const{
	Vec3 tmp(x*rhs,y*rhs,z*rhs);
	return tmp;

}