#pragma once
#include "Toolbox.h"

class Loaders //contains static functions to load textures & models
{
public:
	//loads a tga texture
	static void tex(char *fname,GLuint *texID);
	//loads 6 tga textures 
	static void cubemap(char **cubefaces, GLuint *cubeTexture);
	//loads uwsm file
	static void uwsm(char *fname, GLTriangleBatch *obj);
	static void uwsm(char *fname, MyTBatch *obj,Vec3 *cmax, Vec3 *cmin);
	static void normGen(M3DVector3f v[3], M3DVector3f out);
	static void normGen(float *v[3], float* normal);
	static Vec3 normGen(Vec3 v[3]);
	
	static int uwsmMultiCheck(char *fname);
	static void uwsmMultiLoad(char *fname, MyTBatch *obj,Vec3 *cmax, Vec3 *cmin);
	static void uwsmComponent(const char *fname, MyTBatch *obj,Vec3 *cmax, Vec3 *cmin);

	static void overlay(char *fname, GLuint *texID);
	static void *sound(char *fname);
};

