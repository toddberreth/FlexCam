#include "flexcamApp.h"

//--------------------------------------------------------------
void flexcamApp::drawImage()
{
    ofSetColor(255);
    if (bShowLabels) dataImage.draw(INFO_WINDOW_WIDTH,0);
    else if (currentData.stitchedDisplayMode == 's') stitchedGrayImage.draw(INFO_WINDOW_WIDTH,0);
    else if (currentData.stitchedDisplayMode == 'd') differenceGrayImage.draw(INFO_WINDOW_WIDTH,0);
    else if (currentData.stitchedDisplayMode == 't') thresholdGrayImage.draw(INFO_WINDOW_WIDTH,0);
}

//--------------------------------------------------------------
void flexcamApp::drawInfo()
{
    ofPushMatrix();
    ofSetColor(255);
    ofFill();
    ofTranslate(0, 0, -1);
    ofRect(0,0,INFO_WINDOW_WIDTH,windowHeight);
    ofNoFill();
    ofPopMatrix();
    
    ofPushMatrix();
    string flexcamTitle = "FLEXCAM";
    ofRectangle bounds = verdana24.getStringBoundingBox(flexcamTitle, 0, 0);
    ofTranslate(INFO_WINDOW_WIDTH/2 + 2, 20, 0);
    ofSetColor(0,0,0,224); verdana24.drawString(flexcamTitle, -bounds.width/2, bounds.height/2);
    ofSetColor(0,0,0,16); verdana24.drawString(flexcamTitle, -bounds.width/2+2, bounds.height/2+2);
    ofSetColor(255);
    ofPopMatrix();
    
    ofPushMatrix();
    ofSetRectMode(OF_RECTMODE_CENTER);
    configImage.draw(INFO_WINDOW_WIDTH/2, INFO_WINDOW_HEIGHT/2 + 10);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofPopMatrix();

    if ((state != APP_WAITING_FOR_CONFIG) && (state != APP_CONFIGURE_LAYOUT)){
        
        glPushMatrix();
        ofSetColor(0,0,0,224);
        veramono10.drawString("RUN MODE", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT);
        veramono10.drawString("<c>onfig mode", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE);
        
        veramono10.drawString("<s>titched", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*3);
        if (currentData.stitchedDisplayMode == 's') veramono10.drawString("*", INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*3);
        veramono10.drawString("<d>ifference", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*4);
        if (currentData.stitchedDisplayMode == 'd') veramono10.drawString("*", INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*4);
        veramono10.drawString("<t>hreshold", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*5);
        if (currentData.stitchedDisplayMode == 't') veramono10.drawString("*", INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*5);
        veramono10.drawString("<a>ll data", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*7);
        if (bShowLabels) veramono10.drawString("ON", INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*7);
        else veramono10.drawString("OFF", INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*7);
        veramono10.drawString("e<x>clude from ROI", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*8);
        if (bCropROI) veramono10.drawString("ON", INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*8);
        else veramono10.drawString("OFF", INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*8);
        
        veramono10.drawString("<b>kgrd image", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*10);
        veramono10.drawString("<B>kgrding (auto)", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*11);
        if (currentData.bAutoBackgrounding) veramono10.drawString("ON", INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*11);
        else veramono10.drawString("OFF", INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*11);
        veramono10.drawString("<e>xport config", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*12);
        
        veramono10.drawString("threshold <-+>", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*14);
        veramono10.drawString(ofToString(currentData.threshold),  INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*14);
        veramono10.drawString("blur <ui>", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*15);
        veramono10.drawString(ofToString(currentData.blur),  INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*15);
        veramono10.drawString("dilate <op>", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*16);
        veramono10.drawString(ofToString(currentData.dilate),  INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*16);
        veramono10.drawString("erode <[]>", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*17);
        veramono10.drawString(ofToString(currentData.erode),  INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*17);
        
        veramono10.drawString("blobminarea <hj>", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*19);
        veramono10.drawString(ofToString(currentData.blobMinArea),INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*19);
        veramono10.drawString("blobmaxarea <kl>", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*20);
        veramono10.drawString(ofToString(currentData.blobMaxArea), INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*20);
        veramono10.drawString("blobsmooth <nm>", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*21);
        veramono10.drawString(ofToString(currentData.blobSmoothing, 3), INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*21);
        
        if ((currentData.configType == "ipcameras")&&(currentData.IPCameras.size() > 0)){
            veramono10.drawString("sho<w> ipcam info", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*23);
            if (bShowIPCamerasInfo) veramono10.drawString("ON", INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*23);
            else veramono10.drawString("OFF", INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*23);
        }
        
        ofSetColor(255);
        glPopMatrix();
    }
    else if (state == APP_CONFIGURE_LAYOUT){
        
        glPushMatrix();
        ofSetColor(0,0,0,224);
        veramono10.drawString("CONFIG MODE", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT);
        veramono10.drawString("<r>un mode", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE);
        veramono10.drawString("objects", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*3);
        veramono10.drawString(ofToString(currentData.numberObjects), INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*3);
        veramono10.drawString("selected (#)", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*4);
        veramono10.drawString(ofToString(currentData.activeObject+1), INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*4);
        
        veramono10.drawString("<m>ove object", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*6);
        if (bConfigMoveObject) veramono10.drawString("ON", INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*6);
        else veramono10.drawString("OFF", INFO_WINDOW_WIDTH-INFO_TEXT_RIGHT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*6);
        veramono10.drawString("<e>xport config", INFO_TEXT_LEFT_MARGIN, INFO_WINDOW_HEIGHT+INFO_TEXT_VERT_SPACE*7);
        ofSetColor(255);
        glPopMatrix();
    }
}

