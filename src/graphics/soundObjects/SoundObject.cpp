/*
 *  SoundObject.cpp
 *  SCPad
 *
 *  Created by machine on 2/7/11.
 *  Copyright 2011 .. All rights reserved.
 *
 */

#include "SoundObject.h"
#include "G_SelectButton.h"
#include "G_PlayButton.h"
#include "G_Slider.h"
#include "G_CircularSlider.h"
#include "G_CircularRangeSlider.h"
#include "G_CircularSelectSlider.h"
#include "G_ScatterXY.h"
#include "G_ScatterRadial.h"
#include "OSC.h"
#include "GeometryUtils.h"


SoundObject::SoundObject(){
	button=NULL;
	controlList=new ObjectList();
	//define size of control interfaces:
	controlSize=80;
	controlRadius=40;
	active=false;
	time=0;
    this->isPlaying=FALSE;
    //printf("\nSTARTUP isPlaying? %i\n", this->isPlaying==TRUE?1:0);
	//timer=NULL;
	//timer=new Timer(0, 400);
}
SoundObject::~SoundObject(){
	for(int i=0; i<controlList->length(); i++){
		this->removeChild((G*) controlList->getObjectAt(i));
		this->controlList->removeObjectAt(i);
	}
}

void SoundObject::setup(){
	
}
void SoundObject::update(){
	if(button!=NULL){
		button->setPosition(x, y);
		if (button->isDown) {
			float elapsedTime = ofGetSystemTime()-this->time;
			printf("\nISDOWN: %f\n", elapsedTime);
			if(!isActive() && elapsedTime > 400.0){
				setActive(true);
				//m.addStringArg("status");
				//m.addIntArg(isActive()?1:0);
				//display controls:
				for (int i=0; i<controlList->length(); i++) {
					((G*) controlList->getObjectAt(i))->setVisible( true );
					//printf("\nclick function: %i %s %s\n", response, this->getLabel().data(), ((G*) controlList->getObjectAt(i))->isVisible()?"true":"false");
					
				}
				
			}
		}else {
			//printf("IS NOT DOWN");
		}

	}
}
void SoundObject::draw(){
	if(isActive()){
		//ofEnableAlphaBlending();
		if(controlList->length()>0){
			ofSetColor(100, 100, 100, 0.5);
			ofFill();
			ofBeginShape();
			GeometryUtils::drawRoundRect(controlAreaX-20, controlAreaY-20, controlAreaWidth-4, controlAreaHeight-4, 5);
			ofEndShape(true);
			//ofDisableAlphaBlending();
			
			ofSetColor(255, 255, 255, 255);
            ofPushMatrix();
            ofTranslate( controlAreaX-15, controlAreaY-12);
            ofRotate(90, 0, 0, 1);
			font.drawString(this->label, 0, 0);
            ofPopMatrix();
            
		}
		
	}
}

void SoundObject::setLabel(string label){
	this->label=label;
}
string SoundObject::getLabel(){
	return this->label;
}
void SoundObject::setControlType(string controlType){
	this->controlType=controlType;
	if (button!=NULL) {
		this->removeChild(this->button);
		//this->button->onClick-= Delegate<SoundObject, int>(this, &SoundObject::onClickFunction);
		delete this->button;
		this->button=NULL;
	}
	if(controlType=="trig"){
		this->button=new G_Button();
	}else {
		this->button=new G_SelectButton();
	}
	
	
	this->button->setFont(&font2);
	this->button->setSize(60, 60);
	this->button->setCornerRadius(5);
	this->button->setLabel(this->label);
	
	this->button->onClick+= Delegate<SoundObject, int>(this, &SoundObject::onClickFunction);
	if(controlType=="trig"){
		this->button->onRelease+= Delegate<SoundObject, int>(this, &SoundObject::onReleaseButtonFunction);
	}
	this->addChild(button);
	
	
}
string SoundObject::toString(){
	//string value ""
	return "";
}

