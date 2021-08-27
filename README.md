# ofxSurfingUndoHelper
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![Stage: beta](https://img.shields.io/badge/-alpha-red)

## Overview
An add-on helper to integrate an **Undo Engine** based on [ofxUndo](https://github.com/nariakiiwatani/ofxUndo) from [nariakiiwatani](https://github.com/nariakiiwatani).

## Screenshot
![Capture](docs/Capture.PNG?raw=true "Capture")

## Features
- Controls the **ofParameters** inside an **ofParameterGroup**.
- Adds **ImGui** controls to improve the workflow.

## Usage
- Check this example:  
https://github.com/moebiussurfing/ofxSurfingRandomizer/tree/master/example-RandomScene

ofApp.h
```.cpp
#include "ofxSurfingUndoHelper.h"
#include "ofxSurfingImGui.h"

ofxSurfingUndoHelper undoManger;
ofxSurfing_ImGui_Manager guiManager;
ofParameterGroup params {"Params"};

```

ofApp.cpp
```.cpp
void ofApp::setup()
{
    params.add(..) // -> add params

    undoManger.setup(params);
    undoManger.drawImGui();

    guiManager.setup();
}

void ofApp::draw()
{
    guiManager.beginWindow();
    {
        undoManger.drawImGui();
    }
    guiManager.endWindow();
}

void ofApp::keyPressed(int key)
{
    if (key == 's'){
        undoManger.doStoreUndo(); // -> Force store params current state
        //undoManger.doStoreUndoWhenAuto(); // -> Stores params state but if auto mode is enabled by GUI. 
    }
}
```

## Dependencies
* [ofxUndo](https://github.com/moebiussurfing/ofxUndo)  
* [ofxImGui](https://github.com/Daandelange/ofxImGui/). Fork from @**Daandelange**.  
* [ofxImGuiSurfing](https://github.com/moebiussurfing/ofxImGuiSurfing/) [WIP] 
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  

## Tested Systems
* **Windows 10** / **VS 2017** / **OF ~0.11**

## Author
An add-on by **@moebiusSurfing**  
*( ManuMolina ) 2021*  

## License
[**MIT License**](https://github.com/LICENSE)