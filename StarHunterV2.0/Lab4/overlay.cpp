#include "overlay.h"

overlay::overlay(){}

overlay::overlay(GLuint *texIN,int wd,int ht,int x, int y)
{
	tex=texIN;
	m3dMakeOrthographicMatrix(screenspace,0.0f,800,0.0f,600,-1.0f,1.0f);
	rect.Begin(GL_TRIANGLE_FAN,4,1);
	
	rect.MultiTexCoord2f(0,0,ht);
	rect.Vertex3f(x,y,0);

	rect.MultiTexCoord2f(0,0,0);
	rect.Vertex3f(x,y-ht,0);

	rect.MultiTexCoord2f(0,wd,0);
	rect.Vertex3f(x+wd,y-ht,0);

	rect.MultiTexCoord2f(0,wd,ht);
	rect.Vertex3f(x+wd,y,0);

	rect.End();

	myShader = gltLoadShaderPairWithAttributes("Shaders/overlay.vert", "Shaders/overlay.frag", 
			2, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_TEXTURE0, "vTexCoord");
                 

	locRectMVP = glGetUniformLocation(myShader, "mvpMatrix");
	locRectTexture = glGetUniformLocation(myShader, "rectangleImage");

}



void overlay::render(){

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	glUseProgram(myShader);
	glUniform1i(locRectTexture, 0);
	glUniformMatrix4fv(locRectMVP, 1, GL_FALSE, screenspace);
	glBindTexture(GL_TEXTURE_RECTANGLE, tex[0]);
	this->rect.Draw();
	glEnable(GL_DEPTH_TEST);
}
overlay::~overlay(void)
{
}