string SoundObject::type(){
	return "SoundObject";
}
bool SoundObject::isActive(){
	return active;
}
void SoundObject::setActive(bool value){
	active=value;
}
void SoundObject::onReleaseButtonFunction(const void* pSender, int& response){
	//timer->stop();
	printf("\n timerStop CLOSES on release!\n");
	setActive(false);
	
	for (int i=0; i<controlList->length(); i++) {
		((G*) controlList->getObjectAt(i))->setVisible( false);
		//printf("\nclick function: %i %s %s\n", response, this->getLabel().data(), ((G*) controlList->getObjectAt(i))->isVisible()?"true":"false");
		
	}
	/*
	if(this->timer!=NULL){
		delete this->timer;
		this->timer=NULL;
	}
	 */

}
void SoundObject::onTimer(Timer& timer){
	
	//should delay before displaying:
	//
	setActive(true);
	//m.addStringArg("status");
	//m.addIntArg(isActive()?1:0);
	//display controls:
	for (int i=0; i<controlList->length(); i++) {
		((G*) controlList->getObjectAt(i))->setVisible( true );
		//printf("\nclick function: %i %s %s\n", response, this->getLabel().data(), ((G*) controlList->getObjectAt(i))->isVisible()?"true":"false");
		
	}
	//this->timer->stop();
	printf("\n timerStop OPENS on timer!\n");
	/*if(this->timer!=NULL){
		delete this->timer;
		this->timer=NULL;
	}
	 */
}	
void SoundObject::onClickFunction(const void* pSender, int& response){
	time=ofGetSystemTime();
	cout << time;
	//send to SC soundObject activation
	//OSC* osc=OSC::getInstance();
	//ofxOscMessage m;
	//string address = "/"+this->getLabel();
	//m.setAddress(address.data());
	
	
	if(this->controlType=="trig"){
        OSC* osc=OSC::getInstance();
        ofxOscMessage m;
        string address = "/scPad";
        printf("send: '%s'", address.data());
        m.setAddress(address.data());
        //thrName:
        m.addStringArg(this->getLabel());
        //
        m.addStringArg("play");
        m.addIntArg(1);
        osc->sendMessage(m);
        
        
        
		//m.addIntArg(1);
		//timer=new Timer(0, 400);
		//timer->start(TimerCallback<SoundObject>(*this, &SoundObject::onTimer));
		
		printf("a TRIG start timer");
		/*
		setActive(true);
		//m.addStringArg("status");
		//m.addIntArg(isActive()?1:0);
		//display controls:
		for (int i=0; i<controlList->length(); i++) {
			((G*) controlList->getObjectAt(i))->setVisible( true );
			//printf("\nclick function: %i %s %s\n", response, this->getLabel().data(), ((G*) controlList->getObjectAt(i))->isVisible()?"true":"false");
			
		}
		//this->timer->stop();
		printf("\n timerStop OPENS on timer!\n");
		 */
		
	}else {	
		setActive(((G_SelectButton*)this->button)->isActive());
		//m.addStringArg("status");
		//m.addIntArg(isActive()?1:0);
        if(!isActive()){
            OSC* osc=OSC::getInstance();
            ofxOscMessage m;
            string address = "/scPad";
            printf("send: '%s'", address.data());
            m.setAddress(address.data());
            //thrName:
            m.addStringArg(this->getLabel());
            //
            m.addStringArg("play");
            m.addIntArg(0);//isActive()?1:0);
            osc->sendMessage(m);
            
            for (int i=0; i<controlList->length(); i++) {
                G* obj = ((G*) controlList->getObjectAt(i));
                if(obj->type()=="G_PlayButton"){
                    ((G_PlayButton*) obj)->active=false;
                }
                   //printf("\nclick function: %i %s %s\n", response, this->getLabel().data(), ((G*) controlList->getObjectAt(i))->isVisible()?"true":"false");
                   
            }
            
        }
		//display controls:
		for (int i=0; i<controlList->length(); i++) {
            G* obj = ((G*) controlList->getObjectAt(i));
			obj->setVisible( isActive() );
			//printf("\nclick function: %i %s %s\n", response, this->getLabel().data(), ((G*) controlList->getObjectAt(i))->isVisible()?"true":"false");
			
		}
		
	}
	//osc->sendMessage(m);
}



