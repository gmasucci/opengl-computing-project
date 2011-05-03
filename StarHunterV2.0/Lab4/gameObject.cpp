#include "gameObject.h"

GameObject::GameObject(){}
GameObject::GameObject(GLGeometryTransform *pGLGTin)
{
	pGLGT=pGLGTin;
}

void GameObject::getPos(float*pos){myFrame->GetOrigin(pos);}
void GameObject::setPos(float*pos){myFrame->SetOrigin(pos);}
void GameObject::getAngle(float*pos){myFrame->GetForwardVector(pos);}
void GameObject::setAngle(float*pos){myFrame->SetForwardVector(pos);}
void GameObject::setAngle(float degrees){myFrame->RotateLocalY(1.570795*degrees);}


void GameObject::render(GLMatrixStack *pMVM, GLGeometryTransform *pGLGTin){
	pMVM->PushMatrix();
	pMVM->MultMatrix(*myFrame);

	pMVM->Scale(scaleValue,scaleValue,scaleValue);
	
	//myS->useTex(*myTex);
	myS->bind(pMVM,pGLGTin);

	mesh->Draw();
	
	pMVM->PopMatrix();
}

float GameObject::getRad(){return radius;}
int GameObject::getType(){return this->ptype;}
GameObject::~GameObject(void){
	//if multi
	//delete[] mesh;
	//else
	//delete mesh;
}
