#include"Camerak.h"

Camerak::Camerak(GLMatrixStack *pMVMin)
{
	this->pMVM = pMVMin;
	movement_rate = 0.15f;
	rotation_rate = float(m3dDegToRad(1.0f));
	cam.SetOrigin(0,0,0);
	camMx = new M3DMatrix44f[1];
	camera_mode = FIRST_PERSON;

	//	limiter stuff
	maxAngle= 89.0f;
	minAngle= -89.0f;
	currentAngle=0.0f;
	allowMove = new bool[4];
	for(int i=0;i<4;i++){allowMove[i] = true;}
}

Camerak::~Camerak(void)
{
}

// This function alows the camera to move along the worlds z axis regardless of the cameras own orientation
void Camerak::moveForward()
{
	if(allowMove[Forward]){
	M3DVector3f temp;
	cam.GetForwardVector(temp);
	float x = cam.GetOriginX() + (temp[0] * movement_rate);
	float y = cam.GetOriginY();
	float z = cam.GetOriginZ() + (temp[2] * movement_rate);
	cam.SetOrigin(x,y,z);
	}
}

// This function alows the camera to move along the worlds z axis regardless of the cameras own orientation
void Camerak::moveBackward()
{
	if(allowMove[Back]){
	M3DVector3f temp;
	cam.GetForwardVector(temp);
	float x = cam.GetOriginX() + (temp[0] * -movement_rate);
	float y = cam.GetOriginY();
	float z = cam.GetOriginZ() + (temp[2] * -movement_rate);
	cam.SetOrigin(x,y,z);
	}
}


// The following two functions are used to differentiate camera behaviour between objects that will remain stationary to the camera like a skybox,
// or that will move independantly of the camera like actors or scene objects


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
void Camerak::setCameraMode(int imode, Vec3 iorigin, float iorientation)  // the iorigin is the players origin, and the iorientation is the players angle from the forward vector
{
	if(imode > 3 && imode < -1)   // This is to avoid errors if a non defined value is sent to the function
	{
		camera_mode = imode;
	}
	if(camera_mode == FIRST_PERSON)
	{
		resetUpVector();
	}
	if(camera_mode == THIRD_PERSON)
	{
		resetUpVector();
		rotateAntiClockWise(iorientation);
	}
	if(camera_mode == FREE_ROAMING)
	{
		resetUpVector();
	}
}

void Camerak::update(Vec3 iorigin){
		cam.SetOrigin(iorigin.x , iorigin.y + 0.75f, iorigin.z);	
}


M3DMatrix44f *Camerak::getMx(bool sky){
	if(sky){
		cam.GetCameraMatrix(camMx[0],true);
	}else{
		cam.GetCameraMatrix(camMx[0]);
	}
	return camMx;

}