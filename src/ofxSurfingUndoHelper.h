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
		ofxSurfingHelpers::CheckFolder(path_Global);
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
		ui.refreshLayout();

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
		if (ImGui::Button("UNDO", ImVec2(_w2, _h)))
		{
			doUndo();
		}
		ImGui::SameLine();
		if (ImGui::Button("REDO", ImVec2(_w2, _h)))
		{
			doRedo();
		}
		ImGui::PopButtonRepeat();
		
		//--

		if (!bMinimal)
		{
			if (ImGui::Button("ADD", ImVec2(_w2, _h)))
			{
				doAddStateToUndo();
			}
			ImGui::SameLine();
			ofxImGuiSurfing::AddBigToggle(bAutoAddStatesToUndo, _w2, _h, true, false);

			if (ImGui::Button("CLEAR", ImVec2(_w1, _h * 0.75f)))
			{
				doClearUndoHistory();
			}
		}

		//if (ImGui::Button("STORE", ImVec2(_w2, _h)))
		//{
		//	doStoreState();
		//}
		//ImGui::SameLine();
		//if (ImGui::Button("RECALL", ImVec2(_w2, _h)))
		//{
		//	doRecallState();
		//}
	}

	//--------------------------------------------------------------
	void drawImGuiWidgetsHistoryInfo(bool bMinimal = false)
	{
		ImGui::Spacing();

		string str;

		if (bMinimal) 
		{
			if (!bFilesMode) 
			{
				str = ofToString(undo_StringParams.getUndoLength()) + "/";
				str += ofToString(undo_StringParams.getRedoLength());
			}
			else 
			{
				str = ofToString(undo_StringParamsFiles.getUndoLength()) + "/";
				str += ofToString(undo_StringParamsFiles.getRedoLength());
			}
			ImGui::Text("%s", str.c_str());
		}
		else 
		{
			//str = "Group " + params.getName();
			str = params.getName();
			ImGui::Text("%s", str.c_str());

			if (!bFilesMode) 
			{
				str = "History " + ofToString(undo_StringParams.getUndoLength()) + "/";
				str += ofToString(undo_StringParams.getRedoLength());
			}
			else 
			{
				str = "History " + ofToString(undo_StringParamsFiles.getUndoLength()) + "/";
				str += ofToString(undo_StringParamsFiles.getRedoLength());
			}
			ImGui::Text("%s", str.c_str());
		}

	}

private:

	string path_Global = "";
	string path_MemoryState;//there's a memory state to play with reset/random workflow
	ofParameterGroup params{ "UndoHelper" };
	ofParameterGroup params_AppState{ "UndoHelper_AppState" };

	//--

public:

	bool isAutoAddMode() const {
		return bAutoAddStatesToUndo.get();
	}

	ofParameter<bool> bGui_UndoEngine{ "UNDO ENGINE", false };
	ofParameter<bool> bAutoAddStatesToUndo{ "AUTO ADD", true };
	ofParameter<bool> bFilesMode{ "Files Mode", true };//default is file mode. could be slow but resumen undo history between app sessions.

private:

	string path_UndoHistory;
	string path_AppState;

	ofxUndoSimple <std::string> undo_StringParams;

	ofXml undoXmlsParams;

	void setupUndo();
	void doRefreshUndoParams();

public:

	void doAddStateToUndo(); // Force store current point to undo history
	
	//--------------------------------------------------------------
	void doSaveUndoWhenAuto() {// Store current point to undo history, but ONLY when auto mode is enabled.
		if (bAutoAddStatesToUndo) doAddStateToUndo();
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

	ofxSurfingGui ui;
};
