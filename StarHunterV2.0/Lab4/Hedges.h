#pragma once
#include "staticmodel.h"
#include "loaders.h"
#include "terrain.h"
#define HEDGE01 31
#define HEDGE02 70


class Hedges :
	public StaticModel
{
private:
	int clones;
	int mode;
	GLFrame *hedgeFrame;
	Terrain* theTerrain;


public:
	Hedges(int mode,int numhedges,GLuint *tex,Terrain *theTerrain,GLGeometryTransform *pGLGT,	Camera *camIn);
	void setup();
	void render(GLMatrixStack *pMVM);
	void getColInfo(Vec3 *cmax,Vec3 *cmin);
	Vec3 getColBox(int obj);
	~Hedges();
};

