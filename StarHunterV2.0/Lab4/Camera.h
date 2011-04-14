#pragma once
#include "Toolbox.h"

class Camera
{
public:
	Camera(GLMatrixStack *pGLMS);
	M3DMatrix44f *camMx;
	void pushMatrix(bool sky = false);
	void popcam();
	void update(float*ppos,float*pang);
	void spin();
	M3DMatrix44f *getMx(bool sky=false);
	~Camera();
private:
	GLFrame	cam;
	float rot;
	GLMatrixStack *pMVM;
};