void SoundObject::addControl(string label, string type, vector<float> params, vector<string> paramNames){
	bool exists=false;
	
	//printf("controlList has %i controls", controlList->length());
	
    for (int i=0; i<controlList->length(); i++) {
		G* ctrl = (G*)(controlList->getObjectAt(i));
				   
		if (ctrl->getLabel()==label) {
			exists=true;
			break;
            controlList->removeObjectAt(i);
			//TODO: update control!
		}
	}
     /*
	if (!exists) {
     */
		if (type=="Play"){
            G_PlayButton* control;
            control = new G_PlayButton();
            control->active=(this->isPlaying);
            //control->setPosition(20, 100);
            control->setLabel(label);
            ((G_PlayButton*) control)->setSize(controlSize * 0.3, controlSize);
            control->setControlNames(paramNames);
            this->addChild(control);
            control->setup();
            control->setVisible(false);
            //params should be mapped to control param name
            control->onClick+= Delegate<SoundObject, int>(this, &SoundObject::onIntLockPlayFunction);
            controlList->addObject(control);
		}	
        else if (type=="Slider"){
            G_Slider* control;
            control = new G_Slider();
            //control->setPosition(20, 100);
            control->setLabel(label);
            ((G_Slider*) control)->setSize(controlSize * 0.3, controlSize);
            control->setControlNames(paramNames);
            this->addChild(control);
            control->setup();
            control->setVisible(false);
            ((G_Slider*) control)->setScrollPosition(params[0]);
            //params should be mapped to control param name:
            control->onDrag+= Delegate<SoundObject, float>(this, &SoundObject::onSliderFunction);
            controlList->addObject(control);
		}	
        else if (type=="circularRange"){
				G_CircularRangeSlider* control;
				control = new G_CircularRangeSlider();
				//control->setPosition(20, 100);
				control->setLabel(label);
				((G_CircularRangeSlider*) control)->setRadius(controlRadius);
            for(int i=0; i<paramNames.size(); i++){
                printf("%i ____>>>> %s", i, paramNames[i].data());
            }
                control->setControlNames(paramNames);
				this->addChild(control);
				control->setup();
				control->setVisible(false);
            printf("RANGE OF SLIDER: %f %f", params[0], params[1]);
				((G_CircularRangeSlider*) control)->setRange(params[0], params[1]);
                //params should be mapped to control param name:
				control->onDrag+= Delegate<SoundObject, float>(this, &SoundObject::onCircularRangeFunction);
				controlList->addObject(control);
		}
		else if(type=="circularSlider"){
				G_CircularSlider* control;
            control = new G_CircularSlider();
            //control->setPosition(20, 100);
            control->setLabel(label);
            ((G_CircularSlider*) control)->setRadius(controlRadius);
            control->setControlNames(paramNames);
            this->addChild(control);
            control->setup();
            control->setVisible(false);
            ((G_CircularSlider*) control)->setScrollPosition(params[0]);
            control->onDrag+= Delegate<SoundObject, float>(this, &SoundObject::onCircularFunction);
			controlList->addObject(control);
		}
		else if(type=="circularSelectSlider"){
			G_CircularSelectSlider* control;
			control = new G_CircularSelectSlider();
			//control->setPosition(20, 100);
			control->setLabel(label);
			((G_CircularSelectSlider*) control)->setRadius(controlRadius);
            control->setControlNames(paramNames);
			this->addChild(control);
			control->setup();
			control->setVisible(false);    
			((G_CircularSelectSlider*) control)->setNumElements(params[0]);
			control->onPress+= Delegate<SoundObject, int*>(this, &SoundObject::onCircularSelectFunction);
			controlList->addObject(control);
		}
		else if(type=="scatterXY"){
            G_ScatterXY* control;
            control = new G_ScatterXY();
            //control->setPosition(20, 100);
            control->setLabel(label);
            ((G_ScatterXY*) control)->setSize(controlSize, controlSize);
            control->setControlNames(paramNames);
            //((G_ScatterXY*) control)->setRange(params[0], params[1]); //Should use this!
            this->addChild(control);
            control->setup();
            control->setVisible(false);
            //((G_ScatterXY*) control)->setRange(params[0], params[1]);
            ((G_ScatterXY*) control)->onDrag+= Delegate<SoundObject, ofVec3f>(this, &SoundObject::onScatterXYFunction);
            if(paramNames.size()>2){
                if(paramNames[2]=="play"){
                    ((G_ScatterXY*) control)->onPress+= Delegate<SoundObject, int>(this, &SoundObject::onIntPlayFunction);
                    ((G_ScatterXY*) control)->onRelease+= Delegate<SoundObject, int>(this, &SoundObject::onIntPlayFunction);
                }else{
                    ((G_ScatterXY*) control)->onPress+= Delegate<SoundObject, int>(this, &SoundObject::onScatterIntFunction);
                    ((G_ScatterXY*) control)->onRelease+= Delegate<SoundObject, int>(this, &SoundObject::onScatterIntFunction);
                }
            }
            
            /*
            
            if(paramNames.size()>2){
                if(paramNames[2]=="play"){
                    ((G_ScatterXY*) control)->onClick+= Delegate<SoundObject, int>(this, &SoundObject::onIntPlayFunction);
                }else{
                    ((G_ScatterXY*) control)->onClick+= Delegate<SoundObject, int>(this, &SoundObject::onScatterIntFunction);
                }
            }
            */
			controlList->addObject(control);
		}
		else if(type=="scatterRadial"){
            G_ScatterRadial* control;
            control = new G_ScatterRadial();
            //control->setPosition(20, 100);
            control->setLabel(label);
            ((G_ScatterRadial*) control)->setRadius(controlRadius);
            control->setControlNames(paramNames);
            this->addChild(control);
            control->setup();
            control->setVisible(false);
            //((G_ScatterXY*) control)->setRange(params[0], params[1]);
            ((G_ScatterRadial*) control)->onDrag+= Delegate<SoundObject, ofVec2f>(this, &SoundObject::onVectorDataFunction);
            if(paramNames.size()>2){
                if(paramNames[2]=="play"){
                    ((G_ScatterRadial*) control)->onPress+= Delegate<SoundObject, int>(this, &SoundObject::onIntPlayFunction);
                    ((G_ScatterRadial*) control)->onRelease+= Delegate<SoundObject, int>(this, &SoundObject::onIntPlayFunction);
                }else{
                    ((G_ScatterRadial*) control)->onPress+= Delegate<SoundObject, int>(this, &SoundObject::onScatterIntFunction);
                    ((G_ScatterRadial*) control)->onRelease+= Delegate<SoundObject, int>(this, &SoundObject::onScatterIntFunction);
                }
            }
				//((G_ScatterRadial*) control)->onClick+= Delegate<SoundObject, int>(this, &SoundObject::onIntFunction);
                //((G_ScatterRadial*) control)->onRelease+= Delegate<SoundObject, int>(this, &SoundObject::onIntFunction);
            controlList->addObject(control);
		}
	//}
	
}

