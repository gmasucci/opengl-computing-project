#include "staticModel.h"


staticModel::staticModel(){}

staticModel::staticModel	(
			char* fname,
			GLuint *tex,
			GLGeometryTransform *pGLGTin,
			Camera *camIn,
			bool mult
			):gameObject(pGLGTin)
{
	myFrame = new GLFrame();
	multi = mult;
	myTex=tex;
	if(multi){
		numParts = loaders::uwsmMultiCheck(fname);
		mesh = new MyTBatch[numParts];
		loaders::uwsmMultiLoad(fname,mesh);
	}else{
		mesh = new MyTBatch();
		loaders::uwsm(fname,mesh);
	}
	
	scaleValue=0.5f;
	this->radius = 0.4f;
	this->ptype = 0;
	this->theCam = camIn;
	myS = new Shader(camIn,"Shaders/nospec.vert","Shaders/nospec.frag",&myTex[0]);

}

void staticModel::render(GLMatrixStack *pMVM, bool spcblnd){

	if(spcblnd){
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE); < good ghost effect :)
		glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
	}

	//myFrame->RotateLocalY(0.01);

	pMVM->PushMatrix();
	pMVM->MultMatrix(*myFrame);

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


		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


}

staticModel::~staticModel(void){
	if(multi){
		delete[] mesh;
	}else{
		delete mesh;
	}
	delete myS;

	//other objects reference only, do not delete!
}
