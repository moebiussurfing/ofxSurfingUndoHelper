#pragma once


/*

This example shows the workflow of undo engine.
We will randomize all the parameters pressing SPACE.
We will browse the randomizations history.

KEYS:
SPACE      : Randomize Parameters
BACKSPACE  : Reset parameters
ENTER      : Save Undo State

*/


#include "ofMain.h"

#include "ofxSurfingUndoHelper.h"

#include "surfingParamsRandom.h"
#include "ofxSurfingImGui.h"

class ofApp : public ofBaseApp
{
public:
	
    void setup();
    void draw();	
	void keyPressed(ofKeyEventArgs& eventArgs);
    void drawScene();

	// Parameters
	ofParameterGroup params;
	ofParameter<float> alpha;
	ofParameter<float> round;
	ofParameter<float> rotation;
	ofParameter<int> size;
	ofParameter<ofColor> colorShape;

	// Scene
	void drawShape(int x, int y, int size);
	
	void doChangeColor();
	vector<ofColor> colors;

	void doReset();
	void doRandom();

	//--

	// Undo
	ofxSurfingUndoHelper undoManager;

	//--

	ofxSurfing_ImGui_Manager guiManager;
	ofParameter<bool> bGui{ "ofApp",true };

	SurfingParamsRandom surfingParamsRandom;

};
