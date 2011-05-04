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

private:
	Vec3 move;
	GLFrame cam;
	GLMatrixStack *pMVM;
	M3DMatrix44f *camMx;
	float movement_rate;
	float rotation_rate;

public:
	Vec3 lastPos;

	// Constuctor and destructor
	Camerak(GLMatrixStack *pMVMin);
	~Camerak(void);
	void getOrigin(float *o){cam.GetOrigin(o);}
	// Functions that control the movement and rotation of the camera.  These generally work by manipulating 
	// the cameraFrame.
	inline void moveIn(){cam.MoveForward(movement_rate);}
	inline void moveOut(){cam.MoveForward(-movement_rate);}
	
	void strafeLeft();
	void strafeRight();
	void moveForward();
	void moveBackward();

	
	inline void rotateAntiClockWise(float irotationrate){cam.RotateWorld(irotationrate,0,1,0);};
	inline void rotateUp(float irotationrate){	cam.RotateLocalX(-irotationrate);}
	inline void resetForwardVector(){cam.SetForwardVector(0,0,-1);};
	inline void resetUpVector(){cam.SetUpVector(0,1,0);};

	// These functions apply the camera matrix to the scene
	
	void PushMatrix(bool sky = false);
	void PopMatrix(){pMVM->PopMatrix();}
	
	// The update is used to ensure the camera remains remains in the correct position in relation to the player.
	void update();
	void collisionResponse();
	// Functions for accessing and manipulating private or protected attributes
	inline GLFrame getFrame(){return cam;}
	
	M3DMatrix44f *Camerak::getMx(bool sky = false);
	
	void setOrigin(M3DVector3f newOrigin){cam.SetOrigin(newOrigin);}
				// Used to specify first-peron, third-person or free-roaming camera
};
#endif