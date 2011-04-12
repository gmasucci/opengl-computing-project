#pragma once
#include "gameobject.h"
#include "DA_MD5Model.h"
#include "terrain.h"
#include "soundManager.h"

class player :	public gameObject
{
private:
	struct keyList{
		bool w;
		bool a;
		bool d;
		bool s;
		bool k;
		bool l;
	};
	bool rev;
	keyList kp;
	//md2model *me;
	DA::DA_MD5Model *me;

	terrain *map;
	float velocity;
	int stars,health;
	bool pain;
	bool winner;
	soundManager *sm;

public:
	player();
	player(GLuint *tex,
		GLGeometryTransform *pGLGT,
		terrain *mapIn,
		Camera *camIn,
		soundManager *smIn);
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
	void removeHealth(){health--;}
	int getStars()	{return stars;}
	int getHealth()	{return health;}
	void render(GLMatrixStack *pMVM);
	bool isDead(){return health==0;}
	void setPain(){pain = true;}
	bool isWinner(){return winner;}
	void setWinner(bool win){this->winner = win;}
	~player();

};

