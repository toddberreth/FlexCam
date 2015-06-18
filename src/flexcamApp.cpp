#include "flexcamApp.h"

//--------------------------------------------------------------
void flexcamApp::setup()
{
    state = APP_JUST_LAUNCHED;
    
    ofSetFrameRate(APP_FRAMERATE);
    ofSetVerticalSync(TRUE);
    ofSetWindowPosition(INIT_POSITION_X, INIT_POSITION_Y);
    ofBackground(0,0,0);
    ofSetLogLevel(OF_LOG_VERBOSE);  //OF_LOG_SILENT
    
    windowWidth = INFO_WINDOW_WIDTH; windowHeight = INFO_WINDOW_HEIGHT;
    
    bProcessConfig = bLoadConfig = bLoadedConfig = bSaveConfig = bConfigDisplay = bCropROI = bLearnBackground = bConfigMoveObject = bShowLabels = bShowIPCamerasInfo = false;
    bShowFramerate = true;
    
    setupMedia();
    
    state = APP_WAITING_FOR_CONFIG;
    
    if (AUTOLOAD_CONFIG) autoLoadConfig(DEFAULT_CONFIG);
    
}

//--------------------------------------------------------------
void flexcamApp::update()
{
    ofSetWindowShape(windowWidth, windowHeight);
    ofSetWindowTitle(currentData.configName + "- fps: " + ofToString(ofGetFrameRate(),2));
    
    if (state != APP_WAITING_FOR_CONFIG){
        
        if (currentData.configType == "ipcameras")		updateIPCameras();
        if (currentData.configType == "localcamera")    updateLocalCamera();
        if (currentData.configType == "videos")			updateVideos();
        
        if (!bConfigDisplay){
            
            updateStitchedImage();
            updateProcessedImage();
            updateBlobTracking();
            updateOscSend();
            
            if (currentData.bAutoBackgrounding) updateAutoBackgrounding();
            if (currentData.configType == "videos")		state = APP_VIDEOS;
            if (currentData.configType == "ipcameras")	state = APP_IP_CAMERAS;
            if (currentData.configType == "local")		state = APP_LOCAL_CAMERA;
        }
        else{
            state = APP_CONFIGURE_LAYOUT;
        }

    }
    
    if (bProcessConfig) processConfigFile();
    if (bLoadConfig) loadConfig();
    if (bSaveConfig) saveConfig();
}

//--------------------------------------------------------------
void flexcamApp::draw()
{
    drawInfo();
    
    
    if (state != APP_WAITING_FOR_CONFIG){
        
        if (!bConfigDisplay) {
            
            drawImage();
            if (bShowLabels) drawRunROI();
            if (bShowIPCamerasInfo) drawIPCamerasInfo();
        }
        else drawConfigDisplay();
    }
}

//--------------------------------------------------------------
void flexcamApp::exit()
{
    if (bLoadedConfig) clearAllData();
}

