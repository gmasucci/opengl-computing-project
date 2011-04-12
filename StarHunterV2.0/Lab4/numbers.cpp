#include "numbers.h"


numbers::numbers(GLuint *texIN)
{
	this->tex=texIN;

	m3dMakeOrthographicMatrix(screenspace,0.0f,800,0.0f,600,-1.0f,1.0f);
	
	x=30;y=0;z=0;

	theNumbers = new GLBatch[10];
	
	for(int i=0;i<300;i+=30){
	
		theNumbers[i/30].Begin(GL_TRIANGLE_FAN,4,1);
	
			theNumbers[i/30].MultiTexCoord2f(0,i,40);
			theNumbers[i/30].Vertex3f(128,560,0);

			theNumbers[i/30].MultiTexCoord2f(0,i,0);
			theNumbers[i/30].Vertex3f(128,520,0);

			theNumbers[i/30].MultiTexCoord2f(0,i+30,0);
			theNumbers[i/30].Vertex3f(158,520,0);

			theNumbers[i/30].MultiTexCoord2f(0,i+30,40);
			theNumbers[i/30].Vertex3f(158,560,0);

		theNumbers[i/30].End();

	}

	myShader = gltLoadShaderPairWithAttributes("overlay.vert", "overlay.frag", 
			2, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_TEXTURE0, "vTexCoord");
                 

	locRectMVP = glGetUniformLocation(myShader, "mvpMatrix");
	locRectTexture = glGetUniformLocation(myShader, "rectangleImage");



}

void numbers::render(int numberToRender,int x,int y){

	m3dTranslationMatrix44(this->translate,x,y,this->z);
	M3DMatrix44f temp;
	m3dMatrixMultiply44(temp,screenspace,translate);

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	
	glUseProgram(myShader);
	glUniform1i(locRectTexture, 0);
	glUniformMatrix4fv(locRectMVP, 1, GL_FALSE, temp);
	
	glBindTexture(GL_TEXTURE_RECTANGLE, tex[0]);
	int n = numberToRender;
	if(numberToRender<0||numberToRender>99){
 		//MessageBox(NULL, "I can't display a number less than 0!\nFix it!", "Fail!", MB_OK | MB_ICONERROR);
	}else{
		
		if(n>9){
			int tens = n/10;
			int units= n-(tens*10);
			theNumbers[tens ].Draw();
			m3dTranslationMatrix44(this->translate,x+30,y+0,0);
			m3dMatrixMultiply44(temp,screenspace,translate);
			glUniformMatrix4fv(locRectMVP, 1, GL_FALSE, temp);
			theNumbers[units].Draw();
		}else{
			theNumbers[n].Draw();
		}
	}
	glEnable(GL_DEPTH_TEST);
}

numbers::~numbers(void)
{
}
