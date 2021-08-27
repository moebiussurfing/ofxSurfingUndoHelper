#pragma once

#include "ofMain.h"

// undo engine
#include "ofxUndoSimple.h"
#include "ofxSurfing_Files.h"
#include "ofxSurfingImGui.h"
#include "ofxSurfingHelpers.h"

class ofxSurfingUndoHelper
{
private:
	ofxSurfing_ImGui_Manager guiManager;

public:
	ofxSurfingUndoHelper();
	~ofxSurfingUndoHelper();

	void setup(ofParameterGroup &g);

	void drawImGui();

	//--------------------------------------------------------------
	void setPathGlobal(string path) {
		path_Global = path;
	}

	//--------------------------------------------------------------
	ofParameterGroup &getParamsAppState()
	{
		return params_AppState;
	}

private:
	string path_Global;
	ofParameterGroup params{ "ofxSurfingUndoHelper" };
	ofParameterGroup params_AppState{ "ofxSurfingUndoHelper_AppState" };

//--

public:
	// undo engine
	// you can manually store all the parameters states to store points
	// only works on edit mode
	// then you can browse doing undo/redo to decide what states you like more.
	// when doing a random, the engine auto stores the states.
	// currently working automatic only when called by key command (ctrl+R) not when clicking gui "randomize parameters"
	// when called using gui button, you must store states manually (ctrl+s)

	ofParameter<bool> bGui_UndoEngine{ "Undo Engine", false };
	ofParameter<bool> bUndoAuto{ "Undo Auto", false };

	void doStoreUndo();// store current point to undo history
	void doStoreUndoWhenAuto() {
		if (bUndoAuto) doStoreUndo();
	}; // store current point to undo history when auto mode is enabled

private:
	string path_UndoHistory;
	string path_AppState;
	ofxUndoSimple <std::string> undoStringsParams;
	ofXml undoXmlsParams;

	void doRefreshUndoParams();
	void doUndo();
	void doRedo();
	void doClearUndoHistory();
	void setupUndo();

	ofParameter<bool> bKeys{ "Keys", true };

public:
	//TODO: sqave/load history
	void loadUndoHist();
	void saveUndoHist();

	void keyPressed(ofKeyEventArgs &eventArgs);
	void exit();

};