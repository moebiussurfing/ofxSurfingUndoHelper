#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	// Parameters

	params.setName("theParamsGroup");
	params.add(alpha.set("alpha", 0.5f, 0, 1.0f));
	params.add(colorShape.set("Color", ofColor(0), ofColor(0), ofColor(255)));
	params.add(size.set("size", ofGetWidth() / 2, 10, (int)ofGetHeight() / 2));
	params.add(round.set("round", 0.5, 0, 1));
	params.add(rotation.set("rotation", 180, 0, 360));

	// Scene Colors

	colors.push_back(ofColor::red);
	colors.push_back(ofColor::green);
	colors.push_back(ofColor::blue);
	colors.push_back(ofColor::yellow);
	colors.push_back(ofColor::royalBlue);
	colors.push_back(ofColor::lawnGreen);
	colors.push_back(ofColor::cornflowerBlue);
	colors.push_back(ofColor::greenYellow);
	colors.push_back(ofColor::mediumSeaGreen);

	//--

	// Randomizer
	// editing to test Undo workflow

#ifdef USE__OFX_SURFING__OFX_SURFING_UNDEO_HELPER 
	surfingParamsRandom.setup(params);
#endif

	surfingParamsRandom.doReset();

	//--

	// Gui
	guiManager.setup();

	//--

	// Undo Manager
	undoManager.setup(params);

	//--

	// Help Info
	{
		textBoxWidget.setup();
		textBoxWidget.setMode(TextBoxWidget::BOX_LAYOUT::TOP_RIGHT);

		string helpInfo;
		helpInfo = "";
		helpInfo += "HELP  \n";
		helpInfo += "ofApp \n";
		helpInfo += "\n";
		helpInfo += "HOW TO \n";
		helpInfo += "\n";
		helpInfo += "1. TWEAK SCENE to modify the parameters. \n";
		helpInfo += "2. Browse the random Undo History. \n";
		helpInfo += "\n";
		helpInfo += "TWEAK SCENE \n";
		helpInfo += "\n";
		helpInfo += "BACKSPACE : RESET  \n";
		helpInfo += "SPACE     : RANDOM \n";
		helpInfo += "Each tweak trig change the color. \n";
		helpInfo += "\n";
		helpInfo += "---------------------------------\n";
		helpInfo += "\n";

#ifdef USE__OFX_SURFING__OFX_SURFING_UNDEO_HELPER 
		helpInfo += undoManager.helpInfo;
#endif

		textBoxWidget.setText(helpInfo);
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	guiManager.begin();
	{
		if (guiManager.beginWindow(bGui))
		{
			// ofApp params fast editing to test Undo workflow
			{
				guiManager.AddLabelBig("Scene Tweak");

				float w = ofxImGuiSurfing::getWidgetsWidth(1);
				float w2 = ofxImGuiSurfing::getWidgetsWidth(2);
				float h = 2 * ofxImGuiSurfing::getWidgetsHeightUnit();

				if (ImGui::Button("RESET", ImVec2(w2, h))) {
					doReset();

				}
				ImGui::SameLine();
				if (ImGui::Button("RANDOM", ImVec2(w2, h))) {
					doRandom();
				}
				guiManager.AddSpacingSeparated();
			}

			//--

			// Show Undo Gui toggle

#ifdef USE__OFX_SURFING__OFX_SURFING_UNDEO_HELPER 
			{
				guiManager.AddLabelBig("Undo Helper");
				guiManager.Add(undoManager.bGui_UndoEngine, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
				guiManager.AddSpacingSeparated();
			}
#endif

			//--

			// Params
			{
				guiManager.AddLabelBig("Scene Parameters");
				guiManager.AddGroup(params);
			}

			//--

			guiManager.endWindow();
		}

		//--

		undoManager.drawImGui();
	}
	guiManager.end();

	//--

	drawScene();

	//--

	textBoxWidget.draw();
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	static ofColor colorBg = 32;

	ofClear(colorBg);

	ofPushStyle();
	ofPushMatrix();
	{
		ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

		// alpha
		float _a = ofMap(alpha, alpha.getMin(), alpha.getMax(), 0.1, 1);
		ofSetColor(colorShape, 255 * _a);

		// size
		float _sz = 100 + size * 1.1;
		// rotation
		float _rot = rotation / 3.0;
		ofRotateDeg(ofGetElapsedTimef() * TWO_PI);
		ofRotateZDeg(45);
		ofRotateDeg(_rot);

		float _s = ofMap(rotation, rotation.getMin(), rotation.getMax(), 0, 0.1);
		float _scale = 1;

		drawShape(-_sz / 2, -_sz / 2, _sz);

		ofRotateDeg(_rot);
		_scale -= _s;
		ofScale(_scale);
		drawShape(-_sz / 2, -_sz / 2, _sz);

		ofRotateDeg(_rot);
		_scale -= _s;
		ofScale(_scale);
		drawShape(-_sz / 2, -_sz / 2, _sz);

		ofRotateDeg(_rot);
		_scale -= _s;
		ofScale(_scale);
		drawShape(-_sz / 2, -_sz / 2, _sz);
	}
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawShape(int x, int y, int size)
{
	float r = ofMap(round, 0, 1, 0, 30);
	ofDrawRectRounded(x, y, size, size, r);
}

//--------------------------------------------------------------
void ofApp::keyPressed(ofKeyEventArgs& eventArgs) {

	const int key = eventArgs.key;

	ofLogNotice(__FUNCTION__) << " : " << key;

	// Reset
	if (key == OF_KEY_BACKSPACE)
	{
		doReset();
	}

	// Randomize
	if (key == ' ')
	{
		doRandom();
	}

	//--

#ifdef USE__OFX_SURFING__OFX_SURFING_UNDEO_HELPER 
	undoManager.keyPressed(eventArgs);
#endif
}


//--------------------------------------------------------------
void ofApp::doReset() {
	surfingParamsRandom.doReset();
	doChangeColor();

	undoManager.doSaveUndoWhenAuto();
}

//--------------------------------------------------------------
void ofApp::doRandom() {

	surfingParamsRandom.doRandom();
	doChangeColor();

	//--

#ifdef USE__OFX_SURFING__OFX_SURFING_UNDEO_HELPER 
	undoManager.doSaveUndoWhenAuto();
	// Another method:
	//undoManager.doSaveUndo(); // force store ignoring auto store toggle
#endif
}

//--------------------------------------------------------------
void ofApp::doChangeColor()
{
	static int k = 0;
	k = ++k % colors.size();
	colorShape = colors[k];
}
