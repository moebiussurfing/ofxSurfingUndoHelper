#pragma once

/*

TODO: ?
Files mode should be linked to the simple undo / ram mode... ?
For the momment files mode is independent and can be working together.
There's a also good to have both modes like they are now.

*/


#include "ofMain.h"
#include "ofxUndoSimple.h"

#include "ofxUndoHistoryToFile.h"
#include "ofFileUtils.h"

class UndoHistory : public ofx::undo::HistoryToFile {

public:

	//UndoHistory();
	//~UndoHistory();

	void save(const std::filesystem::path &path) const {
		ofBufferToFile(path, ofBuffer(str_.c_str(), str_.size()), false);
	}

	void load(const std::filesystem::path &path) {
		str_ = ofBufferFromFile(path, false).getText();
	}

	operator std::string&() {
		return str_;
	}

public:

	void clearRemoveFiles() {
		ofLogNotice(__FUNCTION__);
		directory_.remove(true);
	};

private:
	std::string str_;

	//-

//	ofxUndoSimple<std::string> &undo_StringParams;
//
//public:
//	void setUndoPtr(ofxUndoSimple<std::string> * u) {
//		undo_StringParams = *u;
//	}
};