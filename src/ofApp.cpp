#include "ofApp.h"
#include "iostream"




//char video_path[] = "/Users/rudyharricks/Documents/VFX/final-vids/vidart1-FINAL/ForUnderFeedback.mov";
char video_path[] = "/Users/rudyharricks/Documents/VFX/final-vids/FALLIN2PLACE/fallin2place-ROUGH-CUT-7.mp4";
//char video_path[] = "/Users/rudyharricks/Documents/GRAPHICS/outputs/platfieldFOrSYBNTH2.mov";

int width=1920;
int height=1440;

int vid_width;
int vid_height;

// need to change this every time replugged in
int port_id = 1;

float az = 1.0;
float sx = .5;
float dc = 0;
int dd = 0;
float time1;
const int fbob = 15;

// lumavalue
float ff=0.0;
// luma threshold
float gg=0.0;
// key colour
float hh=0.0;
// key threshold
float jj=0.1;

//delayscale
float aa=1.0;


float red = 1.0;
float green = 1.0;
float blue = 1.0;

float hue = 1.0;
float saturation = 1.0;
float brightness = 1.0;

float camscaler = 1.0;
float pos_scaler = 1.0;
float centre_scale = 1.0;
float rotater = 0.0;

float midi12 = 1.0;
float midi13 = 0.0;
float midi14 = 0.0;

bool second_vid_playing = false;



ofFbo framebufferDelay[fbob];
ofFbo framebufferDelayNDI[fbob];

//--------------------------------------------------------------



void ofApp::setup(){
    
//    imgui = std::make_shared<GuiApp>(); // Initialize imgui to point to a GuiApp object
//    imgui->setup(); // Call the setup method of GuiApp object if needed

    // //setup gui
    NDI_setup();
    gui.setup();
   // Starting value, min, and max
    
    gui.add(sxSlider.setup("sx MIX", 0.5, -10.0, 10.0));
    gui.add(ddSlider.setup("dd DELAY AMOUNT", 0, -fbob, fbob));
    
    gui.add(ffSlider.setup("ff LUMA VAL", 0.1, 0, 1.0));
    gui.add(ggSlider.setup("gg LUMA THRESH", 0.1, 0.0, 1.0));

    gui.add(hhSlider.setup("hh  KEY COLOUR", 1.0, 0.0, 1.0));
    gui.add(jjSlider.setup("jj  KEY THRESHOLD", 0.1, 0.0, 1.0));
    
    
    

    gui.add(aaSlider.setup("aa DELAY SCALE", 0.0, -10.0, 10.0));
    gui.add(videoPositionSlider.setup("Video Position", 0.0, 0.0, 1.0));
    gui.add(volumeSlider.setup("Volume", 1.0, 0.0, 1.0));


    
    // // Add listeners to your sliders
    sxSlider.addListener(this, &ofApp::sxChanged);
    ddSlider.addListener(this, &ofApp::ddChanged);
    ffSlider.addListener(this, &ofApp::ffChanged);
    ggSlider.addListener(this, &ofApp::ggChanged);
    hhSlider.addListener(this, &ofApp::hhChanged);
    aaSlider.addListener(this, &ofApp::aaChanged);
    jjSlider.addListener(this, &ofApp::jjChanged);
    videoPositionSlider.addListener(this, &ofApp::scrubVideo);
    volumeSlider.addListener(this, &ofApp::volumeChanged);



    // //_______________________________________________________________________________________

    ofSetVerticalSync(true);
    ofSetFrameRate(30);
    ofBackground(0);
//    ofHideCursor();
    
//    webcam.setup(width,height);

    
    // load in videos
    videoName[0].load(video_path);
    videoName[1].load("/Users/rudyharricks/Documents/VFX/final-vids/FALLIN2PLACE/fallin2place-ROUGH-CUT-7.mp4");

    videoName[0].play();

    vid_width = videoName[0].getWidth();
    vid_height = videoName[0].getHeight();

    allocateAndDeclareSundries();

    shader1.load("shadersES2/shader1");
    camShader.load("camShader/shader1");

    for (int i=0;i<fbob;i++){
        framebufferDelay[i].allocate(vid_width,vid_height);
        framebufferDelay[i].begin();
        ofClear(0,0,0,255);
        framebufferDelay[i].end();
    }
    
    for (int i=0;i<fbob;i++){
        framebufferDelayNDI[i].allocate(vid_width,vid_height);
        framebufferDelayNDI[i].begin();
        ofClear(0,0,0,255);
        framebufferDelayNDI[i].end();
    }

    
    // list all available midi devices
    midiIn.listInPorts();
    midiOut.listOutPorts();
    
    
    // open a midi port by number (you may need to change this)
    midiIn.openPort(port_id);
//    midiOut.openPort(port_id);
    
    // optionally add ofApp as a listener for incoming messages
    midiIn.addListener(this);
    
    videoName[0].setVolume(0);

}
//------------------------------------------------------------
void ofApp::allocateAndDeclareSundries(){
    framebuffer0.allocate(vid_width,vid_height);
    framebuffer0.begin();
    ofClear(0,0,0,255);
    framebuffer0.end();
    framebuffer1.allocate(vid_width,vid_height);
    framebuffer1.begin();
    ofClear(0,0,0,255);
    framebuffer1.end();
    ndi_fbo.allocate(width,height);
    ndi_fbo.begin();
    ofClear(0,0,0,255);
    ndi_fbo.end();

}
//--------------------------------------------------------------
//void ofApp::inputSetup(){
//
//    webcam.setDesiredFrameRate(30);
//    webcam.initGrabber(width,height);
//}
//--------------------------------------------------------------
void ofApp::update(){
    time1 += 0.01;
    videoName[0].update();
    NDI_update();

    // webcam.update(); // grab another frame from the camera

    if(second_vid_playing){
        videoName[1].update();
    }
    

}

