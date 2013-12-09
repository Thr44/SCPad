/*
 *  OSC.h
 *  SCPad
 *
 *  Created by machine on 9/11/10.
 *  Copyright 2010 .. All rights reserved.
 *
 */
#ifndef _COMMUNICATION
#define _COMMUNICATION

#pragma once
#include "ofMain.h"

class Communication{
public:
	static Communication* getInstance();
	~Communication();
protected:
	Communication(); //constructor
	private:
	static Communication* instance;
};

#endif