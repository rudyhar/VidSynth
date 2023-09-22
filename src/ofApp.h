#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxMidi.h"
#include "GuiApp.h"
#include "ofxNDIReceiver.h"
#include "ofxNDIRecvStream.h"
#define NUM_VIDEOS 4


class ofApp : public ofBaseApp, public ofxMidiListener{

    public:

        ofShader shader1;
        ofShader camShader;
        ofFbo framebuffer0;
        ofFbo framebuffer1;
        ofFbo ndi_fbo;  
//        ofVideoGrabber webcam;

       ofVideoPlayer videoName[NUM_VIDEOS];


        // GUI stuff
        ofxPanel gui;
        ofxFloatSlider aaSlider;
        ofxFloatSlider sxSlider;
        ofxIntSlider ddSlider;
        ofxFloatSlider ffSlider;
        ofxFloatSlider ggSlider;
        ofxFloatSlider hhSlider;
        ofxFloatSlider jjSlider;
        ofxFloatSlider volumeSlider; // audio

    
        ofxFloatSlider videoPositionSlider;
    
    
        // MIDI THINGS
        ofxMidiIn midiIn;
        ofxMidiOut midiOut;
    
        //ndi biz
        ofxNDIReceiver ndi_receiver_;
        ofxNDIRecvVideoFrameSync ndi_video_;
        ofPixels ndi_pixels;

        shared_ptr<GuiApp> imgui;

    
        


        void setup();
        void update();
        void draw();
    
        void exit();
        void newMidiMessage(ofxMidiMessage& msg);


        void allocateAndDeclareSundries();
        void inputSetup();
        void inputUpdate();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);

        void aaChanged(float &value);
        void sxChanged(float &value);
        void ddChanged(int &value);
        void ffChanged(float &value);
        void ggChanged(float &value);
        void hhChanged(float &value);
        void jjChanged(float &value);

        void midi13Changed(float &value);
        void midi14Changed(float &value);
        void delay_zChanged(float &value);

        void scrubVideo(float & value);
        void gotMessage(ofMessage msg);
        void playVideo(int vid);
        void drawBorder();
    
        void NDI_setup();
        void NDI_update();

    
    
    void volumeChanged(float &value);
    
    vector <ofxMidiMessage> messages;



            
};
