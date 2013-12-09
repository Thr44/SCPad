/*
 *  SoundObject.h
 *  SCPad
 *
 *  Created by machine on 2/7/11.
 *  Copyright 2011 .. All rights reserved.
 *
 */

#ifndef _SOUNDOBJECT
#define _SOUNDOBJECT

#pragma once
#include "G.h"
#include "ObjectList.h"
#include "G_Button.h"

#include "Poco/Stopwatch.h"
#include "Poco/Thread.h"
#include "Poco/Timestamp.h"
#include "Poco/Timer.h"

using Poco::Stopwatch;
using Poco::Thread;
using Poco::Timer;
using Poco::TimerCallback;

class SoundObject:public G{
public:
	~SoundObject();
	SoundObject(); //constructor
	virtual void setup();
	virtual void update();
	virtual void draw();
	void setLabel(string label);
	string getLabel();
	void setControlType(string controlType);
	virtual string toString();
	virtual string type();
	G_Button* button;
	void onClickFunction(const void* pSender, int& response);
	void onReleaseButtonFunction(const void* pSender, int& response);
	//void onChangeFunction(const void* pSender, int& response);
	
	//EVENTS:
    void onSliderFunction(const void* pSender, float& response);
    void onCircularFunction(const void* pSender, float& response);
	void onCircularSelectFunction(const void* pSender, int*& response);
	void onCircularRangeFunction(const void* pSender, float& response);
	void onScatterXYFunction(const void* pSender, ofVec3f& response);
	void onVectorDataFunction(const void* pSender, ofVec2f& response);
	void onScatterIntFunction(const void* pSender, int& response);
	void onIntFunction(const void* pSender, int& response);
    void onIntPlayFunction(const void* pSender, int& response);
    void onIntLockPlayFunction(const void* pSender, int& response);
	//
	
	void addControl(string label, string type, vector<float> params, vector<string> paramNames);
	//void removeControl(string label);
	void removeControl(int index);
	bool isActive();
	void setActive(bool value);
	ObjectList* controlList;
	float time;
	
	//elements to draw controlArea
	float controlAreaWidth;
	float controlAreaHeight;
	float controlAreaX;
	float controlAreaY;
    bool isPlaying;
	
	bool operator<( const SoundObject & other ) { 
		printf("time: %f", time);
		if ( time < other.time ){
			return true; 
			
		}else{ 
			return false; 
		}
	};
	
protected:
	string label;
	string controlType;
	bool active;
	float controlSize;
	float controlRadius;
	
	Timer* timer;
	void onTimer(Timer& timer);
	
};

#endif