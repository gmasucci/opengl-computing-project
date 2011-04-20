#include"Camerak.h"

Camerak::Camerak(GLMatrixStack *pMVMin)
{
	this->pMVM = pMVMin;
	movement_rate = 0.10f;
	rotation_rate = float(m3dDegToRad(1.0f));
	cam.SetOrigin(0,0,0);
	camMx = new M3DMatrix44f[1];
	cam.RotateLocalY(1.57075);
}

Camerak::~Camerak(void)
{
}

// This function alows the camera to move along the worlds z axis regardless of the cameras own orientation
void Camerak::moveForward()
{
	M3DVector3f temp;
	cam.GetOrigin(temp);
	lastPos.fromM3D(temp);
	cam.GetForwardVector(temp);
	temp[1] = 0;
	m3dNormalizeVector3(temp);
	float x = cam.GetOriginX() + (temp[0] * movement_rate);
	float y = cam.GetOriginY();
	float z = cam.GetOriginZ() + (temp[2] * movement_rate);
	cam.SetOrigin(x,y,z);
	
}
void Camerak::strafeRight(){
	
	M3DVector3f temp;
	cam.GetOrigin(temp);
	lastPos.fromM3D(temp);
	cam.MoveRight(-movement_rate);
}
void Camerak::strafeLeft(){
	
	M3DVector3f temp;
	cam.GetOrigin(temp);
	lastPos.fromM3D(temp);
	cam.MoveRight(movement_rate);

}
void Camerak::moveBackward()
{
	
	M3DVector3f temp;
	cam.GetOrigin(temp);
	lastPos.fromM3D(temp);
	cam.GetForwardVector(temp);
	temp[1] = 0;
	m3dNormalizeVector3(temp);
	float x = cam.GetOriginX() + (temp[0] * -movement_rate);
	float y = cam.GetOriginY();
	float z = cam.GetOriginZ() + (temp[2] * -movement_rate);
	cam.SetOrigin(x,y,z);
	
}


void Camerak::PushMatrix(bool sky){
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


// This function is used to switch between the camera modes and realign the camera in relation to each mode.

void Camerak::update(Vec3 iorigin){
		cam.SetOrigin(iorigin.x , iorigin.y + 0.75f, iorigin.z);	
}

void Camerak::collisionResponse(){
	
	M3DVector3f m;
	lastPos.toM3D(m);
	cam.SetOrigin(m);
}

M3DMatrix44f *Camerak::getMx(bool sky){
	if(sky){
		cam.GetCameraMatrix(camMx[0],true);
	}else{
		cam.GetCameraMatrix(camMx[0]);
	}
	return camMx;

}