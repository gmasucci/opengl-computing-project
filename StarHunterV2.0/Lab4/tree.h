#pragma once
#include "staticmodel.h"
#include "loaders.h"
#include "terrain.h"

//poorly named class, this draws all the trees.
class Tree :public StaticModel
{
private:
	int clones;
	GLFrame *treeFrame;
	Terrain* theTerrain;

public:
	Tree(int numtrees,GLuint *tex,Terrain *theTerrain,GLGeometryTransform *pGLGT,	Camera *camIn);
	void setup();
	void render(GLMatrixStack *pMVM);
	~Tree();
};

