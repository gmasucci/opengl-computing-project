#include "objectManager.h"

using namespace std;

ObjectManager::ObjectManager(GLGeometryTransform *pGLGTin,
				Player *playerIn,
				Terrain *map,
				GLuint *treetex,
				GLuint *housetex,
				GLuint *keytex,
				GLuint *stumpTex,
				GLuint *hedgeTex,
				GLuint *graveTex,
				GLuint *ruinsTex,
				Camera *camIn,
				Camerak *camKin,
				Controls *pInputIn,
				Overlay * hint,
				SoundManager *sndManPtr)

{
	psndMan = sndManPtr;
	this->eHint = hint;
	this->pGLGT=pGLGTin;
	this->theTerrain= map;
	this->thePlayer=playerIn;
	this->verticalPos=0.0f;
	this->pInput = pInputIn;
	this->camK = camKin;
	M3DVector3f tmp,ang;
	renderhint[0] = renderhint[1] = false;
	tmp[0] = tmp[2] = 40.0f;
	tmp[1]=theTerrain->getHeightAt(tmp[0],tmp[2]);
	camK->setOrigin(tmp);

	
	smHouse = new StaticModel("house",housetex,pGLGT,camIn);
	ruins = new StaticModel("ruins",ruinsTex,pGLGT,camIn);

	smKeyOne = new StaticModel("key",keytex,pGLGT,camIn,false);
	smKeyTwo = new StaticModel("key",keytex,pGLGT,camIn,false);

	smStump = new StaticModel("stump",stumpTex,pGLGT,camIn);
	smStump->setAngle(180.0f);
	smLogs = new StaticModel("logs",stumpTex,pGLGT,camIn);
	
	smKeyOne->setSpinning(true);
	smKeyTwo->setSpinning(true);

	smGateOne = new StaticModel("gate",keytex,pGLGT,camIn,false);
	smGateTwo = new StaticModel("gate",keytex,pGLGT,camIn,false);
	smGateTwo->rotateCollisionPoints();
	smGateTwo->setAngle(90);

	M3DVector3f p;
	p[0] = p[2] = 50.0f;
	p[1] = theTerrain->getHeightAt(p[0],p[2]);
	smHouse->setPos(p);

	p[0] = 146;
	p[2] = 38.0f;
	p[1] = theTerrain->getHeightAt(p[0],p[2]);
	ruins->setPos(p);

	p[0] = 30;
	p[2] = 60;
	p[1] = theTerrain->getHeightAt(p[0],p[2]);
	smKeyOne->setPos(p);
	smKeyOne->setScale(0.2);

	p[0] = 132;
	p[2] = 46;
	p[1] = theTerrain->getHeightAt(p[0],p[2]);
	smKeyTwo->setPos(p);
	smKeyTwo->setScale(0.2);

	p[0] = p[2] = 40.0f;
	p[1] = theTerrain->getHeightAt(p[0],p[2]);
	smStump->setPos(p);

	p[0] = 30.0f;
	p[2] = 40.0f;
	p[1] = theTerrain->getHeightAt(p[0],p[2]);
	smLogs->setPos(p);

	p[0] = 125.45;
	p[2] = 70.2;
	p[1] = theTerrain->getHeightAt(p[0],p[2]);
	smGateOne->setPos(p);
	
	p[0] = 198.69;
	p[2] = 117.66;
	p[1] = theTerrain->getHeightAt(p[0],p[2]);
	smGateTwo->setPos(p);

	int numtrees = 200;
	t = new Tree(numtrees,treetex,map,pGLGT,camIn);
	h1 = new Hedges(0,HEDGE01,hedgeTex,map,pGLGT,camIn);
	h2 = new Hedges(1,HEDGE02,hedgeTex,map,pGLGT,camIn);
	houses1 = new House(1,HOUSE1,housetex,theTerrain,pGLGT,camIn);
	houses2 = new House(0,HOUSE2,housetex,theTerrain,pGLGT,camIn);
	graves1 = new Graves(0,GRAVES01,graveTex,theTerrain,pGLGT,camIn);
}

void ObjectManager::loadPositions(char *fname){
	//no longer used;
}

void ObjectManager::renderAllObjects(GLMatrixStack *pMVM){
	
	smHouse ->render(pMVM);
	ruins->render(pMVM);
	smKeyOne->render(pMVM);
	smKeyTwo->render(pMVM);
	smStump->render(pMVM);
	smLogs->render(pMVM);
	smGateOne->render(pMVM);
	smGateTwo->render(pMVM);
	t->render(pMVM);
	h1->render(pMVM);
	h2->render(pMVM);
	houses1->render(pMVM);
	houses2->render(pMVM);
	graves1->render(pMVM);

	if(renderhint[0] || renderhint[1])
		eHint->render();
}

int ObjectManager::getStars(){return numStars;}

void ObjectManager::updateAllObjects(){

	
	M3DVector3f tmp,ang;
	thePlayer->getPos(tmp,ang);
	tmp[1]=1.0+theTerrain->getHeightAt(tmp[0],tmp[2]);
	thePlayer->setPos(tmp);

	if(thePlayer->isColliding(smHouse)){
		camK->collisionResponse();
	}
	if(thePlayer->isColliding(ruins)){
		camK->collisionResponse();
	}

	if(thePlayer->isColliding(smStump)){
		camK->collisionResponse();
	}

	if(thePlayer->isColliding(smLogs)){
		camK->collisionResponse();
	}

	if (thePlayer->isColliding(smKeyOne)){
		float p[] = {0.0f, -20.0f, 0.0f};
		smKeyOne->setPos(p);
		thePlayer->addKey();
	}
	if (thePlayer->isColliding(smKeyTwo)){
		float p[] = {0.0f, -20.0f, 0.0f};
		smKeyTwo->setPos(p);
		thePlayer->addKey();
	}

	if(thePlayer->isColliding(HEDGE01,h1)){
		camK->collisionResponse();
	}
	if(thePlayer->isColliding(HEDGE02,h2)){
		camK->collisionResponse();
	}

	if(thePlayer->isColliding(HOUSE1,houses1)){
		camK->collisionResponse();
	}

	if(thePlayer->isColliding(HOUSE2,houses2)){
		camK->collisionResponse();
	}
	if(thePlayer->isColliding(smGateOne)){
		camK->collisionResponse();
	}
	if(thePlayer->isColliding(smGateTwo)){
		camK->collisionResponse();
	}

	this->checkRenderHint(smGateOne,0);
	this->checkRenderHint(smGateTwo,1);

}
void ObjectManager::checkRenderHint(StaticModel *gate,int gn){
	M3DVector3f a,b;
	Vec3 va,vb;
	camK->getOrigin(a);
	gate->getPos(b);
	
	va.fromM3D(a);
	vb.fromM3D(b);
	vb-=va;
	float dist = vb.length();
	if (dist < 3){
		renderhint[gn] = true;
		currentGate = gate;
	}else{
		renderhint[gn] = false;
	}
}

void ObjectManager::colliding(int type,GLFrame *obj){
	

}

void ObjectManager::ePressed(){

	if(renderhint){
		if(thePlayer->getNumKeys() > 0){
			M3DVector3f v;
			v[0] = v[2] = 0;
			v[1] = -20;
			currentGate->setPos(v);
			//currentGate->deleteColInfo();
			thePlayer->removeKey();
		}
	}

}
ObjectManager::~ObjectManager(void)
{
	//delete[] starpositions;
	//delete[] heartpositions;
	//delete[] baddiepositions;
	//delete[] theBaddies;
	//delete[] theHearts;
	//delete[] theStars;
}
