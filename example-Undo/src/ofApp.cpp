#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	setupScene();

	// Randomizer
	// editing to test Undo workflow
	surfingParamsRandom.setup(params);
	surfingParamsRandom.doReset();

	//--

	// Undo Engine
#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER 
	undoManager.setPathGlobal("ofApp/");
	undoManager.setup(params);
#endif

	//--

	setupGui();
}

//--------------------------------------------------------------
void ofApp::setupScene()
{
	// Scene Parameters
	params.setName("myScene");
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
}

//--------------------------------------------------------------
void ofApp::setupGui()
{
	guiManager.setup();

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

		// Undo Engine
#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER 
		helpInfo += undoManager.helpInfo;
#endif

		textBoxWidget.setText(helpInfo);
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	drawScene();

	//--

	drawGui();
}

//--------------------------------------------------------------
void ofApp::drawGui()
{
	guiManager.begin();
	{
		// Useful widgets to integraste our GUI
		if (guiManager.beginWindow("WIDGETS"))
		{
			guiManager.AddLabelBig("Useful widgets");
			guiManager.Add(guiManager.bMinimize);
			guiManager.AddSpacingBigSeparated();

			undoManager.drawImGuiWidgetsBrowse(guiManager.bMinimize);
			
			guiManager.AddSpacingBigSeparated();
			
			undoManager.drawImGuiWidgetsHistoryInfo(guiManager.bMinimize);

			guiManager.endWindow();
		}

		//--

		// ofApp params fast editing to test Undo workflow
		if (guiManager.beginWindow(bGui))
		{
			{
				guiManager.AddLabelBig("Scene Tweak");

				float _w1 = getWidgetsWidth(1);
				float _w2 = getWidgetsWidth(2);
				float _h2 = 2 * getWidgetsHeightUnit();

				if (ImGui::Button("RESET", ImVec2(_w2, _h2)))
				{
					doReset();
				}

				ImGui::SameLine();
				
				if (ImGui::Button("RANDOM", ImVec2(_w2, _h2))) 
				{
					doRandom();
				}

				guiManager.AddSpacingSeparated();
			}

			//--

			// Show Undo show Gui toggle
			// Undo Engine
#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER 
			{
				guiManager.AddLabelBig("Undo Helper");
				guiManager.Add(undoManager.bGui_UndoEngine, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
				guiManager.AddSpacingSeparated();
			}
#endif
			//--

			// -> The Params handled by the Undo Engine!
			{
				guiManager.AddLabelBig("Scene Parameters");

				guiManager.AddGroup(params);
			}

			//--

			guiManager.endWindow();
		}

		//--

		// Undo Engine
#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER 
		undoManager.drawImGuiWindow();
#endif

		//--
	}
	guiManager.end();

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

	// Undo Engine
#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER 
	undoManager.keyPressed(eventArgs);
#endif

	//--

	// Help
	if (key == OF_KEY_TAB)
	{
		textBoxWidget.setToggleMode();
	}
}

// Scene Helpers
//--------------------------------------------------------------
void ofApp::doReset() 
{
	surfingParamsRandom.doReset();
	doChangeColor();

	//--

	// Undo Engine
#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER 
	undoManager.doSaveUndoWhenAuto();
#endif
}

//--------------------------------------------------------------
void ofApp::doRandom() 
{

	surfingParamsRandom.doRandom();
	doChangeColor();

	//--

	// Undo Engine
#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER 
	// Store states only if auto store enabled.
	undoManager.doSaveUndoWhenAuto();

	// Alternative method:
	// force store, ignoring auto store toggle.
	//undoManager.doSaveUndo(); 
#endif
}

//--------------------------------------------------------------
void ofApp::doChangeColor()
{
	static int k = 0;
	k = ++k % colors.size();
	colorShape = colors[k];
}
