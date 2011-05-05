#pragma once
#include "staticmodel.h"
#include "loaders.h"
#include "terrain.h"
#define GRAVES01 10
#define GRAVES02 20


class Graves :
	public StaticModel
{
private:
	int clones;
	int mode;
	GLFrame *graveFrame;
	Terrain* theTerrain;


public:
	Graves(int mode,int numgraves,GLuint *tex,Terrain *theTerrain,GLGeometryTransform *pGLGT,	Camera *camIn);
	void setup();
	void render(GLMatrixStack *pMVM);
	void getColInfo(Vec3 *cmax,Vec3 *cmin);
	Vec3 getColBox(int obj);
	~Graves();
};

