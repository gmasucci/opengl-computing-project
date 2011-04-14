//////////////////////////////////////////////
//Controls.h                                //
//The Control class is used to to implement	//
//the keyboard and mouse controls.			//
//If I had the time I would consider making //
//this a singleton.							//
//////////////////////////////////////////////

#ifndef CONTROLS_H
#define CONTROLS_H
#include "Toolbox.h"
#define POS_KEY			 43
#define NEG_KEY			 45
#define ESC_KEY			 27

class Controls
{
public:
	Controls();
	~Controls();
	inline void mouseMotion(int x, int y){mouse_x = x;mouse_y = y;};						// Called via the game class to the mouseMotion callback function
	void mouseClick(int button, int state, int x, int y);									// Called via the game class to the mouseClick callback function
	inline void keyboardPress(unsigned char key,int x,int y){keys[key] = true;};			// Called via the game class to the keyyboardPress callback function
	inline void keyboardRelease(unsigned char key,int x,int y){keys[key] = false;};			// Called via the game class to the keyboardUp callback function
	inline bool getKeyState(int key){return keys[key];};									//Used to check if a specific key is pressed or not, the keys ASCII value - 32 equals its position in the key array
	inline int getMouseX(){return mouse_x;};												// Returns the cursers x position
	inline int getMouseY(){return mouse_y;};												// Returns the cursers y position
	inline bool getMouseActive(){return mouse_active;};										// Returns the mouse left button condition
	inline bool getMouseRightActive(){return mouse_right_active;};							// Returns the mouse right button condition
private:
	bool		keys[255];																	// An array to store all the ASCII keys of the keyboard. The ASCII number of the key minus 32 equates to the position in the array
	bool		mouse_active;																// Stores the mouse left button state
	bool		mouse_right_active;															// Stores the mouse right button state
	int			mouse_x;																	// Stores the mouse x position
	int			mouse_y;																	// Stores the mouse y position
};
#endif                                                                                                                                                                                                                                                                                                                                                                                                                                                  