#include "testApp.h"
#include "GeometryUtils.h"
#include "Globals.h"
#include "ofxXmlSettings.h"
#include "SoundObject.h"
#include "ObjectList.h"

#include "G_CircularSelectSlider.h"

#include "Object.h"

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int Object::someNumber = 1;
string Object::sortType = "empty";

//--------------------------------------------------------------
void testApp::setup(){
    ofColor color;
    color.set(255, 34, 44);
    printf("color? %hhu %hhu %hhu %hhu", color.r, color.g, color.b, color.a);
    stageHeight=ofGetScreenHeight();
	stageWidth=ofGetScreenWidth();
	
	printf("running setup");

	// register touch events
	ofRegisterTouchEvents(this);
	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
	
	//If you want a landscape oreintation 
	//iPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);
	
	//config:
	ofBackground(127, 127, 127);
	font.loadFont("DomesSanNor.ttf", 12, true, true, true);
    font2.loadFont("DomesSanNor.ttf", 10, true, true, true);
	/*
     ThGlobals::setHeight(ofGetScreenHeight());
     ThGlobals::setWidth(ofGetScreenWidth());
     */
	
	
	//initCanvas:
	canvas=new G_Canvas();
	
	trademark=new G_ImageButton();
	trademark->setImage("lettering.png");
	canvas->addChild(trademark);
	
	
	settingsPanel=new SettingsPanel();
	printf("\ncanvas add:\n");
	canvas->addChild(settingsPanel);
	settingsPanel->setup();
	printf("\n------\\\\\\\\n");
	
	/*
     G_CircularSelectSlider* ob =new G_CircularSelectSlider();
     ob->setSize(100, 100);
     ob->setPosition(400, 400);
     ob->setRadius(40);
     ob->setNumElements(5);
     canvas->addNode(ob);
     */
	canvas->setVisible(true);
	soundObjects=new ObjectList();
	
	
	OSC* osc=OSC::getInstance();
	osc->onData+= Delegate<testApp, ofxOscMessage>(this, &testApp::onDataFunction);
	
	
}

bool testApp::sortOnTime(Object* a, Object*b){
	return ((SoundObject*)a)->time < ((SoundObject*)b)->time;
    
}
/*
 bool testApp::sortOnID(Node* a, Node*b){
 return a->id < b->id;
 
 }
 */