void SoundObject::removeControl(int index){
	controlList->removeObjectAt(index);
}



//LISTENER FUNCTIONS:
void SoundObject::onSliderFunction(const void* pSender, float& response){
	//OSC MESSAGE: /address, /thrName, /paramName0, /paramValue0, ... , /paramNameN, paramValueN
    OSC* osc=OSC::getInstance();
	ofxOscMessage m;
	string address = "/scPad";
	//printf("send: '%s'", address.data());
	m.setAddress(address.data());
    //thrName:
    m.addStringArg(this->getLabel());
    
    G_Slider* slider = (G_Slider*) pSender;
    string params="{\n'";
    params+= slider->controlNames[0];
    params += ofToString("':");// [");
    params += ofToString(slider->getScrollPosition());

    params += ofToString("\n}");//"]\n}");
    
    m.addStringArg(params);
    
    //
	//m.addStringArg(((G*) pSender)->getLabel().data());
	//float p = ((G_CircularSlider*) pSender)->getSliderPosition();
	//m.addFloatArg(p);
	osc->sendMessage(m);
   
}

void SoundObject::onCircularFunction(const void* pSender, float& response){
	//OSC MESSAGE: /address, /thrName, /paramName0, /paramValue0, ... , /paramNameN, paramValueN
    OSC* osc=OSC::getInstance();
	ofxOscMessage m;
	string address = "/scPad";
	//printf("send: '%s'", address.data());
	m.setAddress(address.data());
    //thrName:
    m.addStringArg(this->getLabel());
    
    G_CircularSlider* circularSlider = (G_CircularSlider*) pSender;
    string params="{\n'";
    params+= circularSlider->controlNames[0];
    params += ofToString("': ");//[");
    params += ofToString(((G_CircularSlider*) pSender)->getScrollPosition());
    params += ofToString("\n}");//("]\n}");
    
    m.addStringArg(params);

	osc->sendMessage(m);
   
}

