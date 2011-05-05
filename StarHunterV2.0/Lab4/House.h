#pragma once
#include "staticmodel.h"
#include "loaders.h"
#include "terrain.h"
#define HOUSE1 6
#define HOUSE2 5

class House :
	public StaticModel
{
private:
	int clones;
	int mode;
	GLFrame *houseFrame;
	Terrain* theTerrain;
	M3DMatrix44f mx4;
	M3DMatrix33f mx3;

public:
	House(int mode,int numhouses,GLuint *tex,Terrain *theTerrain,GLGeometryTransform *pGLGT,	Camera *camIn);
	void setup();
	void render(GLMatrixStack *pMVM);
	void getColInfo(Vec3 *cmax,Vec3 *cmin);
	Vec3 getColBox(int obj);
	~House();
};

