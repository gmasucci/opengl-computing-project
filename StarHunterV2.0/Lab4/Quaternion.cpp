#include "Quaternion.h"

DA::Quaternion::Quaternion(void)
{
	store[3]	=	1.0f;
	store[0]	=	store[1]	=	store[2]	=	0.0f;
}

DA::Quaternion::Quaternion(float x, float y, float z, float w)
{
	store[0]	=	x;
	store[1]	=	y;
	store[2]	=	z;
	store[3]	=	w;
}

DA::Quaternion::~Quaternion(void)
{
}


/*
ok so there may be times when we want to multiply a quaternion by another
so . . . 
*/
DA::Quaternion	DA::Quaternion::operator *(const Quaternion &quatIn) const {
   Quaternion returner;

   returner.store[0] = store[3]*quatIn.store[0] + store[0]*quatIn.store[3] + store[1]*quatIn.store[2] - store[2]*quatIn.store[1];
   returner.store[1] = store[3]*quatIn.store[1] + store[1]*quatIn.store[3] + store[2]*quatIn.store[0] - store[0]*quatIn.store[2];
   returner.store[2] = store[3]*quatIn.store[2] + store[2]*quatIn.store[3] + store[0]*quatIn.store[1] - store[1]*quatIn.store[0];
   returner.store[3] = store[3]*quatIn.store[3] - store[0]*quatIn.store[0] - store[1]*quatIn.store[1] - store[2]*quatIn.store[2];

   return returner;
}
/*
sometimes may want tio make a quaternion eual to itself times another quaternion
therefore we overload this too and we get  . . 
*/
DA::Quaternion	DA::Quaternion::operator *=(const Quaternion &quat)
{
	/*	need some temp storage for the values we are calculating,
		as I neeed to re-use them a few times each before i can return any values,
		so . . .
	*/
	float temp[4];	//	where 0=x, 1=y, 2=z, 3=w

	temp[0] = store[3]*quat.store[0] + store[0]*quat.store[3] + store[1]*quat.store[2] - store[2]*quat.store[1];
	temp[1] = store[3]*quat.store[1] + store[1]*quat.store[3] + store[2]*quat.store[0] - store[0]*quat.store[2];
	temp[2] = store[3]*quat.store[2] + store[2]*quat.store[3] + store[0]*quat.store[1] - store[1]*quat.store[0];
	temp[3] = store[3]*quat.store[3] - store[0]*quat.store[0] - store[1]*quat.store[1] - store[2]*quat.store[2];

	store[0]=temp[0];
	store[1]=temp[1];
	store[2]=temp[2];
	store[3]=temp[3];

	return (*this);		
	/*	i am sure i could have done this whole function more elegantly, 
	but this works and i need to know a whole lot more about quaternions
	before i will brave it :^)
	*/
}

double	DA::Quaternion::lengthSquared() const
{
	return store[0]*store[0]	+	store[1]*store[1]	+	store[2]*store[2]	+	store[3]*store[3];
}

float	DA::Quaternion::lengthSquaredf() const		{	return (float)lengthSquared();			}

double	DA::Quaternion::length()	const			{	return sqrt(lengthSquared());			}
float	DA::Quaternion::lengthf()	const			{	return (float)sqrt(lengthSquared());	}


void	DA::Quaternion::normalise()
{ 
	//	always handy to limit the number of function calls
	// to that end a handy variable will serve
   float length = lengthf();
   store[0] /= length;
   store[1] /= length;
   store[2] /= length;
   store[3] /= length;
}


void	DA::Quaternion::toMatrix(float matrix[]) const
{
	/*
	using the following logic we can make a matrix from the quaternion
	            2     2                                 
        | 1 - 2Y  - 2Z    2XY - 2ZW      2XZ + 2YW     |
        |                                              |
        |                       2     2                |
    M = | 2XY + 2ZW       1 - 2X  - 2Z   2YZ - 2XW     |
        |                                              |
        |                                      2     2 |
        | 2XZ - 2YW       2YZ + 2XW      1 - 2X  - 2Y  |

	so here goes . . .	:^)
	thats for a 3 * 3, for gl we need a 4*4 matrix . . .
	so we can add in the last few bits like so
	positions of the 4x4 matrix detailed below 
		0	4	8	12
		1	5	9	13
		2	6	10	14
		3	7	11	15

		to take the 3x3 and make a 4 x 4 we just need to make a few insertions
		position		value
		12 to 14		0
		15				1
		3, 7, 11		0
		and thats a whole 4x4 matrix filled
	*/

  matrix[ 0] = 1.0f - 2.0f * ( store[1] * store[1] + store[2] * store[2] ); 
  matrix[ 1] = 2.0f * (store[0] * store[1] + store[2] * store[3]);
  matrix[ 2] = 2.0f * (store[0] * store[2] - store[1] * store[3]);
    
  matrix[ 4] = 2.0f * ( store[0] * store[1] - store[2] * store[3] );  
  matrix[ 5] = 1.0f - 2.0f * ( store[0] * store[0] + store[2] * store[2] ); 
  matrix[ 6] = 2.0f * (store[2] * store[1] + store[0] * store[3] );  
    
  matrix[ 8] = 2.0f * ( store[0] * store[2] + store[1] * store[3] );
  matrix[ 9] = 2.0f * ( store[1] * store[2] - store[0] * store[3] );
  matrix[10] = 1.0f - 2.0f * ( store[0] * store[0] + store[1] * store[1] );  
    
  //	lets set all the 0.0f matrix values now . . . 
  matrix[3] = matrix[7] = matrix[11] = matrix[12] = matrix[13] = matrix[14] = 0.0f;
  //	and now the singular 1.0f set value . . . 
  matrix[15] = 1.0f;

}



