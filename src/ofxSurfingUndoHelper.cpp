#include "ofxSurfingUndoHelper.h"


//----

//--------------------------------------------------------------
ofxSurfingUndoHelper::ofxSurfingUndoHelper() {
}

//--------------------------------------------------------------
ofxSurfingUndoHelper::~ofxSurfingUndoHelper() {
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::setup(ofParameterGroup &g) {
	params = g;
	path_UndoHistory = path_Global + "ofxSurfingUndoHelper_UndoHistory.xml";
	path_AppState = path_Global + "ofxSurfingUndoHelper_AppSession.json";

	params_AppState.add(bGui_UndoEngine);
	params_AppState.add(bUndoAuto);
	params_AppState.add(bKeys);

	//--

	setupUndo();
	loadUndoHist();

	//-

	guiManager.setup(IM_GUI_MODE_NOT_INSTANTIATED);

	ofxSurfingHelpers::loadGroup(params_AppState, path_AppState);
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::setupUndo() {
	// TODO: main group only
	//undoStringParams = groups[0].toString();

	undoStringsParams.clear();

	undoXmlsParams.clear();

	undoStringsParams = params.toString();
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::doStoreUndo() {
	{
		undoXmlsParams.clear();

		ofParameterGroup _group = params;
		ofSerialize(undoXmlsParams, _group);// fill the xml with the ofParameterGroup

		undoStringsParams = (undoXmlsParams.toString());// fill the ofxUndoSimple with the xml as string
		undoStringsParams.store();

		std::string str = "";
		str += "UNDO HISTORY: " + ofToString(undoStringsParams.getUndoLength()) + "/";
		str += ofToString(undoStringsParams.getRedoLength());
		//str += "\n";
		//str += "DESCRIPTOR\n";
		//str += undoStringParams.getUndoStateDescriptor() + "\n";

		ofLogNotice(__FUNCTION__) << str;
	}
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::doClearUndoHistory() {
	ofLogNotice(__FUNCTION__) << "UNDO CLEAR";
	undoStringsParams.clear();
	//undoStringParams.clear();
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::doUndo() {
	ofLogNotice(__FUNCTION__) << "UNDO < Group " << params.getName();
	undoStringsParams.undo();
	//undoStringParams.undo();
	doRefreshUndoParams();
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::doRedo() {
	ofLogNotice(__FUNCTION__) << "REDO < Group " << params.getName();
	undoStringsParams.redo();
	//undoStringParams.redo();
	doRefreshUndoParams();
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::doRefreshUndoParams() {
	{
		undoXmlsParams.clear();
		undoXmlsParams.parse(undoStringsParams);// fill the xml with the string 

		ofParameterGroup _group = params;
		ofDeserialize(undoXmlsParams, _group);// load the ofParameterGroup

		std::string str = "";
		str += "UNDO HISTORY: " + ofToString(undoStringsParams.getUndoLength()) + "/";
		str += ofToString(undoStringsParams.getRedoLength());
		//str += "\n";
		//str += "DESCRIPTOR\n";
		//str += undoStringParams.getUndoStateDescriptor() + "\n";

		ofLogNotice(__FUNCTION__) << str;
	}
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::drawImGui() {

	if (!bGui_UndoEngine.get()) return;

	{
		// window editor
		ImGuiWindowFlags _flagsw;
		string n;

		bool bOpen;
		ImGuiColorEditFlags _flagc;

		// widgets sizes
		float _w100;
		float _w50;
		float _w33;
		float _w25;
		float _h;

		_flagsw = ImGuiWindowFlags_None;

#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
		if (guiManager.bGui) _flagsw |= ImGuiWindowFlags_AlwaysAutoResize;
#endif

		guiManager.beginWindow(bGui_UndoEngine, _flagsw);
		{
			//ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);
			//if (ImGui::CollapsingHeader("UNDO ENGINE"))
			{
				ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);
				_h *= 2;

				ofxImGuiSurfing::AddBigToggle(bUndoAuto);

				if (ImGui::Button("Store", ImVec2(_w50, _h)))
				{
					doStoreUndo();
				}
				ImGui::SameLine();
				if (ImGui::Button("Clear", ImVec2(_w50, _h)))
				{
					doClearUndoHistory();
				}
				//ImGui::SameLine();

				if (ImGui::Button("< Undo", ImVec2(_w50, _h)))
				{
					doUndo();
				}
				ImGui::SameLine();
				if (ImGui::Button("Redo >", ImVec2(_w50, _h)))
				{
					doRedo();
				}
				ImGui::Spacing();

				string str;
				str = "History: " + ofToString(undoStringsParams.getUndoLength()) + "/";
				str += ofToString(undoStringsParams.getRedoLength());
				ImGui::Text(str.c_str());

				ofxImGuiSurfing::AddToggleRoundedButton(bKeys);
			}
		}
		guiManager.endWindow();
	}
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::loadUndoHist() {
	//undoStringsParams.clear();
	//undoXmlsParams.clear();
	//undoStringsParams = params.toString();

	//undoStringsParams
	//doRefreshUndoParams();
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::saveUndoHist() {
	ofLogNotice(__FUNCTION__) << path_UndoHistory;
	//ofxSurfingHelpers::saveGroup();

	//TODO:
	string s = undoStringsParams;
	//auto s = undoStringsParams.toString();
	//auto s = undoXmlsParams.toString();
	ofLogNotice(__FUNCTION__) << s;

	ofxSurfingHelpers::TextToFile(path_UndoHistory, s, false);
}


//--------------------------------------------------------------
void ofxSurfingUndoHelper::keyPressed(ofKeyEventArgs &eventArgs) {
	if (!bKeys) return;

	const int key = eventArgs.key;

	// modifiers
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);
	
	ofLogNotice(__FUNCTION__) << " : " << key;

	if (bGui_UndoEngine.get())
	{
		if (!mod_SHIFT && mod_CONTROL && (key == 'z' || key == 26))// previous
		{
			doUndo();
		}
		else if (mod_SHIFT && mod_CONTROL && (key == 'Z' || key == 26))// next
		{
			doRedo();
		}
		else if (mod_SHIFT && mod_CONTROL && key == 'C' || key == 3)// clear
		{
			doClearUndoHistory();
		}
		else if (!mod_SHIFT && mod_CONTROL && key == 's' || key == 19)// store
		{
			doStoreUndo();
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::exit() {
	saveUndoHist();

	ofxSurfingHelpers::saveGroup(params_AppState, path_AppState);
}