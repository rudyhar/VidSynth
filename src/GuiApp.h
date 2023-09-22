#pragma once

#include "ofMain.h"
//#include "ofxDatGui.h"
#include "ofxImGui.h"
class GuiApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    //gui variables
    ofxImGui::Gui gui;
    ImGuiStyle my_style;
    ImVec4 backgroundColor;
 
    //ndi
    int ndi_scale=0;
   
    //global
    int source_select=1;

    
   };
