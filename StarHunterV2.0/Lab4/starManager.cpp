#include "starManager.h"
using namespace std;


starManager::starManager(GLGeometryTransform *pGLGTin,terrain *map,GLuint*startex)
{
	this->pGLGT=pGLGTin;
	this->theTerrain= map;
	this->starTexture=startex;
	this->loadStarPositions("1.starpos");
	this->aStar = new star(&starTexture[0],pGLGT);
	this->verticalPos=0.0f;
	
	if(numStars<0 || numStars > 64){
		cout << "Stars did not load correctly!\nGame's a bogey!" << endl;
		MessageBox(NULL, "Stars did not load correctly!\nGame's a bogey!", "Fail!", MB_OK);
		glutLeaveMainLoop();
	}else{
	
		theStars = new GLFrame[numStars];
		for(int i=0;i<numStars;i++){
			positions[i][1] = 1+map->getHeightAt( positions[i][0],
												positions[i][2] );
			theStars[i].SetOrigin(positions[i]);
		
		}

		
	}
}

void starManager::loadStarPositions(char *fname){

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
		cout << "file " << fname << " loaded" << endl;
	}
	else
	{	
		cerr << "Unable to open file " << fname << endl;
	}
	
	stringstream strstream(memblock);

	string check;
	strstream >> check;
	if (check.compare(0,7,"starMan")!=0)
	{	
		cerr << "File " << fname << " is not a uwsm file." << endl;
	}else{

		
		strstream >> this->numStars;


		positions = new M3DVector3f[this->numStars];

		for(int i=0;i<this->numStars;i++){
			strstream >> positions[i][0];
			positions[i][1] =0;
			strstream >> positions[i][2];
		}
	}

	delete[] memblock;

}
void starManager::renderAllStars(GLMatrixStack *pMVM){
	for(int i=0;i<numStars;i++){
		pMVM->PushMatrix();
		pMVM->MultMatrix(theStars[i]);

		aStar->render(pMVM,pGLGT);

		pMVM->PopMatrix();
	}
}
void starManager::updateAllStars(){
	for(int i=0;i<numStars;i++){
		theStars[i].RotateLocalY(0.03);
		theStars[i].TranslateLocal(0.0f,0.01*cos(this->verticalPos),0.0f);
	}
	this->verticalPos+=0.03f;
}
starManager::~starManager(void)
{
}
