#pragma once
#include "staticmodel.h"
#include "loaders.h"
#include "terrain.h"


class Hedges :
	public StaticModel
{
private:
	int clones;
	GLFrame *hedgeFrame;
	Terrain* theTerrain;

public:
	Hedges(int numhedges,GLuint *tex,Terrain *theTerrain,GLGeometryTransform *pGLGT,	Camera *camIn);
	void setup();
	void render(GLMatrixStack *pMVM);
	~Hedges();
};

