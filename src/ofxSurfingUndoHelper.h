#pragma once

#include "ofMain.h"

// undo engine
#include "ofxUndoSimple.h"
#include "ofxSurfing_Files.h"
#include "ofxSurfingImGui.h"
#include "ofxSurfingHelpers.h"

#include "UndoHistory.h"

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

	string path_Global = "";
	ofParameterGroup params{ "ofxSurfingUndoHelper" };
	ofParameterGroup params_AppState{ "ofxSurfingUndoHelper_AppState" };

//--

public:

	ofParameter<bool> bGui_UndoEngine{ "Undo Engine", false };
	ofParameter<bool> bAutoStore{ "Auto Store", true };
	ofParameter<bool> bFilesMode{ "Files Mode", false };

	void doStoreUndo();// store current point to undo history
	void doStoreUndoWhenAuto() {
		if (bAutoStore) doStoreUndo();
	}; // store current point to undo history when auto mode is enabled

private:

	string path_UndoHistory;
	string path_AppState;

	ofxUndoSimple <std::string> undo_StringParams;
	
	ofXml undoXmlsParams;

	void doRefreshUndoParams();
	void doUndo();
	void doRedo();
	void doClearUndoHistory();
	void setupUndo();

	ofParameter<bool> bKeys{ "Keys", true };

	void exit();

public:

	void keyPressed(ofKeyEventArgs &eventArgs);
	
	string helpInfo;

//public:
private:

	//TODO: save/load history
	void loadUndoHist();
	void saveUndoHist();

private:

	//TODO: 
	//files mode
	//save/load history
	UndoHistory undo_StringParamsFiles;

};