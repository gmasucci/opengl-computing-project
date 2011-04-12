#pragma once
#include "tree.h"
#include "terrain.h"
#include "player.h"
#include "numbers.h"
#include "Camera.h"
#include "Bass.h"
#include "soundManager.h"

class objectManager
{
private:
	tree *t;
	soundManager	*psndMan;
	GLGeometryTransform *pGLGT;
	staticModel		*aStar;
	player			*thePlayer;
	terrain			*theTerrain;
	M3DVector3f		*starpositions;
	M3DVector3f		*heartpositions;
	M3DVector3f		*baddiepositions;
	M3DVector3f		*idlePoints;
	bool			*needNewPoint;


	int numStars,numHearts,numBaddies;
	float verticalPos;



public:
	objectManager(GLGeometryTransform *pGLGTin,player *playerIn,
				terrain *map,
				GLuint *startex,
				GLuint *hearttex,
				GLuint *baddietex,
				Camera *camIn,
				soundManager *sndManPtr);

	void loadPositions(char *fname);
	void renderAllObjects(GLMatrixStack *pMVM);
	void updateAllObjects();
	void colliding(int type,GLFrame *obj);
	int getStars();

	~objectManager(void);
};

