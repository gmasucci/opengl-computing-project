#pragma once
#include "staticModel.h"
#include "DA_MD5Model.h"
#include "terrain.h"
#include "soundManager.h"


class Player :	public GameObject
{
private:

	bool rev;
	//md2model *me;
	DA::DA_MD5Model *me;

	Terrain *map;
	float velocity;
	int stars,health,numKeys;
	bool pain;
	bool winner;
	SoundManager *sm;

public:
	Player();
	Player(GLuint *tex,
		GLGeometryTransform *pGLGT,
		Terrain *mapIn,
		Camera *camIn,
		SoundManager *smIn);
	void setAnim(int n);
	void keys(unsigned char key);
	void keysUp(unsigned char key);
	void keyboardUpdater();
	void setPos(float*pos);
	void setPos(float x,float y,float z);
	void setAng(float* ang);

	void getPos(float*ret,float*ang);
	void addStar()	{stars++;}
	void addHealth(){health++;}

	void addKey(){numKeys++;}
	void removeKey() {numKeys--;}

	void removeHealth(){health--;}
	int getStars()	{return stars;}
	int getHealth()	{return health;}
	void render(GLMatrixStack *pMVM);
	bool isDead(){return health==0;}
	void setPain(){pain = true;}
	bool isWinner(){return winner;}
	void setWinner(bool win){this->winner = win;}
	~Player();

	bool isColliding(StaticModel *m);

};