//--------------------------------------------------------------
void flexcamApp::keyPressed(int key)
{
    
    
    if (bConfigDisplay){
        
        if (((int)key >= 49) && ((int)key <= 57)) {if (((int)key - 48)<= currentData.numberObjects) {currentData.activeObject = (int)key - 49;}}
        if ((currentData.activeObject + 1) <= currentData.numberObjects){
            switch (key){case 'm': bConfigMoveObject = !bConfigMoveObject; break;}
        }
    }
    else{
        switch (key){
                
            case 'b': bLearnBackground = true; break;									// learn background
            case 'x': bCropROI = !bCropROI; break;
            case '=': if (currentData.threshold >= currentData.thresholdMax) currentData.threshold = currentData.thresholdMax; else currentData.threshold++; break;
            case 'u': if (currentData.blur <= 0) currentData.blur = 0; else if (currentData.blur == 1) currentData.blur = currentData.blur - 1; else currentData.blur = currentData.blur - 2; break;
            case 'i': if (currentData.blur >= currentData.blurMax) currentData.blur = currentData.blurMax; else if (currentData.blur == 0) currentData.blur = currentData.blur + 1; else currentData.blur = currentData.blur + 2; break;
            case '-': if (currentData.threshold <= 0) currentData.threshold = 0; else currentData.threshold--; break;
            case 'p': if (currentData.dilate >= currentData.dilateMax) currentData.dilate = currentData.dilateMax; else currentData.dilate++; break;
            case 'o': if (currentData.dilate <= 0) currentData.dilate = 0; else currentData.dilate--; break;
            case ']': if (currentData.erode >= currentData.erodeMax) currentData.erode = currentData.erodeMax; else currentData.erode++; break;
            case '[': if (currentData.erode <= 0) currentData.erode = 0; else currentData.erode--; break;
            case 'j': if (currentData.blobMinArea >= currentData.blobMaxArea) currentData.blobMinArea = currentData.blobMaxArea; else currentData.blobMinArea =  currentData.blobMinArea + BLOB_AREA_STEP; break;
            case 'h': if (currentData.blobMinArea <= 0) currentData.blobMinArea = 0; else currentData.blobMinArea =  currentData.blobMinArea - BLOB_AREA_STEP; break;
            case 'l': if (currentData.blobMaxArea >= 1000000) currentData.blobMaxArea = 1000000; else currentData.blobMaxArea =  currentData.blobMaxArea + BLOB_AREA_STEP; break;
            case 'k': if (currentData.blobMaxArea <= currentData.blobMinArea) currentData.blobMaxArea = currentData.blobMinArea; else currentData.blobMaxArea =  currentData.blobMaxArea - BLOB_AREA_STEP; break;
            case 'n': currentData.blobSmoothing =  currentData.blobSmoothing - BLOB_SMOOTHING_STEP; if (currentData.blobSmoothing < 0) currentData.blobSmoothing = 0; break;
            case 'm': currentData.blobSmoothing =  currentData.blobSmoothing + BLOB_SMOOTHING_STEP; if (currentData.blobSmoothing > 1) currentData.blobSmoothing = 1; break;
            case 'B': currentData.bAutoBackgrounding = !currentData.bAutoBackgrounding; break;
                
            case 'w': if (currentData.configType == "ipcameras") bShowIPCamerasInfo = ! bShowIPCamerasInfo;
        }
    }
    
    switch (key){
            
        case 'r': bConfigDisplay = false; cvSet(stitchedGrayImage.getCvImage(), cvScalar(0,0,0)); break;
        case 'c': bConfigDisplay = true; break;
        case 's': currentData.stitchedDisplayMode = 's'; bConfigDisplay = false; break;									// color display
        case 'd': currentData.stitchedDisplayMode = 'd'; bConfigDisplay = false; break;									// difference display
        case 't': currentData.stitchedDisplayMode = 't'; bConfigDisplay = false; break;									// threshhold frame
        case 'a': bShowLabels = !bShowLabels; break;                                                                    // all data display
        case 'e': bSaveConfig = true; break;
    }
}
//--------------------------------------------------------------
void flexcamApp::keyReleased(int key){}
//--------------------------------------------------------------
void flexcamApp::mouseMoved(int x, int y ){}
//--------------------------------------------------------------
void flexcamApp::mouseDragged(int x, int y, int button)
{
    
    x = x - INFO_WINDOW_WIDTH;
    
    if (bConfigDisplay){
        
        if(bConfigMoveObject){
            
            if (currentData.configType == "videos"){
                
                if(((currentData.videos[currentData.activeObject].videoPosX+currentData.videos[currentData.activeObject].videoRoiMinX +(x-startMoveObjectX)) >= 0) &&
                   ((currentData.videos[currentData.activeObject].videoPosX+currentData.videos[currentData.activeObject].videoRoiMaxX+(x-startMoveObjectX))  <= (currentData.canvasWidth)))
                { currentData.videos[currentData.activeObject].videoPosX = currentData.videos[currentData.activeObject].videoPosX + (x-startMoveObjectX);}
                
                if(((currentData.videos[currentData.activeObject].videoPosY+currentData.videos[currentData.activeObject].videoRoiMinY+(y-startMoveObjectY)) >= 0) &&
                   ((currentData.videos[currentData.activeObject].videoPosY+currentData.videos[currentData.activeObject].videoRoiMaxY+(y-startMoveObjectY)) <= (currentData.canvasHeight)))
                { currentData.videos[currentData.activeObject].videoPosY = currentData.videos[currentData.activeObject].videoPosY + (y-startMoveObjectY);}
            }
            
            if (currentData.configType == "ipcameras"){
                
                if(((currentData.IPCameras[currentData.activeObject].camPosX+currentData.IPCameras[currentData.activeObject].camRoiMinX +(x-startMoveObjectX)) >= 0) &&
                   ((currentData.IPCameras[currentData.activeObject].camPosX+currentData.IPCameras[currentData.activeObject].camRoiMaxX+(x-startMoveObjectX))  <= (currentData.canvasWidth)))
                { currentData.IPCameras[currentData.activeObject].camPosX = currentData.IPCameras[currentData.activeObject].camPosX + (x-startMoveObjectX);}
                
                if(((currentData.IPCameras[currentData.activeObject].camPosY+currentData.IPCameras[currentData.activeObject].camRoiMinY+(y-startMoveObjectY)) >= 0) &&
                   ((currentData.IPCameras[currentData.activeObject].camPosY+currentData.IPCameras[currentData.activeObject].camRoiMaxY+(y-startMoveObjectY)) <= (currentData.canvasHeight)))
                { currentData.IPCameras[currentData.activeObject].camPosY = currentData.IPCameras[currentData.activeObject].camPosY + (y-startMoveObjectY);}
            }
            
            if (currentData.configType == "localcamera"){
                
                if(((currentData.localCamera.localPosX+currentData.localCamera.localRoiMinX +(x-startMoveObjectX)) >= 0) &&
                   ((currentData.localCamera.localPosX+currentData.localCamera.localRoiMaxX+(x-startMoveObjectX))  <= (currentData.canvasWidth)))
                { currentData.localCamera.localPosX = currentData.localCamera.localPosX + (x-startMoveObjectX);}
                
                if(((currentData.localCamera.localPosY+currentData.localCamera.localRoiMinY+(y-startMoveObjectY)) >= 0) &&
                   ((currentData.localCamera.localPosY+currentData.localCamera.localRoiMaxY+(y-startMoveObjectY)) <= (currentData.canvasHeight)))
                { currentData.localCamera.localPosY = currentData.localCamera.localPosY + (y-startMoveObjectY);}
            }
            
            startMoveObjectX = x; startMoveObjectY=y;
        }
        else{
            
            if (currentData.configType == "videos"){
                
                if (((x >= currentData.videos[currentData.activeObject].videoPosX+currentData.videos[currentData.activeObject].videoRoiMinX) &&
                     (x <= currentData.videos[currentData.activeObject].videoPosX+currentData.videos[currentData.activeObject].videoWidth))
                    && ((y >= currentData.videos[currentData.activeObject].videoPosY+currentData.videos[currentData.activeObject].videoRoiMinY) &&
                        (y <=currentData.videos[currentData.activeObject].videoPosY+currentData.videos[currentData.activeObject].videoHeight)))
                {
                    currentData.videos[currentData.activeObject].videoRoiMaxX  = (x - currentData.videos[currentData.activeObject].videoPosX);
                    currentData.videos[currentData.activeObject].videoRoiMaxY  = (y - currentData.videos[currentData.activeObject].videoPosY);
                }
            }
            
            if (currentData.configType == "ipcameras"){
                
                if (((x >= currentData.IPCameras[currentData.activeObject].camPosX+currentData.IPCameras[currentData.activeObject].camRoiMinX) &&
                     (x <= currentData.IPCameras[currentData.activeObject].camPosX+currentData.IPCameras[currentData.activeObject].camWidth))
                    && ((y >= currentData.IPCameras[currentData.activeObject].camPosY+currentData.IPCameras[currentData.activeObject].camRoiMinY) &&
                        (y <=currentData.IPCameras[currentData.activeObject].camPosY+currentData.IPCameras[currentData.activeObject].camHeight)))
                {
                    currentData.IPCameras[currentData.activeObject].camRoiMaxX  = (x - currentData.IPCameras[currentData.activeObject].camPosX);
                    currentData.IPCameras[currentData.activeObject].camRoiMaxY  = (y - currentData.IPCameras[currentData.activeObject].camPosY);
                }
            }
            
            if (currentData.configType == "localcamera"){
                
                if (((x >= currentData.localCamera.localPosX+currentData.localCamera.localRoiMinX) &&
                     (x <= currentData.localCamera.localPosX+currentData.localCamera.localWidth))
                    && ((y >= currentData.localCamera.localPosY+currentData.localCamera.localRoiMinY) &&
                        (y <=currentData.localCamera.localPosY+currentData.localCamera.localHeight)))
                {
                    currentData.localCamera.localRoiMaxX  = (x - currentData.localCamera.localPosX);
                    currentData.localCamera.localRoiMaxY  = (y - currentData.localCamera.localPosY);
                }
            }
        }
    }
    else {
        
        if (x < runROIStartX) {currentData.runROIMinX = x; currentData.runROIMaxX = runROIStartX;}
        else {currentData.runROIMaxX = x; currentData.runROIMinX = runROIStartX;}
        
        if (y < runROIStartY) {currentData.runROIMinY = y; currentData.runROIMaxY = runROIStartY;}
        else {currentData.runROIMaxY = y; currentData.runROIMinY = runROIStartY;}
    }
}
//--------------------------------------------------------------
void flexcamApp::mousePressed(int x, int y, int button)
{
    x = x - INFO_WINDOW_WIDTH;
    
    if (button == 0){
        
        if (bConfigDisplay){
            if(bConfigMoveObject){startMoveObjectX = x; startMoveObjectY=y;}
            else{
                
                if (currentData.configType == "videos"){
                    if (((x >= currentData.videos[currentData.activeObject].videoPosX) && (x <= currentData.videos[currentData.activeObject].videoPosX+currentData.videos[currentData.activeObject].videoWidth))
                        && ((y >= currentData.videos[currentData.activeObject].videoPosY) && (y <=currentData.videos[currentData.activeObject].videoPosY+currentData.videos[currentData.activeObject].videoHeight)))
                    {currentData.videos[currentData.activeObject].videoRoiMinX = x - currentData.videos[currentData.activeObject].videoPosX; currentData.videos[currentData.activeObject].videoRoiMinY = y-currentData.videos[currentData.activeObject].videoPosY;}
                }
                
                if (currentData.configType == "cameras"){
                    if (((x >= currentData.IPCameras[currentData.activeObject].camPosX) && (x <= currentData.IPCameras[currentData.activeObject].camPosX+currentData.IPCameras[currentData.activeObject].camWidth))
                        && ((y >= currentData.IPCameras[currentData.activeObject].camPosY) && (y <=currentData.IPCameras[currentData.activeObject].camPosY+currentData.IPCameras[currentData.activeObject].camHeight)))
                    {currentData.IPCameras[currentData.activeObject].camRoiMinX = x - currentData.IPCameras[currentData.activeObject].camPosX; currentData.IPCameras[currentData.activeObject].camRoiMinY = y-currentData.IPCameras[currentData.activeObject].camPosY;}
                }
                
                if (currentData.configType == "local"){
                    if (((x >= currentData.localCamera.localPosX) && (x <= currentData.localCamera.localPosX+currentData.localCamera.localWidth))
                        && ((y >= currentData.localCamera.localPosY) && (y <=currentData.localCamera.localPosY+currentData.localCamera.localHeight)))
                    {currentData.localCamera.localRoiMinX = x - currentData.localCamera.localPosX; currentData.localCamera.localRoiMinY = y-currentData.localCamera.localPosY;}
                }
            };
        }
        else {runROIStartX = x; runROIStartY = y;}
    }
}
//--------------------------------------------------------------
void flexcamApp::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void flexcamApp::windowResized(int w, int h){}
//--------------------------------------------------------------
void flexcamApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void flexcamApp::dragEvent(ofDragInfo dragInfo){ 

    if( dragInfo.files.size() == 1){
        string filename = ofFilePath::getFileName(dragInfo.files[0]);
        if (ofFilePath::getFileExt(filename) == "xml"){
            bProcessConfig = false;
            bool loadOkay = processXML.loadFile(dragInfo.files[0].c_str());
            if (loadOkay){
                bProcessConfig = true;
                processPath = dragInfo.files[0].c_str();
            }
        }
    }
}
