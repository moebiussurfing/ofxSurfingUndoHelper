#pragma once

#include "ofMain.h"

/*

	TODO:

	- add path global
	- allow multiple independent groups.
	- add lite version without ImGui but ofxGui instead.

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

	void setup(ofParameterGroup& g);

	void drawImGuiWindow();

	//--------------------------------------------------------------
	void setPathGlobal(string path) {
		path_Global = path;
	}

	//--------------------------------------------------------------
	ofParameterGroup& getParamsAppState()
	{
		return params_AppState;
	}

	//--
	// 
	//  Helper widgets to integrate into our GUI
	// 
	//--------------------------------------------------------------
	void drawImGuiWidgetsBrowse(bool bMinimal = false)
	{
		float _w1 = getWidgetsWidth(1);
		float _w2 = getWidgetsWidth(2);
		float _h = getWidgetsHeightUnit();

		if (bMinimal) {
			_h *= 1.25f;
		}
		else {
			_h *= 2;
		}

		ImGui::PushButtonRepeat(true);
		if (ImGui::Button("< UNDO", ImVec2(_w2, _h)))
		{
			doUndo();
		}
		ImGui::SameLine();
		if (ImGui::Button("REDO >", ImVec2(_w2, _h)))
		{
			doRedo();
		}
		ImGui::PopButtonRepeat();
	}

	//--------------------------------------------------------------
	void drawImGuiWidgetsHistoryInfo(bool bMinimal = false)
	{
		ImGui::Spacing();

		string str;

		if (bMinimal) {
			if (!bFilesMode) {
				str = ofToString(undo_StringParams.getUndoLength()) + "/";
				str += ofToString(undo_StringParams.getRedoLength());
			}
			else {
				str = ofToString(undo_StringParamsFiles.getUndoLength()) + "/";
				str += ofToString(undo_StringParamsFiles.getRedoLength());
			}
			ImGui::Text(str.c_str());
		}
		else {
			str = "Group " + params.getName();
			ImGui::Text(str.c_str());

			if (!bFilesMode) {
				str = "History " + ofToString(undo_StringParams.getUndoLength()) + "/";
				str += ofToString(undo_StringParams.getRedoLength());
			}
			else {
				str = "History " + ofToString(undo_StringParamsFiles.getUndoLength()) + "/";
				str += ofToString(undo_StringParamsFiles.getRedoLength());
			}
			ImGui::Text(str.c_str());
		}

	}

private:

	string path_Global = "";
	string path_MemoryState;//there's a memory state to play with reset/random workflow
	ofParameterGroup params{ "ofxSurfingUndoHelper" };
	ofParameterGroup params_AppState{ "ofxSurfingUndoHelper_AppState" };

	//--

public:

	ofParameter<bool> bGui_UndoEngine{ "UNDO ENGINE", false };
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

	void keyPressed(ofKeyEventArgs& eventArgs);

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