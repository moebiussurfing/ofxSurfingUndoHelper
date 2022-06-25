#pragma once


/*

	This example shows the workflow of the Undo Engine.
	
	HOW-TO

	1. Press SPACE to randomize all the parameters.
	2. Browse the random Undo History.

	KEYS:
	SPACE      : Randomize Parameters
	BACKSPACE  : Reset parameters
	ENTER      : Save Undo State

*/


#include "ofMain.h"


#define USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER // -> Declarations to help integrate into our apps/add-ons

#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER 
#include "ofxSurfingUndoHelper.h"
#endif

#include "ofxSurfingImGui.h"
#include "surfingParamsRandom.h"
#include "TextBoxWidget.h"

class ofApp : public ofBaseApp
{
public:
	
    void setup();
    void draw();	
	void keyPressed(ofKeyEventArgs& eventArgs);
    
	void setupGui();	
	void drawGui();	

    void setupScene();
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

	//--

	// Actions top modify the params easy
	void doReset();
	void doRandom();

	// Random helpers
	SurfingParamsRandom surfingParamsRandom;

	//--
	 
	// Help info
	TextBoxWidget textBoxWidget;

	//--

	// Gui
	ofxSurfing_ImGui_Manager guiManager;
	ofParameter<bool> bGui{ "ofApp",true };

	//--

	// Undo Engine
#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER 
	ofxSurfingUndoHelper undoManager;
#endif

};
