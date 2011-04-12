#pragma once
#include "staticmodel.h"
#include "loaders.h"
#include "terrain.h"

//poorly named class, this draws all the trees.
class tree :public staticModel
{
private:
	int clones;
	GLFrame *treeFrame;
	terrain* theTerrain;

public:
	tree(int numtrees,GLuint *tex,terrain *theTerrain,GLGeometryTransform *pGLGT,	Camera *camIn);
	void setup();
	void render(GLMatrixStack *pMVM);
	~tree();
};

