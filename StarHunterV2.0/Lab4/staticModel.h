#pragma once
#include "gameObject.h"
class staticModel : public gameObject
{
private:
	GLint locInvertCam;
	GLint colour;
	GLTriangleBatch tb;

public:
	staticModel();
	staticModel(
		char* fname,
		GLuint *tex,
		GLGeometryTransform *pGLGT,
		Camera *camIn,
		bool multi = true);
	void render(GLMatrixStack *pMVM, bool spcblnd = false);
	~staticModel(void);
};

