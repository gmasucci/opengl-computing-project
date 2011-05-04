#include "Hedges.h"


Hedges::Hedges(int numhedges,GLuint *tex,Terrain *map,GLGeometryTransform *pGLGTin,Camera *camIn)
{
	clones = numhedges;
	hedgeFrame = new GLFrame[clones];

	char* fname = "hedge";
	this->multi = true;
	numParts = Loaders::uwsmMultiCheck(fname);
	mesh = new MyTBatch[numParts];
	Vec3 tmax,tmin;
	Loaders::uwsmMultiLoad(fname,mesh,&tmax,&tmin);

	theTerrain = map;
	this->pGLGT = pGLGTin;
	this->myTex = tex;
	this->scaleValue=1.0f;
	this->radius = 0.4f;
	this->ptype = 0;
	this->theCam = camIn;
	myS = new Shader(camIn,"Shaders/nospec.vert","Shaders/nospec.frag",&myTex[0]);
	setup();
}

void Hedges::setup(){
	for(int i = 0;i<clones;i++){
		M3DVector3f a;
		a[0] = 30 + (i*3);
		a[2] = 30;
		a[1] = theTerrain->getHeightAt(a[0],a[2]);
		hedgeFrame[i].SetOrigin(a);
	}
}


void Hedges::render(GLMatrixStack *pMVM)
{
	for(int j=0;j<clones;j++){
		pMVM->PushMatrix();
		pMVM->MultMatrix(hedgeFrame[j]);

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
Hedges::~Hedges(void)
{
}
