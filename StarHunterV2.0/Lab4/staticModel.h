#pragma once
#include "gameObject.h"


class StaticModel : public GameObject
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
	StaticModel();
	StaticModel(
		char* fname,
		GLuint *tex,
		GLGeometryTransform *pGLGT,
		Camera *camIn,
		bool multi = true);
	void render(GLMatrixStack *pMVM, bool spcblnd = false);
	void getColInfo(Vec3 *max,Vec3 *min);
	void setDrawBounds(bool b = false){drawBounds = b;}
	void setSpinning(bool b = false){spinning = b;}
	~StaticModel(void);
};

