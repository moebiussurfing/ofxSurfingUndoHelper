#pragma once

#include "ofMain.h"

/*

	TODO:

	- Allow multiple independent groups.
	- Add lite version without ImGui but ofxGui instead.

*/


// Undo Engine

#include "ofxUndoSimple.h"
#include "surfingFiles.h"
#include "ofxSurfingImGui.h"
#include "ofxSurfingHelpers.h"

#include "UndoHistory.h"


class ofxSurfingUndoHelper
{

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

	string path_Global = "";
	string path_MemoryState;//there's a memory state to play with reset/random workflow
	ofParameterGroup params{ "ofxSurfingUndoHelper" };
	ofParameterGroup params_AppState{ "ofxSurfingUndoHelper_AppState" };

//--

public:

	ofParameter<bool> bGui_UndoEngine{ "Undo Engine", false };
	ofParameter<bool> bAutoStore{ "Auto Store", true };
	ofParameter<bool> bFilesMode{ "Files Mode", false };

private:

	string path_UndoHistory;
	string path_AppState;

	ofxUndoSimple <std::string> undo_StringParams;
	
	ofXml undoXmlsParams;

	void setupUndo();
	void doRefreshUndoParams();

public:

	void doSaveUndo(); // Force store current point to undo history
	void doSaveUndoWhenAuto() {// Store current point to undo history, but ONLY when auto mode is enabled.
		if (bAutoStore) doSaveUndo();
	}; 
	
	void doUndo();
	void doRedo();
	void doClearUndoHistory();

	void doStoreState();//save the memory state
	void doRecallState();//load the memory state

	ofParameter<bool> bKeys{ "Keys", true };

private:

	void exit();

public:

	void keyPressed(ofKeyEventArgs &eventArgs);
	
	string helpInfo;

private:
//public:

	//TODO: save/load history
	void loadUndoHist();
	void saveUndoHist();

private:

	//TODO: 
	//files mode
	//save/load history
	UndoHistory undo_StringParamsFiles;

private:

	ofxSurfing_ImGui_Manager guiManager;
};