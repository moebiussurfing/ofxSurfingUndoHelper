#pragma once


/*

	This example shows the workflow of the Undo Engine.

	HOWTO:
	
	1. Move the widgets to modify the params.
	2. Press "Add" button to store current state of all params to the Undo History.
	3. Browse all your added states to the history pressing Undo / Redo buttons.
	4. Notice that on Mode Files, history will persistent and recalled between app sessions.

*/


#include "ofMain.h"

#include "ofxSurfingUndoHelper.h"
#include "ofxSurfingImGui.h"

class ofApp : public ofBaseApp
{
public:
	
    void setup();
    void draw();	
	void keyPressed(ofKeyEventArgs& eventArgs);
    
	void setupParams();
    void drawScene();
	void drawGui();	

	// Parameters
	ofParameterGroup params;
	ofParameter<float> alpha;
	ofParameter<float> round;
	ofParameter<float> rotation;
	ofParameter<int> size;
	ofParameter<int> index;
	ofParameter<ofColor> colorShape;

	// Scene
	void drawShape(int x, int y, int size);
	void doChangeColor();
	vector<ofColor> colors;

	// Gui
	ofxSurfing_ImGui_Manager guiManager;
	ofParameter<bool> bGui{ "ofApp",true };

	// Undo Engine
	ofxSurfingUndoHelper undoManager;
};