//--------------------------------------------------------------
void testApp::update(){
    int posX=20;
	int posY=20;
	int ctrlPosX=40;
	int ctrlPosY=(stageHeight*0.5) - 47;
	int maxButtonsInRow;
    int maxWidth;
	int moduleWidth;
	//std::vector<Object*> sv = soundObjects->objectVector;
	//sort(sv.begin(), sv.end());
	//FIXME: sort makes behavior wrong!!
	trademark->setPosition(2, stageHeight-18);
	
	if (soundObjects->length()>0) {
        
        
        ObjectList* clonedList=soundObjects->clone();
        clonedList->sort(testApp::sortOnTime);
        
        ctrlPosX=40;
		if (stageWidth>stageHeight) {
			ctrlPosY=stageHeight-360;
			maxButtonsInRow=10;
            maxWidth=1230;
			moduleWidth=123;
		}else {
			ctrlPosY=(stageHeight*0.5) + 30;
			maxButtonsInRow=7;
			moduleWidth=122;
            maxWidth=854;
		}
		
        
		
        for (int i=0; i<soundObjects->length(); i++) {
            //printf("time: %i", soundObjects[i]->time);
            SoundObject* so=(SoundObject*) soundObjects->getObjectAt(i);
            if(so->isVisible()){
                posY=floor((floor(i/maxButtonsInRow)*85)+20);
                if (i%maxButtonsInRow==0) {
                    posX=20;
                }
                so->setPosition(posX, posY);
                posX+=83;
            }
            
        }
        for (int i=0; i<clonedList->length(); i++) {
            SoundObject* so=(SoundObject*) clonedList->getObjectAt(i);
            if (so->isActive()) {
                so->controlAreaWidth = 0;
                for(int j=0; j<so->controlList->length(); j++){
                    G* control = (G*)so->controlList->getObjectAt(j);
                    int controlWidth=floor(control->width);
                    if(controlWidth>50){
                        so->controlAreaWidth+=moduleWidth;
                    }else{
                        so->controlAreaWidth+=floor(moduleWidth*0.5);
                    }
                    
                    
                }
                if ( (ctrlPosX + so->controlAreaWidth) > ( stageWidth + 40 )) {
                    ctrlPosX=40;
                    ctrlPosY+=120;
                }
                //vars to draw Rect ant title Text:
                so->controlAreaX=ctrlPosX;
                so->controlAreaY=ctrlPosY;
                so->controlAreaHeight=120;
                
                //printf("\n\niterating controlList to update...");
                for(int j=0; j<so->controlList->length(); j++){
                    G* control = (G*)so->controlList->getObjectAt(j);
					
                    if(control->alignment=="CENTER"){
                        control->setPosition(ctrlPosX+(control->width*0.5), ctrlPosY+(control->height*0.5));
                    }else {
                        control->setPosition(ctrlPosX, ctrlPosY);
                    }
                    //printf("\nposition of control %s is %f : %f", control->getLabel().data(), control->x, control->y);
                    int controlWidth=floor(control->width);
                    if(controlWidth>50){
                        ctrlPosX+=moduleWidth;
                    }else{
                       ctrlPosX+=floor(moduleWidth*0.5);
                    }
                    //ctrlPosX+=moduleWidth;
                }
                
                //printf("\nended iterating controlList...");
                
            }
        }
	}
	canvas->updateIfVisible();

}

//--------------------------------------------------------------
void testApp::draw(){
#ifdef IPHONE
	
	/*
	 ofPushMatrix();
	 switch (orientation) {
	 case 2:
	 ofTranslate(ofGetScreenWidth(), ofGetScreenHeight(), 0);
	 ofRotate(180, 0, 0, 1);
	 break;
	 case 3:
	 ofTranslate(ofGetScreenWidth(), 0, 0);
	 ofRotate(90, 0, 0, 1);
	 break;
	 case 4:
	 ofTranslate(0, ofGetScreenHeight(), 0);
	 ofRotate(-90, 0, 0, 1);
	 break;
	 default:
	 break;
	 }
	 canvas->drawIfVisible();
	 
	 ofPopMatrix();
	 */
#endif
	canvas->drawIfVisible();
}

