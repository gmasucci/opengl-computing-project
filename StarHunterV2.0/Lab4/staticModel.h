#pragma once
#include "gameObject.h"
class staticModel : public gameObject
{
private:
	GLint locInvertCam;
	GLint colour;
	GLTriangleBatch tbmax;
	GLFrame tbmaxframe;
	GLTriangleBatch tbmin;
	GLFrame tbminframe;
protected:
	bool spinning;
	bool drawBounds;

public:
	Vec3 max,min;
	staticModel();
	staticModel(
		char* fname,
		GLuint *tex,
		GLGeometryTransform *pGLGT,
		Camera *camIn,
		bool multi = true);
	void render(GLMatrixStack *pMVM, bool spcblnd = false);
	void getColInfo(Vec3 *max,Vec3 *min);
	void setDrawBounds(bool b = false){drawBounds = b;}
	void setSpinning(bool b = false){spinning = b;}
	~staticModel(void);
};

