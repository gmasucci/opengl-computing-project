#include "objectManager.h"
using namespace std;

ObjectManager::ObjectManager(GLGeometryTransform *pGLGTin,
				Player *playerIn,
				Terrain *map,
				GLuint *treetex,
				GLuint *housetex,
				GLuint *keytex,
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
	smKey->setSpinning(true);
	M3DVector3f p;
	p[0] = p[2] = 50.0f;
	p[1] = theTerrain->getHeightAt(p[0],p[2]);
	smHouse->setPos(p);
	p[0] = p[2] = 30.0f;
	p[1] = theTerrain->getHeightAt(p[0],p[2]);
	smKey->setPos(p);
	int numtrees = 100;
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

	t->render(pMVM);

}

int ObjectManager::getStars(){return numStars;}

void ObjectManager::updateAllObjects(){

	thePlayer->keyboardUpdater();
	M3DVector3f tmp,ang;
	thePlayer->getPos(tmp,ang);
	tmp[1]=1.0+theTerrain->getHeightAt(tmp[0],tmp[2]);
	thePlayer->setPos(tmp);

	bool hitAlready=false;
	static int ignoreMoreHits=0;
	if(thePlayer->isColliding(smHouse)){
		if(pInput->getKeyState('w')){ camK->allowMove[Camerak::Forward] = false;}
		if(pInput->getKeyState('s')){ camK->allowMove[Camerak::Back] = false; }
		if(pInput->getKeyState('a')){ camK->allowMove[Camerak::Left] = false; }
		if(pInput->getKeyState('d')){ camK->allowMove[Camerak::Right] = false; }
	}else{
		if(pInput->getKeyState('w')){ camK->allowMove[Camerak::Forward] = true;}
		if(pInput->getKeyState('s')){ camK->allowMove[Camerak::Back] = true; }
		if(pInput->getKeyState('a')){ camK->allowMove[Camerak::Left] = true; }
		if(pInput->getKeyState('d')){ camK->allowMove[Camerak::Right] = true; }
	}
	if (thePlayer->isColliding(smKey)){
		float newpos[] = {0.0f, -20.0f, 0.0f};
		smKey->setPos(newpos);
		std::cout << "GOT A KEY!!!\n";
	}
	//M3DVector3f playerPos, keyPos;
	//M3DVector3f angle;
	//thePlayer->getPos(playerPos, angle);
	//smKey->getPos(keyPos);
	//float playerDist=0;
	//float keyDist=0;
	//float diffDist=0;
	//for (int i=0; i<3; i++)
	//{
	//	playerDist+= playerPos[i] * playerPos[i];
	//	keyDist+=	keyPos[i] * keyPos[i];
	//}
	//diffDist = playerDist - keyDist;
	//if (diffDist < 4)
	//{
	//	std::cout << "GOT A KEY!!!\n";
	//}

}


void ObjectManager::colliding(int type,GLFrame *obj){
	
	switch(type){
	case 0:
		//add to current stars
		
		break;
	case 1:
		//heart/health
		obj->SetOrigin(0,-10,0);
		thePlayer->addHealth();
		psndMan->play(SM_HEART);

		break;
	case 2:
		//baddie
		thePlayer->setPain();
		thePlayer->removeHealth();
		psndMan->play(SM_PAIN);
		break;
	default:
		cerr << "Dunno what the hell youre colliding with!?!?" << endl;
		MessageBox(NULL, "Dunno what the hell youre colliding with!?!?", "Fail!", MB_OK  | MB_ICONERROR);
		break;
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
