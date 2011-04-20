#pragma once
#include "game.h"
#include "Toolbox.h"
#include <GLFrame.h>
#include "loaders.h"



Game::Game(int argc,char* argv[]){
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

void Game::init(){
	
//	p=1;
	this->sndMan = new SoundManager(); 
	

	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_ALPHA_TEST);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glGenTextures(15,textures);
	Loaders::overlay("load.tga",&textures[9]);
	Loaders::overlay("loading.tga",&textures[12]);
	//loading screen;
	//sndMan->play(SM_FLOPPY,1);	

	loading = new Overlay(&textures[9],800,600,0,600);
	loaded = new Overlay(&textures[12],300,100,250,350);
	loading->render();
	
	glutPostRedisplay();
	glutSwapBuffers();

	Loaders::tex("skin.tga",&textures[0]);
	Loaders::tex("star.tga" ,&textures[1]);
	Loaders::tex("grass.tga",&textures[3]);
	Loaders::tex("beast.tga",&textures[4]);
	Loaders::tex("heart.tga",&textures[5]);
	Loaders::overlay("font.tga",&textures[6]);
	Loaders::overlay("starOver.tga",&textures[7]);
	Loaders::overlay("heartOver.tga",&textures[8]);
	Loaders::overlay("GO.tga",&textures[10]);
	Loaders::overlay("WIN.tga",&textures[11]);
	

	glGenTextures(2,treetex);
	Loaders::tex("trunk.tga",&treetex[0]);
	Loaders::tex("leaf.tga",&treetex[1]);
	
	glGenTextures(3,house);
	Loaders::tex("brick.tga",&house[0]);
	Loaders::tex("roof.tga",&house[1]);
	Loaders::tex("door.tga",&house[2]);

	glGenTextures(1,stumpTex);
	Loaders::tex("stump.tga",&stumpTex[1]);
	stumpTex[0] = treetex[0];//already loaded so copy uint so GL uses that texture


	oHeart = new Overlay(&textures[8],128,128,0,600);
	oStar = new Overlay(&textures[7],128,128,672,600);
	oGameOver = new Overlay(&textures[10],400,200,200,400);
	oWinner = new Overlay(&textures[11],400,200,200,300);
	counters = new Numbers(&textures[6]);
	myCam = new Camera(&modelViewMatrix);
	theRealCam = new Camerak(&modelViewMatrix);
	hm = new Terrain ("h4.pgm",3,&textures[3],&transformPipeline);
	
	mySky = new Skybox();

	hayden = new Player(&textures[4],&transformPipeline,hm,myCam,sndMan);

	mySM = new ObjectManager(
		&transformPipeline,
		hayden,
		hm,
		treetex,
		house,
		&textures[4],
		stumpTex,
		myCam,
		theRealCam,
		&input,
		sndMan
	);
	
	
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f ); // white background
	glEnable(GL_DEPTH_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);	
	//glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH)*0.5,glutGet(GLUT_WINDOW_HEIGHT)*0.5);
	transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
	modelViewMatrix.LoadIdentity();
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutSetCursor(GLUT_CURSOR_NONE);
	//sndMan->stop(SM_FLOPPY);

	
	//sndMan->play(SM_MUSIC,1);
}
void Game::update(){
	
	mySM->updateAllObjects();
	M3DVector3f v,fv;
	theRealCam->getFrame().GetOrigin(v);
	theRealCam->getFrame().GetForwardVector(fv);
	v[1] = hm->getHeightAt(v[0],v[2]) + 1.3;
	theRealCam->setOrigin(v);
	hayden->setPos(v);
	hayden->setAng(fv);
	myCam->update(v,fv);

	if(input.getKeyState(27)){glutLeaveMainLoop();}
	if(input.getKeyState('q')){
		M3DVector3f p,a;
		hayden->getPos(p,a);
		std::cout << "pos = " << p[0] << ", " << p[1] << ", " << p[2] << std::endl;
	}

	if(input.getKeyState('w')){theRealCam->moveForward();}
	if(input.getKeyState('a')){theRealCam->strafeLeft();}
	if(input.getKeyState('s')){theRealCam->moveBackward();}
	if(input.getKeyState('d')){theRealCam->strafeRight();}
	if(input.getKeyState('e')){/*use button*/}

	int xcentre = glutGet(GLUT_WINDOW_WIDTH) / 2;		// stores the current horizontal centre iof the screen
	int ycentre = glutGet(GLUT_WINDOW_HEIGHT) / 2;		// stores the current vertical centre of the screen
	
	float tempx = input.getMouseX();
	float tempy = input.getMouseY();
	float offset = 0;
	if(tempx < xcentre - 1 || tempx > xcentre + 1)				// provides a dead zone in the centre of the screen
	{
		offset = (xcentre - tempx) * 0.003f;
		theRealCam->rotateAntiClockWise(offset);
	}
	if(tempy < ycentre - 1 || tempy > ycentre + 1)				// provides a dead zone in the centre of the screen
	{
		offset = (ycentre - tempy) * 0.003f;
		theRealCam->rotateUp(offset);
	}
	if(offset != 0)
	{
		glutWarpPointer((int)xcentre, (int)ycentre);
	}
}

void Game::display(){
	//sqRot++; 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	float t = this->updater.GetElapsedSeconds();
	if(t>0.0166666668){ //update 60 times/sec
		this->update();
		this->updater.Reset();
	}
	
	

	
	// Skybox
	//***********
	theRealCam->PushMatrix(true);	//true for skybox only
	mySky->draw(&transformPipeline);
	theRealCam->PopMatrix();	//pop off the rotation only transform for the camera
	//***********

	theRealCam->PushMatrix();//no params, means standard transforms (ie not rotation only)

		hm->drawMe(theRealCam);
		mySM->renderAllObjects(&modelViewMatrix);
	
	theRealCam->PopMatrix();		//last pop. for cam matrix


	//render overlays
	
	updateOverlays();

	glutPostRedisplay();
	glutSwapBuffers();

}


void Game::updateOverlays(){


}




void Game::reshape(int w,int h){
	glViewport(0, 0, w, h);
	viewFrustum.SetPerspective(60.0f, float(w) / float(h), 0.1f, 1000.0f);
	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
}
void Game::gameMain(){
	glutMainLoop();
}
Game::~Game(){
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