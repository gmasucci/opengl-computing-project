#include "objectManager.h"
using namespace std;

objectManager::objectManager(GLGeometryTransform *pGLGTin,
				player *playerIn,
				terrain *map,
				GLuint *startex,
				GLuint *hearttex,
				GLuint *baddietex,
				Camera *camIn,
				soundManager *sndManPtr)

{
	psndMan = sndManPtr;
	this->pGLGT=pGLGTin;
	this->theTerrain= map;
	this->thePlayer=playerIn;
	this->verticalPos=0.0f;
	
	M3DVector3f tmp,ang;
	thePlayer->getPos(tmp,ang);
	tmp[0] = tmp[2] = 30.0f;
	tmp[1]=theTerrain->getHeightAt(tmp[0],tmp[2]);
	thePlayer->setPos(tmp);
	thePlayer->setAngle(235.0f);

	aStar = new staticModel("house",hearttex,pGLGT,camIn);
	M3DVector3f p;
	p[0] = p[2] = 50.0f;
	p[1] = theTerrain->getHeightAt(p[0],p[2]);
	aStar->setPos(p);
	int numtrees = 100;
	t = new tree(numtrees,startex,map,pGLGT,camIn);


}

void objectManager::loadPositions(char *fname){

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
void objectManager::renderAllObjects(GLMatrixStack *pMVM){
	
	aStar ->render(pMVM);
	t->render(pMVM);

}

int objectManager::getStars(){return numStars;}

void objectManager::updateAllObjects(){

	thePlayer->keyboardUpdater();
	M3DVector3f tmp,ang;
	thePlayer->getPos(tmp,ang);
	tmp[1]=1.0+theTerrain->getHeightAt(tmp[0],tmp[2]);
	thePlayer->setPos(tmp);


}
void objectManager::colliding(int type,GLFrame *obj){
	
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


objectManager::~objectManager(void)
{
	//delete[] starpositions;
	//delete[] heartpositions;
	//delete[] baddiepositions;
	//delete[] theBaddies;
	//delete[] theHearts;
	//delete[] theStars;
}
