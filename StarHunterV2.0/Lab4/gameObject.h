#pragma once
#include "Shader.h"

class GameObject
{
protected:
	GLGeometryTransform *pGLGT;
	MyTBatch *mesh;
	GLFrame *myFrame;
	Camera *theCam;
	int ptype;
	float radius;
	float scaleValue;
	Shader *myS;
	bool multi;
	int numParts;

	GLuint *myTex;
	GLuint myShader;
	GLint locAmbient;
	GLint locDiffuse;
	GLint locSpecular;
	GLint locLight ;
	GLint locMVP;
	GLint locMV ;
	GLint locNM  ;
	GLint locTexture;
	
	
public:
	void getPos(float*pos);
	void getAngle(float*ang);
	void setAngle(float*ang);
	void setAngle(float degrees);
	void setPos(float*pos);
	float getScale(){return scaleValue;}
	void setScale(float nScale){scaleValue = nScale;}

	void render(GLMatrixStack *pMVM,GLGeometryTransform *pGLGTin);
	GameObject();
	GameObject(GLGeometryTransform*pGLGTin);
	float getRad();
	int getType();
	~GameObject(void);
};