DA::Quaternion	DA::Quaternion::power(float pow)
{
	Quaternion retval = (*this);
	//	ok, lets use fabs (floatabsolute) to check for value ranges
	//	(this means i dont need to worry about the sign:^) . . . though i do like ORed ifs as well, may chage this
	if ( fabs(double(store[13])) < 0.9999 )
	{
		float alpha1 = (float)acos(double(store[3]));
		float alpha2 = alpha1	*	pow;
		float factor = float( sin(double(alpha2)) / sin(double(alpha1)) );

		retval.store[0] *= factor;
		retval.store[1] *= factor;
		retval.store[2] *= factor;
		retval.store[3] = (float)cos( double(alpha2) );
	}
	return retval;
}


/*
reverse engineer a quaternion based on vector + angle being
supplied by the user
*/
void DA::Quaternion::fromVectorAngle(const float vector[], float angle)
{
   //	convert the supplied degrees angle to radians
   float rads = (angle/180.0f)*3.1415;
   //	for lowered cpu usage, lets do it once and store it
   float halfSinTheta = (float)sin( double(rads/2.0f) );
	// now i can populate the quaternion with the appropriate data
   store[0] = vector[0] * halfSinTheta;
   store[1] = vector[1] * halfSinTheta;
   store[2] = vector[2] * halfSinTheta;
   store[3] = (float)cos( double(rads/2.0f) );
}


/*
ok heres where my knowledge runs short so I will do my best
quat 1 is the start quaternion, quat 2 is the end value we want to achieve
after interpolation, and  0<=t<=1  (our interpolation value)

and heres the defined 3d use:
When Slerp is applied to unit quaternions, the quaternion path maps to a path through 3D rotations in a standard way. 
The effect is a rotation with uniform angular velocity around a fixed rotation axis

which is pretty much what we want for model use
*/
DA::Quaternion DA::Quaternion::slerp(const Quaternion &quat1, const Quaternion &quat2, float t)
{
  Quaternion retval, _quat2 = quat2;

  //	lets start storing some handy values for reuse later . . .
  float cosOmega = quat1.store[3]*quat2.store[3] + quat1.store[0]*quat2.store[0] + quat1.store[1]*quat2.store[1] + quat1.store[2]*quat2.store[2];
  //	a couple of constants too
  float k0, k1;

  //	in the event of coOmega being negative
  if ( cosOmega < 0.0f )
  {
    _quat2.store[0] = -_quat2.store[0];
    _quat2.store[1] = -_quat2.store[1];
    _quat2.store[2] = -_quat2.store[2];
    _quat2.store[3] = -_quat2.store[3];
    cosOmega = -cosOmega;
  }
  //	now we can reduce some truncation errors and wasted computation as well
  if ( cosOmega > 0.99999f )
  {
    k0 = 1.0f - t;
    k1 = t;
  }
  //	otherwise we can just go about our normal daily business and interpolate away . . .
  else
  {
    float sinOmega = (float)sqrt( double(1.0f - cosOmega*cosOmega) );
    float omega = (float)atan2( double(sinOmega), double(cosOmega) );
    float invSinOmega = 1.0f/sinOmega;

    k0 = float( sin(double((1.0f - t)*omega)) )*invSinOmega;
    k1 = float( sin(double(t*omega)) )*invSinOmega;
  }
  //	lets calculate the overall effect of Slerping	(gotta love the name:^)
  for ( int i=0; i < 4; i++ )
    retval[i] = quat1[i]*k0 + _quat2[i]*k1;

  return retval;
}