//--------------------------------------------------------------
void ofApp::draw(){



    
    framebuffer0.begin();  // Start drawing to the framebuffer
    ofClear(0, 0, 0, 255);  // Clear the framebuffer
    // shader1.begin();  // Begin the shader

    // shader1.setUniform1f("sx",sx);

    camShader.begin();

    camShader.setUniform1f("lumavalue", ff);
    
    camShader.setUniform1f("lumathreshold", gg);

    camShader.setUniform1f("keycolour", hh);

    camShader.setUniform1f("keythreshold", jj);
    
    camShader.setUniform1f("delayscale",aa);

    camShader.setUniform1f("camscaler",camscaler);

    camShader.setUniform1f("pos_scaler",pos_scaler);

    
    
    
    camShader.setUniform3f("rgb_multiplier", red, green, blue);
    camShader.setUniform3f("hsv_multiplier", hue, saturation, brightness);

    
    
    
    
    
    // DO A CHECK FOR VIDEO OR NDI SOURCE
//    switch(imgui->source_select){
//        case 0:
//            camShader.setUniformTexture("video_feed",videoName[0].getTexture(),0);
//            break;
//        case 1:
//            camShader.setUniformTexture("video_feed",ndi_fbo.getTexture(),0);
//            break;
//    }
    // camShader.setUniformTexture("framebufferdelay",az);

    if(imgui->source_select == 0){
        camShader.setUniformTexture("video_feed",videoName[0].getTexture(),0);
        camShader.setUniformTexture("framebufferdelay",framebufferDelay[abs(dd)%fbob].getTexture(),1);
    }
    if(imgui->source_select == 1){
        camShader.setUniformTexture("video_feed",ndi_fbo.getTexture(),0);
        camShader.setUniformTexture("framebufferdelay",framebufferDelayNDI[abs(dd)%fbob].getTexture(),1);
    }
    
    
    camShader.setUniform2f("u_resolution",width,height);
    camShader.setUniform1f("mixx",sx);
    
    
    // camShader.setUniformTexture("framebufferdelay", framebufferDelay[5], 1);
    
    camShader.setUniform1f("midi12", midi12);
    
    camShader.setUniform1f("midi13", midi13);

    camShader.setUniform1f("midi14", midi14);


    camShader.setUniform1f("time", time1);
    ofSetRectMode(OF_RECTMODE_CENTER); //set rectangle mode to the center
    ofDrawRectangle(ofGetWidth()/2, ofGetHeight()/2 , ofGetWidth(), ofGetHeight());
    
    // HERE WE DRAW OTHER layers
    
     if(second_vid_playing){
        camShader.setUniformTexture("video_feed",videoName[1].getTexture(),0);
        ofDrawRectangle(ofGetWidth()/2, ofGetHeight()/2 , ofGetWidth(), ofGetHeight());
     }
    
    
   
    // webcam.draw(0,0,1024,768);
    camShader.end();

    framebuffer0.end();
    
    framebuffer0.setAnchorPoint(0, 0);

    
    if(imgui->source_select == 0){
        framebuffer0.draw(0,0,vid_width,vid_height); // sends frame to tex 0 on shader
    }
    if(imgui->source_select == 1){
        ndi_fbo.draw(startX, startY, newWidth, newHeight);
    }

    if(imgui->source_select == 0){
        framebufferDelay[0].begin();
    }


    // Calculate new width and height after scaling
    float newWidth = vid_width * centre_scale;
    float newHeight = vid_height *  centre_scale;

    // Calculate starting X and Y positions to draw the FBO centered
    float startX = (vid_width - newWidth) / 2.0;
    float startY = (vid_height - newHeight) / 2.0;

    // Draw the FBO
    if(imgui->source_select == 0){
        framebuffer0.draw(startX, startY, newWidth, newHeight);
    }
    if(imgui->source_select == 1){
        ndi_fbo.draw(startX, startY, newWidth, newHeight);
    }
    

    framebuffer0.setAnchorPoint(vid_width/2, vid_height/2);


    float fboCenterX = vid_width / 2;
    float fboCenterY = vid_height / 2;

    // Translate to the center of the FBO
    ofTranslate(fboCenterX, fboCenterY, 0);
    
    // Apply the scaling
    ofScale(centre_scale, centre_scale, centre_scale);
//    ofRotateDeg(45, 0, 0, 1);

    // Translate back by the negative offset of the FBO's center
    framebuffer0.draw(0, 0);
    
//    ofRotateDeg(-45, 0, 0, 1);


    ofTranslate(-fboCenterX, -fboCenterY, 0);

    // Draw the FBO

    if(imgui->source_select == 0){
        framebufferDelay[0].end();

        for (int i = fbob-1; i > 0; i--) {
            framebufferDelay[i].begin();
            framebufferDelay[i-1].draw(0, 0, vid_width, vid_height); // calls draw on every frame delay so eah are drawn
            framebufferDelay[i].end();
        }

        framebufferDelayNDI[0].end();
    }

    if(imgui->source_select == 1){
        for (int i = fbob-1; i > 0; i--) {
            framebufferDelayNDI[i].begin();
            framebufferDelayNDI[i-1].draw(0, 0, vid_width, vid_height); // calls draw on every frame delay so eah are drawn
            framebufferDelayNDI[i].end();
        }
    }

    // videoName.draw(0,0,1024,768);
    // ofDrawRectangle(64,64, 64, 64);

    ofSetHexColor(0xffffff);
    ofDrawBitmapString("fps: " +ofToString(ofGetFrameRate()) + "\n" +
    "to mix between cam and framebuffer delay press 's' and 'x' \n"
    "delay time = 'd' and 'c' \n"
    "luma value = 'f' and 'v' \n"
    "luma threshold = 'g' and 'b' \n"
    "key colour = 'h' and 'n' \n"
    "key threshold = 'j' and 'm' \n"
    "delay scale = 'a' and 'z' \n"
    
    
    , 8, ofGetHeight()-30);
    
    // ofDrawBitmapString("Volume: " + ofToString(videoName.getVolume()), 8, ofGetHeight() - 60);


    gui.draw();


    

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
        //here is how i map controls from the keyboard
    
    //increment and decrement sx
    if (key == 's') {sx += .1;     printf("%f\n",sx);}
    if (key == 'x') {sx -= .1;     printf("%f\n",sx);}

    
    //increment and decrement dd
    if (key == 'd') {dd += 1;       printf("%d\n",dd);}
    if (key == 'c') {dd -= 1;       printf("%d\n",dd);}

    if(key=='a'){aa+=0.1;}
    if(key=='z'){aa-=0.1;}

    if(key=='f'){ff+=0.1;}
    if(key=='v'){ff-=0.1;}

    if(key=='g'){gg+=0.1;}
    if(key=='b'){gg-=0.1;}
    
    if(key=='h'){hh+=0.01;}
    if(key=='n'){hh-=0.01;}

    if(key=='j'){jj+=0.01;}
    if(key=='m'){jj-=0.01;}

    if(key==';'){
        centre_scale+=0.1;
        printf("centre_scale: %f\n", centre_scale);
    }
    if(key=='/'){
        centre_scale-=0.1;
        printf("centre_scale: %f\n", centre_scale);
    }

    
    

    if (key == ',') {
        float frameLength = 1.0 / videoName[0].getTotalNumFrames();
        float newPosition = videoName[0].getPosition() - (15 * frameLength);
        videoName[0].setPosition(ofClamp(newPosition, 0.0, 1.0));
    }

    if (key == '.') {
        float frameLength = 1.0 / videoName[0].getTotalNumFrames();
        float newPosition = videoName[0].getPosition() + (15 * frameLength);
        videoName[0].setPosition(ofClamp(newPosition, 0.0, 1.0));
    }
    if(key == 'p') {
        videoName[0].setPaused(!videoName[0].isPaused());
    }
    
    if (key == 'q') {
        framebuffer0.begin();
        ofClear(0,0,0,255);
        framebuffer0.end();

        
        for (int i = fbob-1; i > 0; i--) {
//            framebufferDelay[i].allocate( width,  height);
           framebufferDelay[i].begin();
            ofClear(0, 0, 0, 255);  // Clear the framebuffer
           framebufferDelay[i].end();
        }
       for (int i = fbob-1; i > 0; i--) {
//            framebufferDelay[i].allocate( width,  height);
           framebufferDelayNDI[i].begin();
            ofClear(0, 0, 0, 255);  // Clear the framebuffer
           framebufferDelayNDI[i].end();
       }

    }

      

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {
    // handle midi messages here
    // for example, print the received message to the console
//    cout << msg.toString() << endl;
    
    
    // get the control number and value of the message
    
    int control = msg.control;
    int value = msg.value;
    int pitch = msg.pitch;
    
    if(msg.status == MIDI_NOTE_ON) {
        
        
        printf("control: %d\n", control);
        printf("%d", msg.pitch);
//        printf("%d", msg.channel);


        // ΤOP ROW
        if(pitch == 48) {
            // SLIDER F7
            playVideo(1);
            second_vid_playing = true;
            printf("loaded new vid");
            printf("%d", msg.pitch);
        }
        if(pitch == 49) {
            second_vid_playing = false;
            printf("removed new vid");
        }
        if(pitch == 50) {

            
        }
        if(pitch == 51) {

        }
        
        // SECOND ROW
        if(pitch == 44) {
  
        }
        if(pitch == 45) {

        }
        if(pitch == 46) {

            
        }
        if(pitch == 47) {

            
        }
        
    }
    
    if(msg.status == MIDI_NOTE_OFF) {
        // ΤOP ROW
        if(pitch == 48) {
            second_vid_playing = false;
            printf("removed new vid");
            
        }
        if(pitch == 49) {
            
        }
        if(pitch == 50) {
            
        }
        if(pitch == 51) {
            
        }
        
        // SECOND ROW
        if(pitch == 44) {
            
        }
        if(pitch == 45) {
            
        }
        if(pitch == 46) {
            
        }
        if(pitch == 47) {
            
            
        }
    }
    
    
    
    
    
    
    
    

    else if(msg.status == MIDI_AFTERTOUCH) {
        // cant query the exact note this came from
        if (pitch == 48){
            printf("YEAH");
        }
    }

    
    
    
    
    
    if(msg.status == MIDI_CONTROL_CHANGE) {

        
        printf("control: %d\n", control);

        // check if the control number corresponds to a specific button
        if(control == 1) {
            // SLIDER F1
            red = ofMap(value, 0, 127, 1.0, 10.0);
        }
        if(control == 2) {
            // SLIDER F2
            green = ofMap(value, 0, 127, 1.0, 10.0);
        }
        if(control == 3) {
            // SLIDER F3
            blue = ofMap(value, 0, 127, 1.0, 10.0);
        }
        if(control == 4) {
            // SLIDER F4
            hue = ofMap(value, 0, 127, 1.0, 10.0);
        }
        if(control == 5) {
            // SLIDER F5
            saturation = ofMap(value, 0, 127, 1.0, 10.0);
        }
        if(control == 6) {
            // SLIDER F6
            brightness = ofMap(value, 0, 127, 1.0, 10.0);
        }
        if(control == 11) {
            // SLIDER F7
            aa = ofMap(value, 0, 127, -10.0, 10.0);
            printf("%f\n", aa);
        }
        if(control == 12) {
            // SLIDER F7

            midi12 = ofMap(value, 0, 127, 1.0, 2000.0 );
            printf("%f\n", midi12);
        }
        if(control == 13) {
            // SLIDER F7
            midi13 = ofMap(value, 0, 127, -200, 200.0);
            printf("%f\n", midi13);
        }
        if(control == 14) {
            // SLIDER F7
            midi14 = ofMap(value, 0, 127, -200.0, 200.0);
            printf("%f\n", midi14);
        }
        if(control == 15) {
            // SLIDER F7
            camscaler = ofMap(value, 0, 127, 0.0, 2.0);
            printf("%f\n", camscaler);
        }
        if(control == 16) {
            // SLIDER F7
            sx = ofMap(value, 0, 127, -10.0, 10.0);
            printf("%f\n", sx);
        }
    }
}
//--------------------------------------------------------------
void ofApp::aaChanged(float &value){
    aa = value;
    printf("%f\n", aa);
}
//--------------------------------------------------------------

void ofApp::sxChanged(float &value){
    sx = value;
    printf("%f\n", sx);
}
//--------------------------------------------------------------

void ofApp::ddChanged(int &value){
    dd = value;
    printf("%d\n", dd);
}
//--------------------------------------------------------------
void ofApp::ffChanged(float &value){
    ff = value;
    printf("%f\n", ff);
}
//--------------------------------------------------------------
void ofApp::ggChanged(float &value){
    gg = value;
    printf("%f\n", gg);
}
//--------------------------------------------------------------
void ofApp::hhChanged(float &value){
    hh = value;
    printf("%f\n", hh);
}
//--------------------------------------------------------------
void ofApp::jjChanged(float &value){
    jj = value;
    printf("%f\n", jj);
}

void ofApp::scrubVideo(float & value) {
    if(videoName[0].isLoaded()) {
        float newPosition = value * videoName[0].getDuration();
        videoName[0].setPosition(newPosition / videoName[0].getDuration());
    }
}


void ofApp::volumeChanged(float &value){
    videoName[0].setVolume(value);
}


void ofApp::exit(){
    midiIn.closePort();
    midiIn.removeListener(this);
}


void ofApp::playVideo(int vid){
//    videoName[1].load("/Users/rudyharricks/Documents/VFX/final-vids/vidart1-FINAL/finalBit2.mov");
//    videoName[1].load(video_path);
    videoName[vid].play();
}


//_____----_-_-_-______---__---_--_----____--_-__-_-

void ofApp::NDI_setup(){
    auto findSource = [](const string &name_or_url) {
        auto sources = ofxNDI::listSources();
        if(name_or_url == "") {
            return make_pair(ofxNDI::Source(), false);
        }
        auto found = find_if(begin(sources), end(sources), [name_or_url](const ofxNDI::Source &s) {
            return ofIsStringInString(s.p_ndi_name, name_or_url) || ofIsStringInString(s.p_url_address, name_or_url);
        });
        if(found == end(sources)) {
            ofLogWarning("ofxNDI") << "no NDI source found by string:" << name_or_url;
            return make_pair(ofxNDI::Source(), false);
        }
        return make_pair(*found, true);
    };
    string name_or_url = "";    // Specify name or address of expected NDI source. In case of blank or not found, receiver will grab default(which is found first) source.
    auto result = findSource(name_or_url);
    if(result.second ? ndi_receiver_.setup(result.first) : ndi_receiver_.setup()) {
        ndi_video_.setup(ndi_receiver_);
    }
    
}
//-------------------------

void ofApp::NDI_update(){
    if(ndi_receiver_.isConnected()) {
        ndi_video_.update();
        if(ndi_video_.isFrameNew()) {
            ndi_video_.decodeTo(ndi_pixels);
        }
    }
    ndi_fbo.begin();
    if(ndi_pixels.isAllocated()) {
        //ofImage ndi_image;
        ofPushMatrix();
        ofTranslate(ndi_fbo.getWidth()/2,ndi_fbo.getHeight()/2);
        ofTranslate(0,0,imgui->ndi_scale);
        ofImage(ndi_pixels).draw(-ofImage(ndi_pixels).getWidth()/2,-ofImage(ndi_pixels).getHeight()/2);
        //ofImage(ndi_pixels).draw(0,0);
        ofPopMatrix();
    }
    ndi_fbo.end();
    
    
    
}