void SoundObject::onCircularSelectFunction(const void* pSender, int*& response){
	OSC* osc=OSC::getInstance();
	ofxOscMessage m;
	string address = "/scPad";//"+this->getLabel();
	printf("send: '%s'", address.data());
	m.setAddress(address.data());
    
    G* gObj = (G*) pSender;
    m.addStringArg(this->getLabel());//gObj->getLabel().data());
    
    
    string params="{\n'";
    params+= gObj->controlNames[0];
    params += ofToString("':");
    
     //returns an array of weights for elements selection!
    params+= ofToString("[");// = ((G_CircularSelectSlider*) pSender)->getActiveElements();
	int i;
	for(i=(((G_CircularSelectSlider*) pSender)->numElements)-1; i>=1; i--){
		params+=ofToString(response[i])+",";
	}
	params+=ofToString(response[0])+"]";
    //
   
    params += ofToString("\n}");
    
    m.addStringArg(params);
    osc->sendMessage(m);
}

void SoundObject::onCircularRangeFunction(const void* pSender, float& response){
	OSC* osc=OSC::getInstance();
	ofxOscMessage m;
	string address = "/scPad";
	printf("send: '%s'", address.data());
	m.setAddress(address.data());
    m.addStringArg(this->getLabel());
	//m.addStringArg(((G*) pSender)->getLabel().data());
	
	/*m.addFloatArg(p->x);
	m.addFloatArg(p->y);
     */
    G* gObj = (G*) pSender;
     G_CircularRangeSlider* circularRangeSlider = (G_CircularRangeSlider*) pSender;
    
    
    
    ofVec2f* p = circularRangeSlider->getRange();
    string params="{\n'";
    if (gObj->controlNames.size()>1) {
        
        params+=gObj->controlNames[0];
        params += ofToString("': ");
        params += ofToString(p->x);
        params += ofToString(",\n'");
        params+= gObj->controlNames[1];
        params += ofToString("': ");
        params += ofToString(p->y);
        params += ofToString("\n}");
    } else {
        //string params="{\n'";
        params+=gObj->controlNames[0];
        params += ofToString("': [");
        params += ofToString(p->x);
        params +=ofToString(", ");
        params += ofToString(p->y);
        params += ofToString("]\n}");
    }
        
    printf("SENDING: %s", params.data());
    m.addStringArg(params);
    
	osc->sendMessage(m);
}

void SoundObject::onScatterXYFunction(const void* pSender, ofVec3f& response){
    OSC* osc=OSC::getInstance();
	ofxOscMessage m;
	string address = "/scPad";
	printf("send: '%s'", address.data());
	m.setAddress(address.data());
    //thrName:
    m.addStringArg(this->getLabel());
    //G_ScatterXY* scatterXY = (G_ScatterXY*) pSender;
    //
    G* gObj = (G*) pSender;

    
    string params="{\n'";
    if (gObj->controlNames.size()>2) {
        
        params+=gObj->controlNames[0];
        params += ofToString("': ");
        params += ofToString(response.x);
        params += ofToString(",\n'");
        params+= gObj->controlNames[1];
        params += ofToString("': ");
        params += ofToString(response.y);
        params += ofToString("\n}");
        params+= gObj->controlNames[2];
        params += ofToString("': ");
        params += ofToString(response.z);
        params += ofToString("\n}");
        
    } else if (gObj->controlNames.size()>1) {
        
        params+=gObj->controlNames[0];
        params += ofToString("': ");
        params += ofToString(response.x);
        params += ofToString(",\n'");
        params+= gObj->controlNames[1];
        params += ofToString("': ");
        params += ofToString(response.y);
        params += ofToString("\n}");
    } else {
        //string params="{\n'";
        params+=gObj->controlNames[0];
        params += ofToString("': [");
        params += ofToString(response.x);
        params +=ofToString(", ");
        params += ofToString(response.y);
        params += ofToString("]\n}");
    }

    /*
    string params="{\n'";
    params+= scatterXY->controlNames[0];
    params += ofToString("': [");
    params += ofToString(response.x);
    params += ofToString("],\n'");
    params+= scatterXY->controlNames[1];
    params += ofToString("': [");
    params += ofToString(response.y);
    params += ofToString("]\n}");
     */
     
     
    m.addStringArg(params);

	osc->sendMessage(m);
}

