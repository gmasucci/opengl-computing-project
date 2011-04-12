#include "health.h"


health::health(){}

health::health(GLuint *tex,GLGeometryTransform *pGLGTin,Camera *camIn):gameObject(pGLGTin)
{
	myTex=tex;
	loaders::uwsm("heart.uwsm",&this->myBatch);
	scaleValue=0.01;
	this->radius = 0.1;
	this->ptype = 1;
	this->theCam = camIn;

	myShader = gltLoadShaderPairWithAttributes("phong.vert", "phong.frag", 3, GLT_ATTRIBUTE_VERTEX, "vVertex",
			GLT_ATTRIBUTE_NORMAL, "vNormal", GLT_ATTRIBUTE_TEXTURE0, "vTexture0");

	locAmbient = glGetUniformLocation(myShader, "ambientColor");
	locDiffuse = glGetUniformLocation(myShader, "diffuseColor");
	locSpecular = glGetUniformLocation(myShader, "specularColor");
	locLight = glGetUniformLocation(myShader, "vLightPosition");
	locMVP = glGetUniformLocation(myShader, "mvpMatrix");
	locMV  = glGetUniformLocation(myShader, "mvMatrix");
	locNM  = glGetUniformLocation(myShader, "normalMatrix");
	locTexture = glGetUniformLocation(myShader, "colorMap");
}

health::~health(void)
{
}
