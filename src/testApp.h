#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"
#include "G_Canvas.h"
#include "SettingsPanel.h"
#include "OSC.h"
#include "ObjectList.h"
#include <algorithm>
#include "G_ImageButton.h"

class testApp : public ofxiPhoneApp {
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void touchDown(ofTouchEventArgs &touch);
	void touchMoved(ofTouchEventArgs &touch);
	void touchUp(ofTouchEventArgs &touch);
	void touchDoubleTap(ofTouchEventArgs &touch);
	void touchCancelled(ofTouchEventArgs &touch);

	void lostFocus();
	void gotFocus();
	void gotMemoryWarning();
	void deviceOrientationChanged(int newOrientation);
    
	G_Canvas* canvas;
	int currentOrientation;
	
	SettingsPanel* settingsPanel;
	
	static bool sortOnTime(Object* a, Object*b);
	//static bool sortOnID(Node* a, Node*b);
	
	ObjectList* soundObjects;
	
	void onDataFunction(const void* pSender, ofxOscMessage & response);
	
protected:
	G_ImageButton* trademark;
    
};
