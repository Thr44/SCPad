/*
 *  Communication.cpp
 *  SCPad
 *
 *  Created by machine on 2/7/11.
 *  Copyright 2011 .. All rights reserved.
 *
 */

#include "Communication.h"

Communication::Communication(){
	
}
Communication* Communication::instance = NULL;

Communication* Communication::getInstance(){
	if(instance == NULL){
		instance=new Communication();
	}
	return instance;
}