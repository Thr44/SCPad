//
//  SettingsPanel.mm
//  SCPad
//
//  Created by machine on 12/1/10.
//  Copyright 2010 .. All rights reserved.
//

#import "SettingsPanel.h"
#import "NodeList.h"
#import "OSC.h"

SettingsPanel::SettingsPanel():G(){
	
}
string SettingsPanel::toString(){
	return "";
}
string SettingsPanel::type(){
	return "SettingsPanel";
}

void SettingsPanel::setup(){
	errorMsg="";
	
	button=new G_ImageButton();
	button->setImage("settingsButton.png");
	button->setPosition(stageWidth-40, 10);
	this->addChild(button);
	button->setup();
	//
	button->onClick+= Delegate<SettingsPanel, int>(this, &SettingsPanel::onClickSettingsButton);
	
	//panel:
	isOpened=false;
	
	serverHostField=new G_InputField();
	addChild(serverHostField);
	serverHostField->setup();
	serverHostField->setLabel("Server Host:");
	serverHostField->setPlaceHolderLabel("127.0.0.1");
	serverHostField->setPosition(stageWidth-240, 45);//this->x+50, this->y+50);
	serverHostField->setSize(180, 29);
	serverHostField->setVisible(false);
	//
	serverPortField=new G_InputField();
	addChild(serverPortField);
	serverPortField->setup();
	serverPortField->setLabel("Server Port:");
	serverPortField->setPlaceHolderLabel("57110");
	serverPortField->setPosition(stageWidth-240, this->y+80);
	serverPortField->setSize(180, 29);
	serverPortField->setVisible(false);
	//
	langHostField=new G_InputField();
	addChild(langHostField);
	langHostField->setup();
	langHostField->setLabel("Lang Host:");
	langHostField->setPlaceHolderLabel("127.0.0.1");
	langHostField->setPosition(stageWidth-240, this->y+110);
	langHostField->setSize(180, 29);
	langHostField->setVisible(false);
	//
	langPortField=new G_InputField();
	addChild(langPortField);
	langPortField->setup();
	langPortField->setLabel("Lang Port:");
	langPortField->setPlaceHolderLabel("57120");
	langPortField->setPosition(stageWidth-240, this->y+140);
	langPortField->setSize(180, 20);
	langPortField->setVisible(false);
	//
	connectButton=new G_ImageButton();
	addChild(connectButton);
	connectButton->setup();
	connectButton->setVisible(false);
	connectButton->setImage("okButton.png");
	connectButton->setPosition(stageWidth-85, 190);
	connectButton->onClick+= Delegate<SettingsPanel, int>(this, &SettingsPanel::onClickConnectButton);
	
}
void SettingsPanel::update(){
	button->setPosition(stageWidth-40, 10);
}

void SettingsPanel::draw(){
	if(isOpened){
		serverHostField->setPosition(stageWidth-240, 45);//this->x+50, this->y+50);
		serverHostField->setVisible(true);
		serverPortField->setPosition(stageWidth-240, this->y+80);//this->x+50, this->y+50);
		serverPortField->setVisible(true);
		langHostField->setPosition(stageWidth-240, this->y+110);//this->x+50, this->y+50);
		langHostField->setVisible(true);
		langPortField->setPosition(stageWidth-240, this->y+140);//this->x+50, this->y+50);
		langPortField->setVisible(true);
		
		
		ofSetColor(100, 100, 100, 200);
		ofFill();
		ofBeginShape();
		ofEnableAlphaBlending();
		GeometryUtils::drawRoundRect(stageWidth-250, y+10, 200, 240, 10);
		ofEndShape(true); 
		ofDisableAlphaBlending();
		ofSetColor(0, 255, 0);
		font.drawString("SuperCollider Settings", stageWidth-240, y+30);
		connectButton->setPosition(stageWidth-85, 190);
		connectButton->setVisible(true);
		ofSetColor(255, 0, 0);
		font.drawString(errorMsg, stageWidth-240, y+210);
		
	}else{
		serverHostField->setVisible(false);
		serverPortField->setVisible(false);
		langHostField->setVisible(false);
		langPortField->setVisible(false);
		connectButton->setVisible(false);
	}
}

//when settingsButton is pressed:
void SettingsPanel::onClickSettingsButton(const void* pSender, int& response){
	isOpened=!isOpened;
}
void SettingsPanel::onClickConnectButton(const void* pSender, int& response){
	printf("\nclick function: %i \n", response);
	printf("\n1::: %s", serverHostField->getValue().data());
	printf("\n2::: %s", serverPortField->getValue().data());
	printf("\n3::: %s", langHostField->getValue().data());
	printf("\n4::: %s", langPortField->getValue().data());
	//isOpened=false;
	OSC* osc=OSC::getInstance();
	try {
		osc->setupSendOsc(langHostField->getValue(), ofToInt(langPortField->getValue()));
		//temp:
		osc->setupReceiveOsc(12345);
		//
		if (osc->isConnected) {
			errorMsg="connected!";
			ofxOscMessage m;
			m.setAddress("/global");
			m.addStringArg("connected to SCPad");
			osc->sendMessage(m);
		}else {
			errorMsg="failed!";
		}
	}
	catch (...) {
		errorMsg="failed!";
	}
	
	
}