void SoundObject::onVectorDataFunction(const void* pSender, ofVec2f& response){
	OSC* osc=OSC::getInstance();
	ofxOscMessage m;
	string address = "/scPad";//+this->getLabel();
	printf("send: '%s'", address.data());
	m.setAddress(address.data());
    G* gObj = (G*) pSender;
	m.addStringArg(this->getLabel());//gObj->getLabel().data());
    
    
    
    string params="{\n'";
    params+= gObj->controlNames[0];
    params += ofToString("': ");
    params += ofToString( (response.angleRad(ofVec2f(0, -1))+PI)/(2.0*PI)); //returns a value between 0 and 1!
    params += ofToString(",\n'");
    params+= gObj->controlNames[1];
    params += ofToString("': ");
    params += ofToString(response.length());
    params += ofToString("\n}");
    
    m.addStringArg(params);
	//m.addFloatArg(response.angleRad(ofVec2f(0, -1)));
	//m.addFloatArg(response.x);
	//m.addFloatArg(response.length());
	osc->sendMessage(m);
}
void SoundObject::onIntFunction(const void* pSender, int& response){
    
	OSC* osc=OSC::getInstance();
	ofxOscMessage m;
	string address = "/scPad";//+this->getLabel();
	printf("send: '%s'", address.data());
	m.setAddress(address.data());
    
    G* gObj = (G*) pSender;
    m.addStringArg(this->getLabel());
	//m.addStringArg(((G*) pSender)->getLabel().data());
	//m.addIntArg(response);
    
    string params="{\n'";
    params+= gObj->controlNames[0];
    params += ofToString("': ");
    params += ofToString( response ); //returns a value between 0 and 1!
    /*params += ofToString("],\n'");
    params+= gObj->controlNames[1];
    params += ofToString("': [");
    params += ofToString(response.length());
     */
    params += ofToString("\n}");

    m.addStringArg(params);

    
	osc->sendMessage(m);
}
void SoundObject::onScatterIntFunction(const void* pSender, int& response){
    printf("\n@onScatterIntFunction response: %i", response);
	OSC* osc=OSC::getInstance();
	ofxOscMessage m;
	string address = "/scPad";//+this->getLabel();
	printf("send: '%s'", address.data());
	m.setAddress(address.data());
    
    G* gObj = (G*) pSender;
    m.addStringArg(this->getLabel());
	//m.addStringArg(((G*) pSender)->getLabel().data());
	//m.addIntArg(response);
    
    string params="{\n'";
    params+= gObj->controlNames[2];
    params += ofToString("': ");
    params += ofToString( response ); //returns a value between 0 and 1!
    params += ofToString("\n}");
    
    printf("SENDING DATA: %s", params.data());
    m.addStringArg(params);
    
    
	osc->sendMessage(m);

}
void SoundObject::onIntLockPlayFunction(const void* pSender, int& response){
    this->isPlaying=(response==1)?TRUE:FALSE;
    OSC* osc=OSC::getInstance();
	ofxOscMessage m;
	string address = "/scPad";
	printf("send: '%s'", address.data());
	m.setAddress(address.data());
    //thrName:
    m.addStringArg(this->getLabel());
    //
    m.addStringArg("play");
	m.addIntArg(response);
	osc->sendMessage(m);
    
    /*
     OSC* osc=OSC::getInstance();
     ofxOscMessage m;
     string address = "/"+this->getLabel();
     printf("send: '%s'", address.data());
     m.setAddress(address.data());
     m.addStringArg("startup");
     m.addIntArg(response);
     osc->sendMessage(m);
     */
}
void SoundObject::onIntPlayFunction(const void* pSender, int& response){
    printf("\n@onIntPlayFunction response:%i is Playing:%i", response, this->isPlaying==TRUE?1:0);
    if(this->isPlaying==FALSE){
        OSC* osc=OSC::getInstance();
        ofxOscMessage m;
        string address = "/scPad";
        printf("\n@onIntPlayFunction sendind '%s': %i", address.data(), response);
        m.setAddress(address.data());
        //thrName:
        m.addStringArg(this->getLabel());
        //
        m.addStringArg("play");
        m.addIntArg(response);
        osc->sendMessage(m);
    }

    /*
	OSC* osc=OSC::getInstance();
	ofxOscMessage m;
	string address = "/"+this->getLabel();
	printf("send: '%s'", address.data());
	m.setAddress(address.data());
	m.addStringArg("startup");
	m.addIntArg(response);
	osc->sendMessage(m);
     */
}

//