
#include "ofxSurfingUndoHelper.h"

//--------------------------------------------------------------
ofxSurfingUndoHelper::ofxSurfingUndoHelper() {

	helpInfo = "";
	helpInfo += "HELP \n";
	helpInfo += "UNDO HELPER \n";
	helpInfo += " \n";
	helpInfo += "KEY COMMANDS \n";
	helpInfo += "\n";
	helpInfo += "AUTO STORE " + ofToString(bAutoStore ? "TRUE" : "FALSE") + " \n";
	helpInfo += "ENTER            : SAVE UNDO \n";
	//helpInfo += "CTRL + ENTER     : SAVE UNDO IF AUTO\n";
	helpInfo += "\n";
	helpInfo += "CTRL + Z         : UNDO  \n";
	helpInfo += "CTRL + SHIFT + Z : REDO  \n";
	helpInfo += "CTRL + C         : CLEAR HISTORY \n";
	helpInfo += "\n";
	helpInfo += "CTRL + S         : STATE STORE \n";
	helpInfo += "CTRL + R         : STATE RECALL \n";
}

//--------------------------------------------------------------
ofxSurfingUndoHelper::~ofxSurfingUndoHelper() {
	exit();
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::setup(ofParameterGroup& g) {
	params = g;
	path_UndoHistory = path_Global + "UndoHelper_UndoHistory.xml"; // -> TODO:
	path_AppState = path_Global + "UndoHelper_AppSession.json";
	path_MemoryState = path_Global + "UndoHelper_MemoryState.json";

	params_AppState.add(bGui_UndoEngine);
	params_AppState.add(bAutoStore);
	params_AppState.add(bFilesMode);
	params_AppState.add(guiManager.bKeys);

	bKeys.makeReferenceTo(guiManager.bKeys);

	//--

	setupUndo();
	loadUndoHist();

	//-

	guiManager.setup(IM_GUI_MODE_NOT_INSTANTIATED);

	ofxSurfingHelpers::loadGroup(params_AppState, path_AppState);

	//-

	//TODO:
	//files
	undo_StringParamsFiles.setDirectory(path_Global + "UndoHelper_History_Log");
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
void ofxSurfingUndoHelper::doSaveUndo() {
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

		IMGUI_SUGAR__WINDOWS_CONSTRAINTS

			if (guiManager.beginWindow(bGui_UndoEngine, _flagsw))
			{
				ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);
				_h *= 2;

				guiManager.Add(guiManager.bMinimize, OFX_IM_TOGGLE_ROUNDED);
				guiManager.AddSpacingBig();

				if (!guiManager.bMinimize) {
					guiManager.Add(bAutoStore, OFX_IM_TOGGLE_BORDER_BLINK);
					guiManager.AddSpacing();
				}

				if (ImGui::Button("Save", ImVec2(_w50, _h * 0.5f)))
				{
					doSaveUndo();
				}
				ImGui::SameLine();
				if (ImGui::Button("Clear", ImVec2(_w50, _h * 0.5f)))
				{
					doClearUndoHistory();
				}

				guiManager.AddSpacing();

				ImGui::PushButtonRepeat(true);
				if (ImGui::Button("< Undo", ImVec2(_w50, _h * 1.5f)))
				{
					doUndo();
				}
				ImGui::SameLine();
				if (ImGui::Button("Redo >", ImVec2(_w50, _h * 1.5f)))
				{
					doRedo();
				}
				ImGui::PopButtonRepeat();

				guiManager.AddSpacingBigSeparated();

				ImGuiTreeNodeFlags f = (guiManager.bMinimize ? ImGuiTreeNodeFlags_NoAutoOpenOnLog : ImGuiTreeNodeFlags_DefaultOpen);
				if (ImGui::TreeNodeEx("MEMORY", f))
				{
					ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

					if (ImGui::Button("Store", ImVec2(_w50, _h)))
					{
						doStoreState();
					}

					ImGui::SameLine();

					if (ImGui::Button("Recall", ImVec2(_w50, _h)))
					{
						doRecallState();
					}

					ImGui::TreePop();
				}

				if (!guiManager.bMinimize)
				{
					guiManager.AddSpacingBigSeparated();

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

					guiManager.AddSpacingBigSeparated();

					string label = (bFilesMode.get() ? "Files Mode" : "RAM Mode");
					ofxImGuiSurfing::ToggleRoundedButton(label.c_str(), (bool*)&bFilesMode.get());
					//ofxImGuiSurfing::AddToggleRoundedButton(bFilesMode);

					ofxImGuiSurfing::AddToggleRoundedButton(bKeys);
				}

				guiManager.endWindow();
			}
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
void ofxSurfingUndoHelper::keyPressed(ofKeyEventArgs& eventArgs) {
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
		if (!mod_CONTROL && key == OF_KEY_RETURN || key == 13) // force store undo
		{
			doSaveUndo();
		}
		if (mod_CONTROL && key == OF_KEY_RETURN || key == 13) // force store undo
		{
			doSaveUndoWhenAuto();
		}

		if (!mod_SHIFT && mod_CONTROL && (key == 'z' || key == 26))// previous
		{
			doUndo();
		}
		else if (mod_SHIFT && mod_CONTROL && (key == 'Z' || key == 26))// next
		{
			doRedo();
		}
		else if (!mod_SHIFT && mod_CONTROL && key == 'C' || key == 3)// clear
		{
			doClearUndoHistory();
		}

		else if (!mod_SHIFT && mod_CONTROL && key == 's' || key == 19)// store
		{
			doStoreState();
		}
		else if (!mod_SHIFT && mod_CONTROL && key == 'r' || key == 18)// recall
		{
			doRecallState();
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::doStoreState() {
	ofLogNotice(__FUNCTION__);
	ofxSurfingHelpers::saveGroup(params, path_MemoryState);
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::doRecallState() {
	ofLogNotice(__FUNCTION__);
	ofxSurfingHelpers::loadGroup(params, path_MemoryState);
}

//--------------------------------------------------------------
void ofxSurfingUndoHelper::exit() {
	saveUndoHist();

	ofxSurfingHelpers::saveGroup(params_AppState, path_AppState);
}