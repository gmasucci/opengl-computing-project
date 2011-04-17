
#include "game.h"
using namespace std;

//Globals
Game *mygame = 0;

//callback functions, point to the game class
void reshape(GLint w, GLint h){ mygame->reshape(w,h); }
void display(void){	mygame->display(); }
void init(void){ mygame->init(); }
void keyboard(unsigned char key, int x, int y){	mygame->keysDn(key,x,y); }
void keyup(unsigned char key, int x, int y){	mygame->keysUp(key,x,y); }
void mouseMove(int x, int y){ mygame->mouseMove(x,y); }

//small func to shift the console window, I hate not being able to see it;
void moveConsole(){
 HWND windowHandle = GetConsoleWindow();
 MoveWindow(windowHandle,815,0,420, 600, true);
}
//main function
int main(int argc, char* argv[])
{	
	int x;
	moveConsole(); //shift that;

		mygame = new Game(argc,argv);	//call constructor for main GLUT setup;
		glutReshapeFunc(reshape);		//feed in the callbacks;
		glutDisplayFunc(display);
		glutPassiveMotionFunc(mouseMove);
		glutKeyboardFunc(keyboard);
		glutKeyboardUpFunc(keyup);
		mygame->gameMain();				//Bam, into openGL
		/* If we're here, GL is done */
		
		delete mygame;					//cleanup
	
	return 0;

}
