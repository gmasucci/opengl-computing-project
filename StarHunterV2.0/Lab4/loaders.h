#pragma once
#include "grid.h"

class loaders //contains static functions to load textures & models
{
public:
	//loads a tga texture
	static void tex(char *fname,GLuint *texID);
	//loads 6 tga textures 
	static void cubemap(char **cubefaces, GLuint *cubeTexture);
	//loads uwsm file
	static void uwsm(char *fname, GLTriangleBatch *obj);
	static void uwsm(char *fname, MyTBatch *obj);
	static void normGen(M3DVector3f v[3], M3DVector3f out);
	static void normGen(float *v[3], float* normal);
	static Vec3 loaders::normGen(Vec3 v[3]);
	
	static int uwsmMultiCheck(char *fname);
	static void uwsmMultiLoad(char *fname, MyTBatch *obj);
	static void uwsmComponent(char *fname, MyTBatch *obj);

	static void overlay(char *fname, GLuint *texID);
	static void *sound(char *fname);
};

