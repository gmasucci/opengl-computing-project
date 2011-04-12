#pragma once
#include "game.h"
#include "grid.h"
#include <GLFrame.h>
#include "loaders.h"



game::game(int argc,char* argv[]){
	vWhite[0]=1.0f;
	vWhite[1]=1.0f;
	vWhite[2]=1.0f;
	vWhite[3]=1.0f;

	vLightPos[0] = 128.0f;
	vLightPos[1] = 30.0f;
	vLightPos[2] = 0.0f;
	gltSetWorkingDirectory(argv[0]);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	
	glutInitWindowPosition(0,0);
	glutInitWindowSize(800	,	600);
	glutCreateWindow("Star Hunter - V1.2");

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		//failed, something blew up
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		exit(9001); //This exit code is OVER 9000!!!!!!
	}


	this->init();
}

void game::init(){
	
	p=1;
	this->sndMan = new soundManager(); //deleted
	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_ALPHA_TEST);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glGenTextures(15,textures);
	loaders::overlay("load.tga",&textures[9]);
	loaders::overlay("loading.tga",&textures[12]);
	//loading screen;
	//sndMan->play(SM_FLOPPY,1);	

	loading = new overlay(&textures[9],800,600,0,600);
	loaded = new overlay(&textures[12],300,100,250,350);
	loading->render();
	
	glutPostRedisplay();
	glutSwapBuffers();

	loaders::tex("skin.tga",&textures[0]);
	loaders::tex("star.tga" ,&textures[1]);
	loaders::tex("grass.tga",&textures[3]);
	loaders::tex("beast.tga",&textures[4]);
	loaders::tex("heart.tga",&textures[5]);
	loaders::overlay("font.tga",&textures[6]);
	loaders::overlay("starOver.tga",&textures[7]);
	loaders::overlay("heartOver.tga",&textures[8]);
	loaders::overlay("GO.tga",&textures[10]);
	loaders::overlay("WIN.tga",&textures[11]);
	

	glGenTextures(2,treetex);
	loaders::tex("trunk.tga",&treetex[0]);
	loaders::tex("leaf.tga",&treetex[1]);
	
	glGenTextures(3,house);
	loaders::tex("brick.tga",&house[0]);
	loaders::tex("roof.tga",&house[1]);
	loaders::tex("door.tga",&house[2]);

	oHeart = new overlay(&textures[8],128,128,0,600);
	oStar = new overlay(&textures[7],128,128,672,600);
	oGameOver = new overlay(&textures[10],400,200,200,400);
	oWinner = new overlay(&textures[11],400,200,200,300);
	counters = new numbers(&textures[6]);
	myCam = new Camera(&modelViewMatrix);

	hm = new terrain ("h4.pgm",3,&textures[3],&transformPipeline);
	
	mySky = new skybox();
	hayden = new player(&textures[4],&transformPipeline,hm,myCam,sndMan);

	mySM = new objectManager(
		&transformPipeline,
		hayden,
		hm,
		treetex,
		house,
		&textures[4],
		myCam,
		sndMan
	);
	

	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f ); // white background
	glEnable(GL_DEPTH_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);	
		
	transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
	modelViewMatrix.LoadIdentity();
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	//sndMan->stop(SM_FLOPPY);

	
	//sndMan->play(SM_MUSIC,1);
}
void game::update(){
	//system("cls");
	M3DVector3f tpos,tang,temp;
	if(p==1)
	{
		mySM->updateAllObjects();
		hayden->getPos(tpos,tang);
		myCam->update(tpos,tang);
		
	}else{
		hayden->getPos(tpos,tang);
		std::cout << tpos[0] << ", " << tpos[1] << ", " << tpos[2] << std::endl;
	}
}

void game::display(){
	//sqRot++; 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	float t = this->updater.GetElapsedSeconds();
	if(t>0.0166666668){ //update 60 times/sec
		this->update();
		this->updater.Reset();
	}
	
	

	
	// Skybox
	//***********
	myCam->pushMatrix(true);	//true for skybox only
	mySky->draw(&transformPipeline);
	myCam->popcam();	//pop off the rotation only transform for the camera
	//***********

	myCam->pushMatrix();//no params, means standard transforms (ie not rotation only)

		hm->drawMe(myCam);
		mySM->renderAllObjects(&modelViewMatrix);
	
	myCam->popcam();		//last pop. for cam matrix


	//render overlays
	
	updateOverlays();

	glutPostRedisplay();
	glutSwapBuffers();

}


void game::updateOverlays(){
	int temp;
	temp = hayden->getStars();
	if(temp>9){
		//counters->render(temp,484,0);
	}else{
		//counters->render(temp,514,0);
	}

	temp = hayden->getHealth();
	//counters->render(temp,0,0);
	if(p==0){
		
	}else{
	
	}

}
void game::keys(unsigned char key, int x, int y){
	
	hayden->keys(key);

	if(key==27){
	glutLeaveMainLoop();


	}else{
		
	switch(key){
	
	case 'w':
		w = 1;	
		break;
	case 's':
		s = 1;
		break;
	case 'a':
		a=1;
		break;
	case 'd':
		d=1;
		break;
	case 'k':
		k=1;
		break;
	case 'l':
		l=1;
		break;
	case 'p':
		if(p==1)
			p=0;
		else
			p=1;
		break;
	default:
		break;
	}
	}
}
void game::keysup(unsigned char key, int x, int y){

	hayden->keysUp(key);

	switch(key){

	case 'w':
		w = 0;	
		break;
	case 's':
		s = 0;
		break;
	case 'a':
		a=0;
		break;
	case 'd':
		d=0;
		break;
	case 'k':
		k=0;
		break;
	case 'l':
		l=0;
		break;
	case 'p':
		break;

	}
}
void game::reshape(int w,int h){
	glViewport(0, 0, w, h);
	viewFrustum.SetPerspective(60.0f, float(w) / float(h), 0.1f, 1000.0f);
	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
}
void game::gameMain(){
	glutMainLoop();
}
game::~game(){
//for future deletions.
	delete oHeart;
	delete oStar;
	delete oGameOver;
	delete oWinner;
	delete counters;
	delete myCam;
	delete hm;
	delete mySky;
	delete hayden;
	delete mySM;
	delete sndMan;
	


}