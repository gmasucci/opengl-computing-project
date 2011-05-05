// SGLVSync.h: Schnittstelle f�r die Klasse SGLVSync.
//
// Copyright by Andr� Stein
// E-Mail: stonemaster@steinsoft.net, andre_stein@web.de
// http://www.steinsoft.net
//////////////////////////////////////////////////////////////////////
#include "Toolbox.h"
#ifndef SGLVSYNC_H
#define SGLVSYNC_H

class SGLVSync  
{
public:
	SGLVSync();
	virtual ~SGLVSync();

	void disable();
	void enable();
	bool init();
	
	bool isDisabled();
	bool isEnabled();
protected:
	bool isOK;
};

#endif 