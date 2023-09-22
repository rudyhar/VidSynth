//
//  GuiApp.cpp
//  VidSynth5
//
//  Created by Rudy Harricks on 21/9/2023.
//


#include "GuiApp.h"




void GuiApp::setup(){
    
    ofBackground(0);
    //floatValue=0.0;
    
    gui.setup();
    
}
  //----------------------------------
    void GuiApp::update() {
       
    }


//------------------------------
    void GuiApp::draw() {
        
        float osc_pm_range=100;
        
        
        
        auto mainSettings = ofxImGui::Settings();
        
        //ImGui::StyleColorsAndrei();
        
        ofSetBackgroundColor(backgroundColor);
        
        gui.begin();
       
        
        int gui_hscaler=170;
        int gui_vscaler=80;
        
        
        
        ImGui::SetNextWindowPos(ImVec2(0*gui_hscaler, 0*gui_vscaler), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(gui_hscaler, gui_vscaler), ImGuiCond_FirstUseEver);
        
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        
        
        mainSettings.windowPos=ImVec2(ImVec2(0*gui_hscaler, 0*gui_vscaler));
        
        
        
        
        
        
        
        if (ofxImGui::BeginWindow("imGUI", mainSettings, false))
        {
            //ImGui::Text("nothin happening here yet");
            if (ImGui::CollapsingHeader("parameters")){
                const float spacing = 10;
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 5));
                ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 200.0f);
                //ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(200, 0));
                //input select
                //ImGui::PushItemWidth(20);
//                ImGui::Checkbox("coolswitch", &cool_swtch );
                
                ImGui::Text("select Source ");
                const char* items[] = { "cam1","ndi","cam2?","internal feedback" };
                static int item_current = 0;
                ImGui::Combo("combo2", &item_current, items, IM_ARRAYSIZE(items));
                if(item_current==0){source_select=0;}
                if(item_current==1){source_select=1;}
//                if(item_current==2){source_select=2;}
//                if(item_current==3){source_select=3;}
//                ImGui::SliderFloat("texmod scale", &osc0_texmod_scale, -1, 1);
                ImGui::SliderInt("ndi scale", &ndi_scale, -400, 400);

                
//                if (ImGui::CollapsingHeader("basic")){
//                    const char* items[] = { "sin","square", "saw","ramp","triangle","parabolic","cubic","quartic", "quintic" };
//                    static int item_current = 0;
//                    ImGui::Combo("osc0 waveshape", &item_current, items, IM_ARRAYSIZE(items));
//                    if(item_current==0){gui_osc0_shape=0;}
//                    if(item_current==1){gui_osc0_shape=1;}
//
//                    ImGui::SliderFloat("rate", &gui_osc0_rate, -1,1);
//
//                    if(texmod_osc0_switch==false){
//                        ImGui::SliderFloat("frequency", &gui_osc0_frequency, 0,1);
//                        ImGui::SliderFloat("amp", &gui_osc0_amp, 0, 1);
//                        ImGui::SliderFloat("osc width", &gui_osc0_width, -1, 1);
//                        ImGui::SliderFloat("angle", &gui_osc0_angle, -1, 1);
//                        ImGui::SliderFloat("radial", &gui_osc0_spiral, -1, 1);
//                    }
//
//                    if(texmod_osc0_switch==true){
//                        ImGui::SliderFloat("texmod frequency", &texmod_gui_osc0_frequency, -1,1);
//                        ImGui::SliderFloat("texmod amp", &texmod_gui_osc0_amp, -1, 1);
//                        ImGui::SliderFloat("texmod osc width", &texmod_gui_osc0_width, -1, 1);
//                        ImGui::SliderFloat("texmod angle", &texmod_gui_osc0_angle, -1, 1);
//                        ImGui::SliderFloat("texmod radial", &texmod_gui_osc0_spiral, -1, 1);
//                    }
//
//                    ImGui::SliderFloat("rotate", &gui_osc0_rotate, -1, 1);
//                }

                ImGui::PopStyleVar(2);
            }
            ImGui::Separator();
            
            
//            if (ImGui::CollapsingHeader("L F O s")){
//                ImGui::Text("Look For Originality");
//                
//                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 5));
//                ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 200.0f);
//                
//                if (ImGui::CollapsingHeader("lfo basic")){
//                    ImGui::SliderFloat("rate rate (lol)", &gui_osc0_rate_theta, 0, 20);
//                                }
//            ImGui::PopStyleVar(2);
//            }
            
        }//endImguiWindow
        ofxImGui::EndWindow(mainSettings);
      
        
        
    
        gui.end();
    }


//---------------------------
