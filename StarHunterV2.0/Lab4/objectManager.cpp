#include "objectManager.h"
using namespace std;

ObjectManager::ObjectManager(GLGeometryTransform *pGLGTin,
				Player *playerIn,
				Terrain *map,
				GLuint *treetex,
				GLuint *housetex,
				GLuint *keytex,
				GLuint *stumpTex,
				Camera *camIn,
				Camerak *camKin,
				Controls *pInputIn,
				SoundManager *sndManPtr)

{
	psndMan = sndManPtr;
	this->pGLGT=pGLGTin;
	this->theTerrain= map;
	this->thePlayer=playerIn;
	this->verticalPos=0.0f;
	this->pInput = pInputIn;
	this->camK = camKin;
	M3DVector3f tmp,ang;
	
	tmp[0] = tmp[2] = 20.0f;
	tmp[1]=theTerrain->getHeightAt(tmp[0],tmp[2]);
	Vec3 pos;
	pos.fromM3D(tmp);
	camK->update(pos);

	//thePlayer->setAngle(235.0f);

	smHouse = new StaticModel("house",housetex,pGLGT,camIn);
	smKey = new StaticModel("key",keytex,pGLGT,camIn,false);
	smStump = new StaticModel("gate",stumpTex,pGLGT,camIn,false);
	smStump->setAngle(180.0f);
	smLogs = new StaticModel("logs",stumpTex,pGLGT,camIn);
	smKey->setSpinning(true);
	
	M3DVector3f p;
	p[0] = p[2] = 50.0f;
	p[1] = theTerrain->getHeightAt(p[0],p[2]);
	smHouse->setPos(p);
	smHouse->setViewCollBox();
	p[0] = p[2] = 30.0f;
	p[1] = theTerrain->getHeightAt(p[0],p[2]);
	smKey->setPos(p);
	smKey->setViewCollBox();
	p[0] = p[2] = 40.0f;
	p[1] = theTerrain->getHeightAt(p[0],p[2]);
	smStump->setPos(p);
	smStump->setViewCollBox();
	p[0] = 30.0f;
	p[2] = 40.0f;
	p[1] = theTerrain->getHeightAt(p[0],p[2]);
	smLogs->setPos(p);
	smLogs->setViewCollBox();

	int numtrees = 200;
	t = new Tree(numtrees,treetex,map,pGLGT,camIn);
	
}

void ObjectManager::loadPositions(char *fname){

	ifstream::pos_type size;
	char * memblock;

	ifstream file (fname, ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char [size];
		file.seekg (0, ios::beg);
		file.read (memblock, size);
		file.close();
	}
	else
	{	
		cerr << "Unable to open file: " << fname << endl;
	}
	
	stringstream strstream(memblock);

	string check;
	strstream >> check;
	if (check.compare(0,7,"OPDFile")!=0)
	{	
		cerr << "File: " << fname << ", is not an Object Position Data file." << endl;
	}else{

			//load object position data.

	}
	
	M3DVector3f p,a;
	thePlayer->getPos(p,a);
	p[0]=48;p[2]=48;p[1]=theTerrain->getHeightAt(48,48);
	thePlayer->setPos(p);
	cout << "Object Position Data File " << fname << " loaded." << endl;
	delete[] memblock;

	//generate trees!



}
void ObjectManager::renderAllObjects(GLMatrixStack *pMVM){
	
	smHouse ->render(pMVM);
	smKey->render(pMVM);
	smStump->render(pMVM);
	smLogs->render(pMVM);
	t->render(pMVM);

}

int ObjectManager::getStars(){return numStars;}

void ObjectManager::updateAllObjects(){

	thePlayer->keyboardUpdater();
	M3DVector3f tmp,ang;
	thePlayer->getPos(tmp,ang);
	tmp[1]=1.0+theTerrain->getHeightAt(tmp[0],tmp[2]);
	thePlayer->setPos(tmp);

	if(thePlayer->isColliding(smHouse)){
		camK->collisionResponse();

		if(pInput->getKeyState('w')){ camK->moveBackward();}
		if(pInput->getKeyState('s')){ camK->moveForward(); }
		if(pInput->getKeyState('a')){ camK->strafeRight(); }
		if(pInput->getKeyState('d')){ camK->strafeLeft(); }
	
	}

	if(thePlayer->isColliding(smStump)){
		camK->collisionResponse();

		if(pInput->getKeyState('w')){ camK->moveBackward();}
		if(pInput->getKeyState('s')){ camK->moveForward(); }
		if(pInput->getKeyState('a')){ camK->strafeRight(); }
		if(pInput->getKeyState('d')){ camK->strafeLeft(); }
	
	}

	if(thePlayer->isColliding(smLogs)){
		
		camK->collisionResponse();
		if(pInput->getKeyState('w')){ camK->moveBackward();}
		if(pInput->getKeyState('s')){ camK->moveForward(); }
		if(pInput->getKeyState('a')){ camK->strafeRight(); }
		if(pInput->getKeyState('d')){ camK->strafeLeft(); }

	}

	if (thePlayer->isColliding(smKey)){
		float newpos[] = {0.0f, -20.0f, 0.0f};
		smKey->setPos(newpos);
		std::cout << "GOT A KEY!!!\n";
	}


}


void ObjectManager::colliding(int type,GLFrame *obj){
	

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
