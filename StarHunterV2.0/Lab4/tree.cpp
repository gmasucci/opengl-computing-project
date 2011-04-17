#include "tree.h"



Tree::Tree(int numtrees,GLuint *tex,Terrain *map,GLGeometryTransform *pGLGTin,	Camera *camIn)
{
	clones = numtrees;
	treeFrame = new GLFrame[clones];

	char* fname = "tree";
	this->multi = true;
	numParts = Loaders::uwsmMultiCheck(fname);
	mesh = new MyTBatch[numParts];
	Vec3 tmax,tmin;
	Loaders::uwsmMultiLoad(fname,mesh,&tmax,&tmin);

	theTerrain = map;
	this->pGLGT = pGLGTin;
	this->myTex = tex;
	this->scaleValue=0.5f;
	this->radius = 0.4f;
	this->ptype = 0;
	this->theCam = camIn;
	myS = new Shader(camIn,"Shaders/nospec.vert","Shaders/nospec.frag",&myTex[0]);
	setup();
}

void Tree::setup()
{

	for(int i = 0;i<clones;i++){
		M3DVector3f p;
		srand((unsigned int)i);
		treeFrame[i].RotateLocalY((float)(D2R*(rand() % 360)));
		int a,c; // RANDOM MADNESS!!!!!!!!
		c = (rand()%100);
		if(c<50){
			//random x, between 1 - 10 or 245 - 255
			a = rand() % 10;
			if(a<5){
				p[0] = (float)((rand() % 10)+1);
			}else{
				p[0] = (float)((rand() % 15)+250);
			}
			//now, if x is clamped, z can be any value
			p[2] = (float)((rand() % 245));

			//get height at coords
			p[1] = this->theTerrain->getHeightAt(p[0],p[2]);
			treeFrame[i].SetOrigin(p);
		}else{

			//random z, between 1 - 20 or 250 - 265
			a = rand() % 10;
			if(a<5){
				p[2] = (float)((rand() % 20)+1);
			}else{
				p[2] = (float)((rand() % 15)+250);
			}
			//now, if z is clamped, x can be any value
			p[0] = (float)((rand() % 245));
				
			//get height at coords
			p[1] = this->theTerrain->getHeightAt(p[0],p[2]);
			treeFrame[i].SetOrigin(p);
			
		}
	}
}

void Tree::render(GLMatrixStack *pMVM)
{
	for(int j=0;j<clones;j++){
		pMVM->PushMatrix();
		pMVM->MultMatrix(treeFrame[j]);

		pMVM->Scale(scaleValue,scaleValue,scaleValue);

		if(multi){
			for(int i=0;i<numParts;i++){
				myS->useTex(&myTex[i]);
				myS->bind(pMVM,pGLGT);
				mesh[i].Draw();	
			}
		}else{
			myS->bind(pMVM,pGLGT);
			mesh->Draw();	
		}
			
		pMVM->PopMatrix();
	}
		
}

Tree::~Tree(void)
{
}
