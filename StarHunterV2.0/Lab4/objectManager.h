#pragma once
#include "tree.h"
#include "Hedges.h"
#include "terrain.h"
#include "player.h"
#include "numbers.h"
#include "Camera.h"
#include "Bass.h"
#include "soundManager.h"
#include "Controls.h"


class ObjectManager
{
private:
	Tree *t;
	Hedges *h1;
	Hedges *h2;

	SoundManager	*psndMan;
	GLGeometryTransform *pGLGT;

	StaticModel		*smHouse;
	StaticModel		*smKey;
	StaticModel		*smStump;
	StaticModel		*smLogs;


	Player			*thePlayer;
	Terrain			*theTerrain;
	M3DVector3f		*starpositions;
	M3DVector3f		*heartpositions;
	M3DVector3f		*baddiepositions;
	M3DVector3f		*idlePoints;
	bool			*needNewPoint;
	Controls		*pInput;
	Camerak			*camK;

	int numStars,numHearts,numBaddies;
	float verticalPos;



public:
	ObjectManager(GLGeometryTransform *pGLGTin,Player *playerIn,
				Terrain *map,
				GLuint *treetex,
				GLuint *housetex,
				GLuint *keytex,
				GLuint *stumpTex,
				GLuint *hedgeTex,
				Camera *camIn,
				Camerak *camK,
				Controls *ctrl,
				SoundManager *sndManPtr);

	void loadPositions(char *fname);
	void renderAllObjects(GLMatrixStack *pMVM);
	void updateAllObjects();
	void colliding(int type,GLFrame *obj);
	int getStars();

	~ObjectManager(void);
};

