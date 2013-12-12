#if ( defined( __GNUC__ ) || defined( __GNUG__ ) )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Werror"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wparentheses"
#endif

#include "camera.h"
#include "math.h"
#include <iostream>
#include <SFML/Graphics/Matrix3.hpp>
#include <SFML/System/Vector3.hpp>
//#include "windows.h" //@F commented out

#define SQR(x) (x*x)

#define NULL_VECTOR F3dVector(0.0f,0.0f,0.0f)

SF3dVector F3dVector( GLfloat x, GLfloat y, GLfloat z ) {
	SF3dVector tmp;
	tmp.x = x;
	tmp.y = y;
	tmp.z = z;
	return tmp;
}

GLfloat GetF3dVectorLength( SF3dVector* v ) {
	return ( GLfloat )( sqrt( SQR( v->x ) + SQR( v->y ) + SQR( v->z ) ) );
}

SF3dVector Normalize3dVector( SF3dVector v ) {
	SF3dVector res;
	float l = GetF3dVectorLength( &v );

	if( l == 0.0f )
		return NULL_VECTOR;

	res.x = v.x / l;
	res.y = v.y / l;
	res.z = v.z / l;
	return res;
}

SF3dVector operator+ ( SF3dVector v, SF3dVector u ) {
	SF3dVector res;
	res.x = v.x + u.x;
	res.y = v.y + u.y;
	res.z = v.z + u.z;
	return res;
}
SF3dVector operator- ( SF3dVector v, SF3dVector u ) {
	SF3dVector res;
	res.x = v.x - u.x;
	res.y = v.y - u.y;
	res.z = v.z - u.z;
	return res;
}


SF3dVector operator* ( SF3dVector v, float r ) {
	SF3dVector res;
	res.x = v.x * r;
	res.y = v.y * r;
	res.z = v.z * r;
	return res;
}

SF3dVector CrossProduct( SF3dVector* u, SF3dVector* v ) {
	SF3dVector resVector;
	resVector.x = u->y * v->z - u->z * v->y;
	resVector.y = u->z * v->x - u->x * v->z;
	resVector.z = u->x * v->y - u->y * v->x;

	return resVector;
}
float operator* ( SF3dVector v, SF3dVector u ) {	//dot product
	return v.x * u.x + v.y * u.y + v.z * u.z;
}




/***************************************************************************************/

CCamera::CCamera() {
	//Init with standard OGL values:
	Position = F3dVector( 0.0, 0.0,	0.0 );
	ViewDir = F3dVector( 0.0, 0.0, -1.0 );
	RightVector = F3dVector( 1.0, 0.0, 0.0 );
	UpVector = F3dVector( 0.0, 1.0, 0.0 );

	//Only to be sure:
	RotatedX = RotatedY = RotatedZ = 0.0;
}

void CCamera::Move( SF3dVector Direction ) {
	Position = Position + Direction;
}

// failed test
//SF3dVector ApplyMatrix(SF3dVector& vec,const sf::Matrix3& matrix)
//{
//	SF3dVector result;
//	const float* mat = matrix.Get4x4Elements();
//	result.x = mat[0]*vec.x + mat[1]*vec.y + mat[2]*vec.z;
//	result.x = mat[4]*vec.x + mat[5]*vec.y + mat[6]*vec.z;
//	result.x = mat[8]*vec.x + mat[9]*vec.y + mat[10]*vec.z;
//
//	vec=result;
//
//	return result;
//}

void CCamera::RotateX( GLfloat Angle ) {
	RotatedX += Angle;

	//Rotate viewdir around the right vector:
	ViewDir = Normalize3dVector( ViewDir * cos( Angle * PIdiv180 )
								 + UpVector * sin( Angle * PIdiv180 ) );

	//now compute the new UpVector (by cross product)
	UpVector = CrossProduct( &ViewDir, &RightVector ) * -1;


}

// failed test
//sf::Matrix3 MakeRotMatrix(float x, float y, float z, float Angle)
//{
//	float AngleR = Angle*PIdiv180; // in radian
//	float t = (1-cos(AngleR));
//	sf::Matrix3 mat(
//		t * pow(x, 2) + cos(AngleR),
//		t*x*y + sin(AngleR)*z,
//		t*x*z + sin(AngleR)*y,
//
//		t*x*y - sin(AngleR)*z,
//		t * pow(y, 2) + cos(AngleR),
//		t*y*z + sin(AngleR)*x,
//
//		t*x*y + sin(AngleR)*y,
//		t*y*z - sin(AngleR)*x,
//		t * pow(z, 2) + cos(AngleR)
//		);
//
//	 return mat;
//}

void CCamera::RotateY( GLfloat Angle ) {
	// a failed test
	//sf::Matrix3 mat = MakeRotMatrix(0,1,0,Angle);
	//ApplyMatrix(ViewDir, mat);
	//ApplyMatrix(RightVector, mat);
	//ApplyMatrix(UpVector, mat);


	RotatedY += Angle;

	//Rotate viewdir around the up vector:
	ViewDir = Normalize3dVector( ViewDir * cos( Angle * PIdiv180 )
								 - RightVector * sin( Angle * PIdiv180 ) );

	////now compute the new RightVector (by cross product)
	RightVector = CrossProduct( &ViewDir, &UpVector );
}

void CCamera::RotateZ( GLfloat Angle ) {
	RotatedZ += Angle;

	//Rotate viewdir around the right vector:
	RightVector = Normalize3dVector( RightVector * cos( Angle * PIdiv180 )
									 + UpVector * sin( Angle * PIdiv180 ) );

	//now compute the new UpVector (by cross product)
	UpVector = CrossProduct( &ViewDir, &RightVector ) * -1;
}

void CCamera::Render( void ) {

	//The point at which the camera looks:
	SF3dVector ViewPoint = Position + ViewDir;

	//as we know the up vector, we can easily use gluLookAt:
	gluLookAt(	Position.x, Position.y, Position.z,
				ViewPoint.x, ViewPoint.y, ViewPoint.z,
				UpVector.x, UpVector.y, UpVector.z );

}

void CCamera::MoveForward( GLfloat Distance ) {
	Position = Position + ( ViewDir * -Distance );
}

void CCamera::StrafeRight( GLfloat Distance ) {
	Position = Position + ( RightVector * Distance );
}

void CCamera::MoveUpward( GLfloat Distance ) {
	Position = Position + ( UpVector * Distance );
}

#if ( defined( __GNUC__ ) || defined( __GNUG__ ) )
#pragma GCC diagnostic pop
#endif
