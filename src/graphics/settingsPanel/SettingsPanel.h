//
//  SettingsPanel.h
//  SCPad
//
//  Created by machine on 12/1/10.
//  Copyright 2010 .. All rights reserved.
//

#ifndef _SETTINGSPANEL
#define _SETTINGSPANEL

#pragma once
#include "ofMain.h"
#include "G.h"
#include "G_ImageButton.h"
#include "Globals.h"
#include "G_InputField.h"


class SettingsPanel : public G{
	
	public:
	SettingsPanel();
	//ThEvent* tempEvent;
	virtual string toString();
	virtual string type();
	virtual void setup();//ofEventArgs & args);
	virtual void update();
	virtual void draw();//ofEventArgs & args);
	string errorMsg;
	
	G_ImageButton* button;
	void onClickSettingsButton(const void* pSender, int& response);
	void onClickConnectButton(const void* pSender, int& response);
	
protected:
	bool isOpened;
	G_InputField * serverHostField;
	G_InputField * serverPortField;
	G_InputField * langHostField;
	G_InputField * langPortField;
	G_ImageButton* connectButton;
	
};


#endif