
#include "ofxSurfingUndoHelper.h"

//--------------------------------------------------------------
ofxSurfingUndoHelper::ofxSurfingUndoHelper() {
}

//--------------------------------------------------------------
ofxSurfingUndoHelper::~ofxSurfingUndoHelper() {
	exit();
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::setup(ofParameterGroup &g) {
	params = g;
	path_UndoHistory = path_Global + "SurfingUndoHelper_UndoHistory.xml"; // -> TODO:
	path_AppState = path_Global + "SurfingUndoHelper_AppSession.json";

	params_AppState.add(bGui_UndoEngine);
	params_AppState.add(bAutoStore);
	params_AppState.add(bFilesMode);
	params_AppState.add(bKeys);

	//--

	setupUndo();
	loadUndoHist();

	//-

	guiManager.setup(IM_GUI_MODE_NOT_INSTANTIATED);

	ofxSurfingHelpers::loadGroup(params_AppState, path_AppState);

	//-

	//TODO:
	//files
	undo_StringParamsFiles.setDirectory(path_Global + "UndoHistory_Log");
	undo_StringParamsFiles.redo(undo_StringParamsFiles.getRedoLength());
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::setupUndo() {
	// TODO: main group only
	//undoStringParams = groups[0].toString();

	undo_StringParams.clear();

	//TODO:
	//files
	undo_StringParamsFiles.clear();

	undoXmlsParams.clear();

	undo_StringParams = params.toString();
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::doStoreUndo() {
	{
		undoXmlsParams.clear();

		ofParameterGroup _group = params;
		ofSerialize(undoXmlsParams, _group);// fill the xml with the ofParameterGroup

		std::string str;

		if (!bFilesMode) {
			undo_StringParams = undoXmlsParams.toString();// fill the ofxUndoSimple with the xml as string
			undo_StringParams.store();

			str = "UNDO HISTORY: " + ofToString(undo_StringParams.getUndoLength()) + "/";
			str += ofToString(undo_StringParams.getRedoLength());
		}

		if (bFilesMode) {
			//TODO:
			//using files
			(std::string&)undo_StringParamsFiles = undoXmlsParams.toString();
			//undo_StringParamsFiles.clearRedo();
			undo_StringParamsFiles.store();

			str = "UNDO HISTORY: " + ofToString(undo_StringParamsFiles.getUndoLength()) + "/";
			str += ofToString(undo_StringParamsFiles.getRedoLength());
		}

		//-

		//str += "\n";
		//str += "DESCRIPTOR\n";
		//str += undoStringParams.getUndoStateDescriptor() + "\n";

		ofLogNotice(__FUNCTION__) << str;
	}
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::doClearUndoHistory() {
	ofLogNotice(__FUNCTION__) << "UNDO CLEAR";

	if (!bFilesMode) undo_StringParams.clear();

	if (bFilesMode) {
		undo_StringParamsFiles.clearRedo();

		undo_StringParamsFiles.clearRemoveFiles();
		undo_StringParamsFiles.clear();
	}
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::doUndo() {
	ofLogNotice(__FUNCTION__) << "UNDO < Group " << params.getName();

	if (!bFilesMode) undo_StringParams.undo();

	//TODO:
	//files
	if (bFilesMode) undo_StringParamsFiles.undo();

	doRefreshUndoParams();
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::doRedo() {
	ofLogNotice(__FUNCTION__) << "REDO < Group " << params.getName();

	if (!bFilesMode) undo_StringParams.redo();

	//TODO:
	//files
	if (bFilesMode) undo_StringParamsFiles.redo();

	doRefreshUndoParams();
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::doRefreshUndoParams() {
	{
		undoXmlsParams.clear();

		if (!bFilesMode) undoXmlsParams.parse(undo_StringParams); // fill the xml with the string 
		else if (bFilesMode) undoXmlsParams.parse((std::string&)undo_StringParamsFiles); // fill the xml with the string 

		ofParameterGroup _group = params;
		ofDeserialize(undoXmlsParams, _group);// load the ofParameterGroup

		std::string str;
		
		if (!bFilesMode) {
			str = "UNDO HISTORY: " + ofToString(undo_StringParams.getUndoLength()) + "/";
			str += ofToString(undo_StringParams.getRedoLength());
		}
		if (bFilesMode) {
			str = "History: " + ofToString(undo_StringParamsFiles.getUndoLength()) + "/";
			str += ofToString(undo_StringParamsFiles.getRedoLength());
		}

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
		_flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

		guiManager.beginWindow(bGui_UndoEngine, _flagsw);
		{
			//ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);
			//if (ImGui::CollapsingHeader("UNDO ENGINE"))
			{
				ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);
				_h *= 2;

				ofxImGuiSurfing::AddBigToggle(bAutoStore);

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

				ImGui::PushButtonRepeat(true);
				if (ImGui::Button("< Undo", ImVec2(_w50, _h)))
				{
					doUndo();
				}
				ImGui::SameLine();
				if (ImGui::Button("Redo >", ImVec2(_w50, _h)))
				{
					doRedo();
				}
				ImGui::PopButtonRepeat();

				ImGui::Spacing();

				string str;
				str = "Group: " + params.getName();
				ImGui::Text(str.c_str());

				if (!bFilesMode) {
					str = "History: " + ofToString(undo_StringParams.getUndoLength()) + "/";
					str += ofToString(undo_StringParams.getRedoLength());
				}
				if (bFilesMode) {
					str = "History: " + ofToString(undo_StringParamsFiles.getUndoLength()) + "/";
					str += ofToString(undo_StringParamsFiles.getRedoLength());
				}
				ImGui::Text(str.c_str());

				string label = (bFilesMode.get() ? "Files Mode" : "RAM Mode");
				ofxImGuiSurfing::ToggleRoundedButton(label.c_str(), (bool*)&bFilesMode.get());
				//ofxImGuiSurfing::AddToggleRoundedButton(bFilesMode);

				ofxImGuiSurfing::AddToggleRoundedButton(bKeys);
			}
		}
		guiManager.endWindow();
	}
}

//----

//TODO:
//store session files

//--------------------------------------------------------------
void ofxSurfingUndoHelper::loadUndoHist() {
	//undo_StringParams.clear();
	//undoXmlsParams.clear();
	//undo_StringParams = params.toString();

	//undo_StringParams
	//doRefreshUndoParams();
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::saveUndoHist() {
	ofLogNotice(__FUNCTION__) << path_UndoHistory;
	//ofxSurfingHelpers::saveGroup();

	//TODO:
	string s = undo_StringParams;
	//auto s = undo_StringParams.toString();
	//auto s = undoXmlsParams.toString();
	ofLogNotice(__FUNCTION__) << s;

	ofxSurfingHelpers::TextToFile(path_UndoHistory, s, false);

	//-

	//undo_StringParamsFiles.();
	//undo_StringParamsFiles.store();
}

//--

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