//--------------------------------------------------------------
void flexcamApp::drawIPCamerasInfo()
{
    ofPushMatrix();
    int row = 0;
    int col = 0;
    
    int x = 0;
    int y = 0;
    
    int w = currentData.canvasWidth / 3;
    int h = ofGetWindowHeight() / 3;
    
    float totalKbps = 0;
    float totalFPS = 0;

    for(int i = 0; i < IPCameraGrabbers.size(); i++)
    {
        x = col * w;
        y = row * h;
        col = (col + 1) % 3;
        if(col == 0)
        {row = (row + 1) % 3;}
        
        ofPushMatrix();
        ofTranslate(INFO_WINDOW_WIDTH + x, y);
        ofEnableAlphaBlending();
        
        float kbps = IPCameraGrabbers[i]->getBitRate() / 1000.0f;
        totalKbps+=kbps;
        
        float fps = IPCameraGrabbers[i]->getFrameRate();
        totalFPS+=fps;
        
        std::stringstream ss;
        
        ss << "          NAME: " << IPCameraGrabbers[i]->getCameraName() << endl;
        ss << "          HOST: " << IPCameraGrabbers[i]->getHost() << endl;
        ss << "           FPS: " << ofToString(fps,  2) << endl;
        ss << "          Kb/S: " << ofToString(kbps, 2) << endl;
        ss << " #Bytes Recv'd: " << ofToString(IPCameraGrabbers[i]->getNumBytesReceived(),  0) << endl;
        ss << "#Frames Recv'd: " << ofToString(IPCameraGrabbers[i]->getNumFramesReceived(), 0) << endl;
        ss << "Auto Reconnect: " << (IPCameraGrabbers[i]->getAutoReconnect() ? "YES" : "NO") << endl;
        ss << " Needs Connect: " << (IPCameraGrabbers[i]->getNeedsReconnect() ? "YES" : "NO") << endl;
        ss << "Time Till Next: " << IPCameraGrabbers[i]->getTimeTillNextAutoRetry() << " ms" << endl;
        ss << "Num Reconnects: " << ofToString(IPCameraGrabbers[i]->getReconnectCount()) << endl;
        ss << "Max Reconnects: " << ofToString(IPCameraGrabbers[i]->getMaxReconnects()) << endl;
        ss << "  Connect Fail: " << (IPCameraGrabbers[i]->hasConnectionFailed() ? "YES" : "NO");
        
        ofSetColor(255,0,0);
        ofDrawBitmapString(ss.str(), 10, 20);
        ofDisableAlphaBlending();
        ofPopMatrix();
        ofPopMatrix();
    }
    
    // keep track of some totals
    float avgFPS = totalFPS / IPCameraGrabbers.size();
    float avgKbps = totalKbps / IPCameraGrabbers.size();
    
    ofEnableAlphaBlending();
    ofSetColor(255);
    ofDrawBitmapString(" AVG FPS: " + ofToString(avgFPS,2), ofGetWindowWidth()-150,ofGetWindowHeight()-45);
    ofDrawBitmapString("AVG Kb/S: " + ofToString(avgKbps,2), ofGetWindowWidth()-150,ofGetWindowHeight()-30);
    ofDrawBitmapString("TOT Kb/S: " + ofToString(totalKbps,2), ofGetWindowWidth()-150,ofGetWindowHeight()-15);
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void flexcamApp::drawConfigDisplay()
{
    ofEnableAlphaBlending();
    
    if (currentData.configType == "videos"){
        
        for (int i = 0; i < videos.size(); i++){
            ofPushMatrix();
            ofTranslate(INFO_WINDOW_WIDTH, 0, 0);
            if (i == currentData.activeObject) {ofSetColor(255, 255, 255, 160); ofTranslate(0, 0, 2);}
            else ofSetColor(255, 255, 255, 64);
            videos[i].draw(currentData.videos[i].videoPosX,currentData.videos[i].videoPosY);
            if (i == currentData.activeObject) {
                ofSetColor(0, 255, 0, 160);
                ofTranslate(0, 0, 2);
                veramono10.drawString(ofToString(currentData.activeObject + 1),
                                      currentData.videos[i].videoPosX + currentData.videos[i].videoRoiMinX + 6,
                                      currentData.videos[i].videoPosY + currentData.videos[i].videoRoiMinY + 12);
            }
            else {ofSetColor(0, 255, 0, 64); ofTranslate(0, 0, 1);}
            ofNoFill();
            ofRect(currentData.videos[i].videoPosX + currentData.videos[i].videoRoiMinX,
                   currentData.videos[i].videoPosY + currentData.videos[i].videoRoiMinY,
                   (currentData.videos[i].videoPosX + currentData.videos[i].videoRoiMaxX) - (currentData.videos[i].videoPosX + currentData.videos[i].videoRoiMinX),
                   (currentData.videos[i].videoPosY + currentData.videos[i].videoRoiMaxY) - (currentData.videos[i].videoPosY + currentData.videos[i].videoRoiMinY));
            ofFill();
            ofPopMatrix();
        }
    }
    
    if (currentData.configType == "ipcameras"){
        
        for (int i = 0; i < IPCameras.size(); i++){
            ofPushMatrix();
            ofTranslate(INFO_WINDOW_WIDTH, 0, 0);
            if (i == currentData.activeObject) {ofSetColor(255, 255, 255, 160); ofTranslate(0, 0, 2);}
            else ofSetColor(255, 255, 255, 64);
            IPCameras[i].getCurrentFrame().draw(currentData.IPCameras[i].camPosX,currentData.IPCameras[i].camPosY);
            if (i == currentData.activeObject) {
                ofSetColor(0, 255, 0, 160);
                ofTranslate(0, 0, 2);
                veramono10.drawString(ofToString(currentData.activeObject + 1),
                                      currentData.IPCameras[i].camPosX + currentData.IPCameras[i].camRoiMinX + 6,
                                      currentData.IPCameras[i].camPosY + currentData.IPCameras[i].camRoiMinY + 12);
            }
            else {ofSetColor(0, 255, 0, 64); ofTranslate(0, 0, 1);}
            ofNoFill();
            ofRect(currentData.IPCameras[i].camPosX + currentData.IPCameras[i].camRoiMinX,
                   currentData.IPCameras[i].camPosY + currentData.IPCameras[i].camRoiMinY,
                   (currentData.IPCameras[i].camPosX + currentData.IPCameras[i].camRoiMaxX) - (currentData.IPCameras[i].camPosX + currentData.IPCameras[i].camRoiMinX),
                   (currentData.IPCameras[i].camPosY + currentData.IPCameras[i].camRoiMaxY) - (currentData.IPCameras[i].camPosY + currentData.IPCameras[i].camRoiMinY));
            ofFill();
            ofPopMatrix();
        }
    }
    
    if (currentData.configType == "localcamera"){
        
        ofPushMatrix();
        ofTranslate(INFO_WINDOW_WIDTH, 0, 0);
        ofSetColor(255, 255, 255, 160);
        localCamera.draw(currentData.localCamera.localPosX,currentData.localCamera.localPosY);
        ofSetColor(0, 255, 0, 160); ofTranslate(0, 0, 1);
        veramono10.drawString(ofToString(currentData.activeObject + 1),
                              currentData.localCamera.localPosX + currentData.localCamera.localRoiMinX + 6,
                              currentData.localCamera.localPosY + currentData.localCamera.localRoiMinY + 12);
        ofNoFill();
        ofRect(currentData.localCamera.localPosX + currentData.localCamera.localRoiMinX,
               currentData.localCamera.localPosY + currentData.localCamera.localRoiMinY,
               (currentData.localCamera.localPosX + currentData.localCamera.localRoiMaxX) - (currentData.localCamera.localPosX + currentData.localCamera.localRoiMinX),
               (currentData.localCamera.localPosY + currentData.localCamera.localRoiMaxY) - (currentData.localCamera.localPosY + currentData.localCamera.localRoiMinY));
        ofFill();
        ofPopMatrix();
    }
    
    ofSetColor(255);
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void flexcamApp::drawRunROI()
{
    ofPushMatrix();
    ofTranslate(INFO_WINDOW_WIDTH, 0, 0);
    ofSetColor(0, 255, 0);
    veramono10.drawString("(0,0)", currentData.runROIMinX, currentData.runROIMinY+10);
    veramono10.drawString("(1,1)", currentData.runROIMaxX-30, currentData.runROIMaxY-2);
    ofNoFill();
    ofRect(currentData.runROIMinX, currentData.runROIMinY, currentData.runROIMaxX - currentData.runROIMinX, currentData.runROIMaxY - currentData.runROIMinY);
    ofFill();
    ofSetColor(255);
    ofPopMatrix();
}