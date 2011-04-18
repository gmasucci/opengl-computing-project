//////////////////////////////////////////////
//Camera.h                                  //
//The Camera class is used to to create the //
//game camera at the moment only one camera	//
//is used, however using this class it		//
//should be possible to use various cameras //
//such as for a minimap						//
//////////////////////////////////////////////
#pragma once

#ifndef CAMERAK_H
#define CAMERAK_H
#include "Toolbox.h"

#define THIRD_PERSON	0
#define FIRST_PERSON	1
#define	FREE_ROAMING	2

class Camerak
{
public:
	// Constuctor and destructor
	Camerak(GLMatrixStack *pMVMin);
	~Camerak(void);

	// Functions that control the movement and rotation of the camera.  These generally work by manipulating 
	// the cameraFrame.
	inline void moveIn(){cam.MoveForward(movement_rate);};
	inline void moveOut(){cam.MoveForward(-movement_rate);};
	inline void strafeLeft(){cam.MoveRight(movement_rate);};
	inline void strafeRight(){cam.MoveRight(-movement_rate);};
	void moveForward();
	void moveBackward();
	inline void rotateAntiClockWise(float irotationrate){cam.RotateWorld(irotationrate,0,1,0);};
	inline void rotateUp(float irotationrate){	cam.RotateLocalX(-irotationrate);};
	//{ if ((currentAngle>0) && currentAngle < (maxAngle+irotationrate)) {	cam.RotateLocalX(-irotationrate); currentAngle+=-irotationrate;	} else if ((currentAngle<0) && currentAngle > (-maxAngle+irotationrate)) {cam.RotateLocalX(-irotationrate); currentAngle-=-irotationrate;}	};
	inline void resetForwardVector(){cam.SetForwardVector(0,0,-1);};
	inline void resetUpVector(){cam.SetUpVector(0,1,0);};

	// These functions apply the camera matrix to the scene
	
	void PushMatrix(bool sky = false);
	void PopMatrix(){pMVM->PopMatrix();}
	// This function is used to switch between thirs-person, first-person and free-roaming cameras.
	// These are all available in code but the actual game has no option to switch between them due
	// to strange behaviour when switching between multiple modes.
	void setCameraMode(int imode, Vec3 iorigin, float iorientation);

	// The update is used to ensure the camera remains remains in the correct position in relation to the player.
	void update(Vec3 iorigin);

	// Functions for accessing and manipulating private or protected attributes
	inline GLFrame getFrame(){return cam;};
	inline int getCameraMode(){return camera_mode;};
	M3DMatrix44f *Camerak::getMx(bool sky = false);

private:
	GLFrame cam;
	GLMatrixStack *pMVM;
	M3DMatrix44f *camMx;
	float movement_rate;
	float rotation_rate;
	int camera_mode;				// Used to specify first-peron, third-person or free-roaming camera

	//  limiter stuff
private:
	float maxAngle;
	float currentAngle;
	
public:
	void setMaxAngle(float theta)		{	maxAngle = theta;	}
	float getMaxAngle()					{	return maxAngle;	}
	float getCurrentAngle()				{	return currentAngle;}
	void adjustCurrentAngle(float  angleMod)	{	currentAngle+=angleMod;	}
	void setCurrentAngle(float theta)			{	currentAngle+=theta;	}
	void setOrigin(M3DVector3f m){cam.SetOrigin(m);}
};
#endif