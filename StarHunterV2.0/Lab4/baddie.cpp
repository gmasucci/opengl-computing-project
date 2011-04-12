#include "baddie.h"


baddie::baddie(){}

baddie::baddie(GLuint *tex,GLGeometryTransform *pGLGTin,Camera *camIn)
{
	this->myTex = tex;
	scaleValue = 0.01;
	pGLGT=pGLGTin;
	this->radius=0.25;
	this->ptype = 2;
	this->theCam=camIn;

	numParts = loaders::uwsmMultiCheck("beast.multi");
	bodypart = new GLTriangleBatch[numParts];
	loaders::uwsmMultiLoad("beast.multi",bodypart);

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

void baddie::render(GLMatrixStack *pMVM){

	pMVM->Scale(scaleValue,scaleValue,scaleValue);

	M3DVector4f vEyeLight,vLight = { 128.0f, 50, 0 ,1.0f};
	M3DMatrix44f *mx;
	mx = theCam->getMx();
    m3dTransformVector4(vEyeLight, vLight, *mx);
		
	GLfloat vAmbientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat vDiffuseColor[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat vSpecularColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glBindTexture(GL_TEXTURE_2D,this->myTex[0]);
	glUseProgram(myShader);
	glUniform4fv(locAmbient, 1, vAmbientColor);
	glUniform4fv(locDiffuse, 1, vDiffuseColor);
	glUniform4fv(locSpecular, 1, vSpecularColor);
	glUniform3fv(locLight, 1, vEyeLight);
	glUniformMatrix4fv(locMVP, 1, GL_FALSE, pGLGT->GetModelViewProjectionMatrix());
	glUniformMatrix4fv(locMV, 1, GL_FALSE, pGLGT->GetModelViewMatrix());
	glUniformMatrix3fv(locNM, 1, GL_FALSE, pGLGT->GetNormalMatrix());
	glUniform1i(locTexture, 0);

	for(int i=0;i<this->numParts;i++){
		bodypart[i].Draw();
	}

}

void baddie::renderMultiple(GLMatrixStack *pMVM,GLFrame &theFrame){
	pMVM->PushMatrix();
	pMVM->MultMatrix(theFrame);
	
	this->render(pMVM);

	pMVM->PopMatrix();
}

void baddie::setup(GLuint *tex,GLGeometryTransform *pGLGTin){
	this->myTex=tex;
	this->pGLGT = pGLGTin;
}

baddie::~baddie(void)
{
}
