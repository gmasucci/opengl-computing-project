#include "skybox.h"


skybox::skybox()
{
	//initial operations
	gltMakeCube(skyCube, 20.0f);
	
	filenames[0]="pos_x.tga";
	filenames[1]="neg_x.tga";
	filenames[2]="pos_y.tga";
	filenames[3]="neg_y.tga";
	filenames[4]="pos_z.tga";
	filenames[5]="neg_z.tga";

	//load textures using loaders library.
	loaders::cubemap(filenames,&skyTexture);

	//load&compile shaders
	skyboxShader = gltLoadShaderPairWithAttributes("texonly.vert", "texonly.frag", 2, 
                                                GLT_ATTRIBUTE_VERTEX, "vVertex",
                                                GLT_ATTRIBUTE_NORMAL, "vNormal");
	
	//get location of combined modelview/projecton matrix
	locMVPSkyBox = glGetUniformLocation(skyboxShader, "mvpMatrix");

}

void skybox::draw(GLGeometryTransform *pGLGT){
	glDisable(GL_DEPTH_TEST);
	glUseProgram(skyboxShader);
	glUniformMatrix4fv(locMVPSkyBox, 1, GL_FALSE, pGLGT->GetModelViewProjectionMatrix());
	skyCube.Draw(); 
	glEnable(GL_DEPTH_TEST);
}

skybox::~skybox()
{
	glDeleteTextures(1, &skyTexture);
}
