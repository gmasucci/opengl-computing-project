#pragma once
#include "grid.h"
#include "loaders.h"
#include "Camera.h"
#include "MyTBatch.h"

typedef unsigned char byte;

class terrain
{
private:
	GLGeometryTransform *pGLGT;
	GLFrame myFrame;
	MyTBatch myBatch;
	GLTriangleBatch myTri;
	unsigned char *heights;
	//float *mesh;
	GLfloat vBlack[4];
	GLuint *myTex;
	Vec3 *vertex;
	Vec3 *normal;
	int size;
	int numVerts;
	float scale;
	float yscale;
	GLuint grassShader;
	GLint locAmbient;
	GLint locDiffuse;
	GLint locSpecular;
	GLint locLight ;
	GLint locMVP;
	GLint locMV ;
	GLint locNM  ;
	GLint locTexture;

public:
	terrain(char *fname,float scaleIn,GLuint *tex,GLGeometryTransform *pGLGTin);
	float getHeight(float x, float z);
	float getHeightAt(float x, float z);
	void avgNormals();
	void drawV(const int x,const int z);
	void drawMe(Camera *camIn);
	void getCPAt(float*result,float x, float z);
	~terrain();

};

