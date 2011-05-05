#include"Camerak.h"

Camerak::Camerak(GLMatrixStack *pMVMin)
{
	this->pMVM = pMVMin;
	movement_rate = 0.08f;//change this
	rotation_rate = float(m3dDegToRad(1.0f));
	cam.SetOrigin(0,0,0);
	camMx = new M3DMatrix44f[1];
	//cam.RotateLocalY(1.57075);
}

Camerak::~Camerak(void)
{
}

// This function alows the camera to move along the worlds z axis regardless of the cameras own orientation
void Camerak::moveForward()
{
	M3DVector3f temp;
	Vec3 thisMove;
	cam.GetOrigin(temp);
	lastPos.fromM3D(temp);
	cam.GetForwardVector(temp);
	temp[1] = 0;
	m3dNormalizeVector3(temp);
	thisMove.fromM3D(temp);
	thisMove*= movement_rate;
	move+=thisMove;

	//cam.SetOrigin(x,y,z);
	
}
void Camerak::strafeRight(){
	
	M3DVector3f temp,fwd,up,right;
	Vec3 thisMove;
	cam.GetOrigin(temp);
	lastPos.fromM3D(temp);
	cam.GetForwardVector(fwd);
	cam.GetUpVector(up);
	m3dCrossProduct3(right,fwd,up);
	thisMove.fromM3D(right);
	thisMove*= movement_rate;
	move+=thisMove;
}
void Camerak::strafeLeft(){
	
	M3DVector3f temp,fwd,up,right;
	Vec3 thisMove;
	cam.GetOrigin(temp);
	lastPos.fromM3D(temp);
	cam.GetForwardVector(fwd);
	cam.GetUpVector(up);
	m3dCrossProduct3(right,fwd,up);
	thisMove.fromM3D(right);
	thisMove*= movement_rate;
	move-=thisMove;

}
void Camerak::moveBackward()
{
	
	M3DVector3f temp;
	Vec3 thisMove;
	cam.GetOrigin(temp);
	lastPos.fromM3D(temp);
	cam.GetForwardVector(temp);
	temp[1] = 0;
	m3dNormalizeVector3(temp);
	thisMove.fromM3D(temp);
	thisMove*= movement_rate;
	move-=thisMove;
	
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


void Camerak::update(){
	M3DVector3f o;
	Vec3 vo;
	cam.GetOrigin(o);
	vo.fromM3D(o);

	vo+=move;
	vo.toM3D(o);
	
	cam.SetOrigin(o);
	move*=0.8;


}

void Camerak::collisionResponse(){
	
	move*=-2.0;
}

M3DMatrix44f *Camerak::getMx(bool sky){
	if(sky){
		cam.GetCameraMatrix(camMx[0],true);
	}else{
		cam.GetCameraMatrix(camMx[0]);
	}
	return camMx;

}