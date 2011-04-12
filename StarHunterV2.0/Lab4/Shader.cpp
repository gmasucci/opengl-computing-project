#include "Shader.h"


Shader::Shader(Camera *camIn,const char* fv,const char* ff,GLuint *tex){

	myTex = tex;	
	theCam = camIn;
	me = gltLoadShaderPairWithAttributes(fv, ff, 3, GLT_ATTRIBUTE_VERTEX, "vVertex",
			GLT_ATTRIBUTE_NORMAL, "vNormal", GLT_ATTRIBUTE_TEXTURE0, "vTexture0");

	locAmbient = glGetUniformLocation(me, "ambientColor");
	locDiffuse = glGetUniformLocation(me, "diffuseColor");
	locSpecular = glGetUniformLocation(me, "specularColor");
	locLight = glGetUniformLocation(me, "vLightPosition");
	locMVP = glGetUniformLocation(me, "mvpMatrix");
	locMV  = glGetUniformLocation(me, "mvMatrix");
	locNM  = glGetUniformLocation(me, "normalMatrix");
	locTexture = glGetUniformLocation(me, "colorMap");


}

void Shader::bind(GLMatrixStack *pMVM, GLGeometryTransform *pGLGTin){
	
	//default lighting
	M3DVector4f vEyeLight,vLight = { 128.0f, 128.0f, -128.0f ,1.0f};
	M3DMatrix44f *mx;
	mx = theCam->getMx();
    m3dTransformVector4(vEyeLight, vLight, *mx);
		
	GLfloat vAmbC[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat vDifC[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat vSpecC[] = { 1.0f, 1.0f, 1.0f, 1.0f };


	glBindTexture(GL_TEXTURE_2D,myTex[0]);
	glUseProgram(me);
	glUniform4fv(locAmbient, 1, vAmbC);
	glUniform4fv(locDiffuse, 1, vDifC);
	glUniform4fv(locSpecular, 1, vSpecC);
	glUniform3fv(locLight, 1, vEyeLight);
	glUniformMatrix4fv(locMVP, 1, GL_FALSE, pGLGTin->GetModelViewProjectionMatrix());
	glUniformMatrix4fv(locMV, 1, GL_FALSE, pGLGTin->GetModelViewMatrix());
	glUniformMatrix3fv(locNM, 1, GL_FALSE, pGLGTin->GetNormalMatrix());
	glUniform1i(locTexture, 0);
}


void Shader::useTex(GLuint *tex){
	myTex = tex;	
}

Shader::~Shader(void)
{
}
