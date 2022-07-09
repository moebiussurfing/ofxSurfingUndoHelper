#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	setupParams();

	// Gui
	guiManager.setup();

	// Undo Engine
	undoManager.setup(params);
}

//--------------------------------------------------------------
void ofApp::setupParams()
{
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

	// Scene Parameters
	params.setName("myScene");
	params.add(alpha.set("alpha", 0.5f, 0, 1.0f));
	params.add(colorShape.set("color", ofColor(0), ofColor(0), ofColor(255)));
	params.add(size.set("size", ofGetWidth() / 2, 10, (int)ofGetHeight() / 2));
	params.add(round.set("round", 0.5, 0, 1));
	params.add(rotation.set("rotation", 180, 0, 360));
	params.add(index.set("index color", 0, 0, colors.size() - 1));
}

//--------------------------------------------------------------
void ofApp::draw()
{
	drawScene();

	drawGui();

	// This is an example of how to use auto add feature.
	// You should decide where in your app to add the params states,
	// e.g. when the params are randomized.
	// 
	// When auto is enabled, we will save the state every 120 frames / 2 seconds
	if (ofGetFrameNum() % 120 == 0)
	{
		if (undoManager.isAutoAddMode())
		{
			undoManager.doAddStateToUndo();
		}
	}

	// Check if index changed and update color
	static int index_PRE = -1;
	if (index != index_PRE) {
		index_PRE = index;
		doChangeColor();
	}
}

//--------------------------------------------------------------
void ofApp::drawGui()
{
	guiManager.begin();
	{
		if (guiManager.beginWindow("WIDGETS"))
		{
			guiManager.AddLabelBig("Useful widgets");
			guiManager.Add(guiManager.bMinimize);
			guiManager.AddSpacingBigSeparated();

			// Useful widgets to integrate our GUI
			 
			// Draw Widgets into an ofxSurfingImGui window
			undoManager.drawImGuiWidgetsBrowse(guiManager.bMinimize);
			
			guiManager.AddSpacingBigSeparated();

			// Draw history info
			undoManager.drawImGuiWidgetsHistoryInfo(guiManager.bMinimize);

			guiManager.endWindow();
		}

		//--

		if (guiManager.beginWindow(bGui))
		{
			// Show Undo show Gui toggle
			guiManager.AddLabelBig("Undo Helper");
			guiManager.Add(undoManager.bGui_UndoEngine, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
			guiManager.AddSpacingSeparated();

			//--

			// These are the params handled by the Undo Engine!
			// We modify the params here using the GUI!
			{
				guiManager.AddLabelBig("Scene Parameters");

				guiManager.AddGroup(params);
			}

			//--

			guiManager.endWindow();
		}

		//--

		// Undo Engine
		undoManager.drawImGuiWindow();
	}
	guiManager.end();
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
void ofApp::doChangeColor()
{
	static int k = 0;
	k = ++k % colors.size();
	colorShape = colors[k];
}

//--------------------------------------------------------------
void ofApp::keyPressed(ofKeyEventArgs& eventArgs) {

	const int key = eventArgs.key;
	ofLogNotice(__FUNCTION__) << " : " << key;

	// Undo Engine
	undoManager.keyPressed(eventArgs);
}
