
#ifndef DA_QUATERNION_H
#define DA_QUATERNION_H
#include <cmath>
#include "grid.h"

namespace DA
{
	class Quaternion
	{
	public:
		Quaternion(void);								//	bare constructor
		Quaternion(float x, float y, float z, float w);	//	fully qualified constructor
		~Quaternion(void);								//	simple destructor
		/*
		will need a simple storage method for some temp values . . . 
		lets make it an array of floats, and as quaternions have an x,y,z,w
		*/

		float store[4];

		/*
		quaternion multiplication differs from the inbuilt
		so better overload that now . . .

		Just need to remember that quaternion multiplication is
		non commutative as well, or I will run into problems
		*/
		Quaternion operator *(const Quaternion &quat) const;
		//	ohh, and the shortcut *= operator too . . . 
		Quaternion operator *=(const Quaternion &quat);
		
		//	end of overloaded maths.
		/*	spherical-linear interpolation, also known as slerp,
		refers to constant speed motion along a unit radius great circle arc,
		given the ends and an interpolation parameter between 0 and 1  
		( introduced by Ken Shoemake in the context of quaternion 
		interpolation for the purpose of animating 3D rotation)
		NB.:	this is probably the maths name that most tickles my fancy at the moment :^)
		*/
		static Quaternion slerp(const Quaternion &q1, const Quaternion &q2, float t);
		
		
		/*
		well, for gl I cant use quaternions, unless I happen to fancy rewriting
		the GL api  (not today i dont think . . .); so, in the wave of a magic wand . . .
		*/
		void toMatrix(float matrix[]) const;		// ohh yeah, this makes it into a 4x4 matrix, just like GL likes

		/*
		in case the quaternion is nor a standard unit length, need a way to calculate its size/magnitude/length
		and after that may well want to normalise it too . . . 
		*/
		double length() const;	//	modifies nothing so may as well const this, however cpu intensive
		float lengthf() const;	//	returns the length as a float
		double	lengthSquared() const;	//	returns the length squared, less cpu intensive by far:)
		float	lengthSquaredf() const;	//	as above but as a float
		void normalise();

		/*
		another handy thing to do is raise a quaternion to a power
		*/
		Quaternion power(float pow);

		/*
		couple of handy inlines to make everything a whole load simpler,
		ohh and theres the minor thing of making the operator work the way it should
		for quaternions ;^)
		*/	

		inline float& operator [] (int i)       { return store[i]; }
		inline float  operator [] (int i) const { return store[i]; }

		/*

		from http://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation

		conjugation of a quaternion results in the values representing a 
		clockwise rotation if we look along the same vector as described
		by the axis v-> (which is equal to i + j + k or, x + y + z,)
		so with that in mind it may be a good idea to turn it into a method/function
		*/

		inline Quaternion conjugate() const { return Quaternion(-store[0], -store[1], -store[2], store[3]); }
		/*
		and a supplemental function too, to allow us to reverse engineer a quaternion
		from a supplied vector and angle
		*/
		void fromVectorAngle(const float vector[], float angle);
	};
}

#endif