#include "gameObject.h"

gameObject::gameObject(){}
gameObject::gameObject(GLGeometryTransform *pGLGTin)
{
	pGLGT=pGLGTin;
}

void gameObject::getPos(float*pos){myFrame->GetOrigin(pos);}
void gameObject::setPos(float*pos){myFrame->SetOrigin(pos);}
void gameObject::getAngle(float*pos){myFrame->GetForwardVector(pos);}
void gameObject::setAngle(float*pos){myFrame->SetForwardVector(pos);}
void gameObject::setAngle(float degrees){myFrame->RotateLocalY((PI/180)*degrees);}


void gameObject::render(GLMatrixStack *pMVM, GLGeometryTransform *pGLGTin){
	pMVM->PushMatrix();
	pMVM->MultMatrix(*myFrame);

	pMVM->Scale(scaleValue,scaleValue,scaleValue);
	
	//myS->useTex(*myTex);
	myS->bind(pMVM,pGLGTin);

	mesh->Draw();
	
	pMVM->PopMatrix();
}

float gameObject::getRad(){return radius;}
int gameObject::getType(){return this->ptype;}
gameObject::~gameObject(void){
	//if multi
	//delete[] mesh;
	//else
	//delete mesh;
}
