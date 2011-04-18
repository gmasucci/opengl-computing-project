#include "Camera.h"
#include <iostream>
#include <math3d.h>

Camera::Camera(GLMatrixStack *pGLMS)
{
	this->pMVM=pGLMS;
	rot=0.0f;
	camMx = new M3DMatrix44f[1];
}
void Camera::pushMatrix(bool sky){
	if(sky){								//if the call is for skybox only
		//M3DMatrix44f camMx;					
		pMVM->PushMatrix();
		cam.GetCameraMatrix(camMx[0],true);
		pMVM->LoadMatrix(camMx[0]);
	}else{
		//M3DMatrix44f camMx;
		cam.GetCameraMatrix(camMx[0]);
		pMVM->PushMatrix(camMx[0]);
	}
}
void Camera::popcam(){
	pMVM->PopMatrix();
}
void Camera::update(float*ppos,float*pang){
	cam.SetOrigin(ppos);
	M3DVector3f t;
	t[0] = pang[0];
	t[2] = pang[2];
	t[1] = 0;
	m3dNormalizeVector3(t);
	cam.SetForwardVector(t);
}
void Camera::spin(){
	rot-=0.01;
}
M3DMatrix44f *Camera::getMx(bool sky){
	if(sky){
		cam.GetCameraMatrix(camMx[0],true);
	}else{
		cam.GetCameraMatrix(camMx[0]);
	}
	return camMx;

}
Camera::~Camera(){
}