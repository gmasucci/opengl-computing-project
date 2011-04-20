#include "staticModel.h"


StaticModel::StaticModel(){}

StaticModel::StaticModel	(
			char* fname,
			GLuint *tex,
			GLGeometryTransform *pGLGTin,
			Camera *camIn,
			bool mult
			):GameObject(pGLGTin)
{
	myFrame = new GLFrame();
	multi = mult;
	myTex=tex;
	Vec3 tmax,tmin;
	drawBounds = false;
	spinning = false;
	//std::cout << "tmin.x = " << tmin.x << ", min.y = " << min->y << std::endl;
	if(multi){
		numParts = Loaders::uwsmMultiCheck(fname);
		mesh = new MyTBatch[numParts];
		Loaders::uwsmMultiLoad(fname,mesh,&tmax,&tmin);
	}else{
		mesh = new MyTBatch();
		Loaders::uwsm(fname,mesh,&tmax,&tmin);
	}

	max.x = tmax.x;
	max.y = tmax.y;
	max.z = tmax.z;

	min.x = tmin.x;
	min.y = tmin.y;
	min.z = tmin.z;


	scaleValue=1;
	this->radius = 0.4f;
	this->ptype = 0;
	this->theCam = camIn;
	myS = new Shader(camIn,"Shaders/nospec.vert","Shaders/nospec.frag",&myTex[0]);
	gltMakeSphere(tbmax,scaleValue,6,6);
	tbmaxframe.TranslateWorld(max.x,0.0f,max.z);
	gltMakeSphere(tbmin,5,6,6);
	tbminframe.TranslateWorld(min.x,0.0f,min.z);
}

void StaticModel::render(GLMatrixStack *pMVM, bool spcblnd){

	if(spcblnd){
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);// < good ghost effect :)
		//glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
	}

	//myFrame->RotateLocalY(0.01);
	if(spinning){
		myFrame->RotateLocalY(0.02f);
	}

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

	if(drawBounds){
		pMVM->PushMatrix();
		pMVM->MultMatrix(tbmaxframe);
		tbmax.Draw();
		pMVM->PopMatrix();
		pMVM->PushMatrix(tbminframe);
		tbmin.Draw();
		pMVM->PopMatrix();
	}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


}

StaticModel::~StaticModel(void){
	if(multi){
		delete[] mesh;
	}else{
		delete mesh;
	}
	delete myS;
	delete myFrame;
	//other objects reference only, do not delete!
}

void StaticModel::getColInfo(Vec3 *cmax,Vec3 *cmin){

	M3DVector3f m;
	this->myFrame->GetOrigin(m);

	cmax->x = (max.x*scaleValue) + m[0];
	cmax->y = (max.y*scaleValue) + m[1];
	cmax->z = (max.z*scaleValue) + m[2];

	cmin->x = (min.x*scaleValue) + m[0];
	cmin->y = (min.y*scaleValue) + m[1];
	cmin->z = (min.z*scaleValue) + m[2];
}