void testApp::onDataFunction(const void* pSender, ofxOscMessage & response){
	string msg_address;
	msg_address = response.getAddress();
    //osc messages from SC:
	if(msg_address=="/soundObjects"){
		for ( int i=0; i<response.getNumArgs(); i++ )
		{
			if( response.getArgType( i ) == OFXOSC_TYPE_STRING ){
				ofxXmlSettings XML;
				string data=response.getArgAsString(i);
				XML.loadFromBuffer(data);
				printf("\nmessage: %s", data.data());
				
				XML.pushTag("xml", 0);
				for (int j=0; j<XML.getNumTags("soundObject"); j++) {
					string def="";
					string name = (string) XML.getAttribute("soundObject", "name", def, j);
					string action = (string) XML.getAttribute("soundObject", "action", def, j);
					string status = (string) XML.getAttribute("soundObject", "status", def, j);
					bool exists=false;
					int index;
					int l;
					
                    //adding new objects:
					if(action=="add"){
						SoundObject* so;
						for (l=0; l<soundObjects->length(); l++) {
                            //verifies if already exists:
							SoundObject* obj=(SoundObject*) soundObjects->getObjectAt(l);
							if (obj->getLabel()==name) {
                                //removes object:
                                canvas->removeChild(obj);
                                soundObjects->removeObject(obj);
                                
								//exists=true;
								//so=obj;
								//index=l;
								break;
							}
						}
                       
						//if(!exists){
							so=new SoundObject();
							soundObjects->addObject(so);
							canvas->addChild(so);
						//}
						so->setLabel(name);
                        //so->isPlaying = "true"?TRUE:FALSE;
						
						string cType = (string) XML.getAttribute("soundObject", "cType", def, j);
						so->setControlType(cType);
						
						if (cType=="trig" || 0==0) {
                            //TODO: fix this, not using cType
							//its just a trigger!
							
							//it may be something else!!
                            //}else {
							
							XML.pushTag("soundObject", j);
							XML.pushTag("controls", 0);
							for (int k=0; k<XML.getNumTags("control"); k++) {
								//G* control;
								
								string controlType = (string) XML.getAttribute("control", "type", def, k);
                                string controlName = controlType;//(string) XML.getAttribute("control", "name", def, k);
								vector<float> params;
								XML.pushTag("control", k);
                                
                                //change this:
                                vector<string> paramNames;
								for(int l=0; l<XML.getNumTags("param"); l++){
                                    string paramName=(string) XML.getAttribute("param", "name", def, l);
                                    printf("name: %s", paramName.data());
                                    paramNames.push_back(paramName);
                                    XML.pushTag("param", l);
                                    for(int m=0; m<XML.getNumTags("value"); m++){
                                        float value = ofToFloat(((string) XML.getValue("value", "", m)));
                                        params.push_back(value);
                                    }
                                    XML.popTag();
								}
								printf("ADDING:\n");
                                printf("CONTROL NAME: %s", controlName.data());
                                printf("PARAMS: %i", (int) params.size());
                                printf("PARAM NAMES: %i", (int) paramNames.size());
								so->addControl(controlName, controlType, params, paramNames);
                                
								XML.popTag();//</control>
								
							}
							XML.popTag(); //</params>
							XML.popTag(); // </soundObject>
						}
						
						//TODO:update sound element!
					}else {
                        //remove sound objects:
						for (l=0; l<soundObjects->length(); l++) {
							if (((SoundObject*) soundObjects->getObjectAt(l))->getLabel()==name) {
								exists=true;
								index=l;
								break;
							}
						}
						if(exists){
							//bool isRemoved = 
							SoundObject* obj = (SoundObject*)soundObjects->getObjectAt(index);
							canvas->removeChild(obj);
							soundObjects->deleteObjectAt(index);
						}
						
					}
				}
				XML.popTag(); //</xml>
			}
		}
	}else{
		printf("failed parsing!");
	}
}


//--------------------------------------------------------------
void testApp::exit(){
    printf("exit()\n");
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){
	canvas->dispatchTouchDown(touch);
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){
	canvas->dispatchTouchMoved(touch);
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){
	canvas->dispatchTouchUp(touch);
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){
	canvas->dispatchTouchDoubleTap(touch);
}

//--------------------------------------------------------------
void testApp::lostFocus() {
}

//--------------------------------------------------------------
void testApp::gotFocus() {
}

//--------------------------------------------------------------
void testApp::gotMemoryWarning() {
}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){
	if (newOrientation==5) {
		return;
	}
	 orientation=newOrientation;
     printf("\norientation %i %i\n", ofGetScreenWidth(), ofGetScreenHeight());
	 if (orientation==3 || orientation==4) {
         stageHeight=ofGetScreenWidth();
         stageWidth=ofGetScreenHeight();
	 }else {
         stageWidth=ofGetScreenWidth();
         stageHeight=ofGetScreenHeight();
	 }
	 
	printf("curr: %i", newOrientation);
	
    
	//orientation=newOrientation;
	switch (orientation) {
		case 2:
			ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_UPSIDEDOWN);
			break;
		case 3:
            ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT);
			break;
		case 4:
			ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);
			break;
		case 1:
			ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_PORTRAIT);
			break;
	}
	//stageHeight=ofGetScreenHeight();
	//stageWidth=ofGetScreenWidth();
	
	canvas->dispatchOrientationChanged(newOrientation);
    
}


//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs& args){

}

