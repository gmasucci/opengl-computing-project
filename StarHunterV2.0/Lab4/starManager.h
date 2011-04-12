#pragma once
#include "star.h"
#include "terrain.h"

class starManager
{
private:
	GLGeometryTransform *pGLGT;
	star	*aStar;
	GLFrame *theStars;
	GLuint *starTexture;
	terrain *theTerrain;
	M3DVector3f *positions;
	int numStars;
	float verticalPos;

public:
	starManager(GLGeometryTransform *pGLGTin,terrain *map,GLuint*startex);
	void loadStarPositions(char *fname);
	void renderAllStars(GLMatrixStack *pMVM);
	void updateAllStars();
	~starManager(void);
};

