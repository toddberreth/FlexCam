#include "flexcamApp.h"

//--------------------------------------------------------------
void flexcamApp::setupMedia()
{
    ofTrueTypeFont::setGlobalDpi(72);
    veramono10.loadFont("fonts/veramono.ttf", 10, true, false); veramono10.setLineHeight(10.0f); veramono10.setLetterSpacing(1);
    verdana24.loadFont("fonts/verdana.ttf", 24, true, false); verdana24.setLineHeight(24.0f); verdana24.setLetterSpacing(1.5);
    configImage.loadImage("images/config.png");
}

//--------------------------------------------------------------
void flexcamApp::setupOscSend()
{
}

//--------------------------------------------------------------
void flexcamApp::autoLoadConfig(string path)
{
    string filename = ofFilePath::getFileName(path);
    if (ofFilePath::getFileExt(filename) == "xml"){
        bProcessConfig = false;
        bool loadOkay = processXML.loadFile(ofFilePath::getAbsolutePath(path, TRUE).c_str());
        if (loadOkay){
            bProcessConfig = true;
            processPath = ofFilePath::getAbsolutePath(path, TRUE).c_str();
        }
    }
}

//--------------------------------------------------------------
void flexcamApp::processConfigFile(){
    
    bProcessConfig = false;
    if (bLoadedConfig) clearAllData();
    currentXML = processXML;
    currentPath = processPath;
    bLoadConfig = true;
}

//--------------------------------------------------------------
void flexcamApp::loadConfig(){
    
    bLoadConfig = false;
    
    currentXML.pushTag("global");
        currentData.configName              = currentXML.getAttribute("name", "config", "");
        currentData.configType              = currentXML.getAttribute("type", "inputtype", "");
        currentData.blobName                = currentXML.getAttribute("blob", "blobname", "");
        currentData.canvasWidth             = currentXML.getAttribute("canvas", "width", 0);
        currentData.canvasHeight            = currentXML.getAttribute("canvas", "height", 0);
        currentData.runROIMinX              = currentXML.getAttribute("roi", "roiminx", 0);
        currentData.runROIMinY              = currentXML.getAttribute("roi", "roiminy", 0);
        currentData.runROIMaxX              = currentXML.getAttribute("roi", "roimaxx", currentData.canvasWidth);
        currentData.runROIMaxY              = currentXML.getAttribute("roi", "roimaxy", currentData.canvasHeight);
        currentData.bConfigDisplay          = ofToBool(currentXML.getAttribute("modeinit", "configdisplay", "false"));
        currentData.stitchedDisplayMode     = ofToChar(currentXML.getAttribute("modeinit", "stitchedmode", "s"));
        currentData.activeObject            = (currentXML.getAttribute("modeinit", "activeobject", 1) - 1);
        currentData.bAutoBackgrounding      = ofToBool(currentXML.getAttribute("autoback", "autoback", "false"));
    currentXML.popTag();
    
    currentXML.pushTag("processing");
        currentData.threshold               = currentXML.getAttribute("threshold", "threshold", 0);
        currentData.thresholdMax            = currentXML.getAttribute("threshold", "thresholdmax", 255);
        currentData.blur                    = currentXML.getAttribute("blur", "blur", 0);
        currentData.blurMax                 = currentXML.getAttribute("blur", "blurmax", 9);
        currentData.dilate                  = currentXML.getAttribute("dilate", "dilate", 0);
        currentData.dilateMax               = currentXML.getAttribute("dilate", "dilatemax", 20);
        currentData.erode                   = currentXML.getAttribute("erode", "erode", 0);
        currentData.erodeMax                = currentXML.getAttribute("erode", "erodemax", 20);
        currentData.blobMinArea             = currentXML.getAttribute("blob", "filterminarea", 0.0);
        currentData.blobMaxArea             = currentXML.getAttribute("blob", "filtermaxarea", 999999.0);
        currentData.blobSmoothing           = currentXML.getAttribute("blob", "blobsmoothing", 0.2);
    currentXML.popTag();
    
    windowWidth = INFO_WINDOW_WIDTH + currentData.canvasWidth; windowHeight = currentData.canvasHeight;
    if (windowWidth < MIN_WINDOW_WIDTH) windowWidth = MIN_WINDOW_WIDTH; if (windowHeight < MIN_WINDOW_HEIGHT) windowHeight = MIN_WINDOW_HEIGHT;
    
    string tag;
        
    currentXML.pushTag("ipcameras");
        tag = "camera";
        int numIPCams = currentXML.getNumTags(tag);
        for(int n = 0; n < numIPCams; n++){
            
            IPCameraData newCameraData;
            newCameraData.camName = currentXML.getAttribute(tag, "name", "", n);
            newCameraData.camIP = currentXML.getAttribute(tag, "ip", "", n);
            newCameraData.camURL = currentXML.getAttribute(tag, "url", "", n);
            newCameraData.camType = currentXML.getAttribute(tag, "type", "", n);
            newCameraData.camUsername = currentXML.getAttribute(tag, "username", "", n);
            newCameraData.camPassword = currentXML.getAttribute(tag, "password", "", n);
            newCameraData.camWidth = currentXML.getAttribute(tag, "width", DEFAULT_IP_CAM_WIDTH, n);
            newCameraData.camHeight = currentXML.getAttribute(tag, "height", DEFAULT_IP_CAM_HEIGHT, n);
            newCameraData.camPosX = currentXML.getAttribute(tag, "posx", 0, n);
            newCameraData.camPosY = currentXML.getAttribute(tag, "posy", 0, n);
            newCameraData.camRoiMinX = currentXML.getAttribute(tag, "roiminx", 0, n);
            newCameraData.camRoiMinY = currentXML.getAttribute(tag, "roiminy", 0, n);
            newCameraData.camRoiMaxX = currentXML.getAttribute(tag, "roimaxx", DEFAULT_IP_CAM_WIDTH, n);
            newCameraData.camRoiMaxY = currentXML.getAttribute(tag, "roimaxy", DEFAULT_IP_CAM_HEIGHT, n);
            
            if (currentData.configType == "ipcameras"){
                flexcamIPCam newCam(newCameraData.camName,newCameraData.camIP, newCameraData.camType, newCameraData.camUsername,newCameraData.camPassword,
                                    newCameraData.camWidth,newCameraData.camHeight,newCameraData.camPosX,newCameraData.camPosY,
                                    newCameraData.camRoiMinX,newCameraData.camRoiMinY,newCameraData.camRoiMaxX,newCameraData.camRoiMaxY);

                IPCameraGrabbers.push_back(newCam.setGrabber());
                IPCameras.push_back(newCam);
                newCameraData.camFrameImage.allocate(newCameraData.camWidth, newCameraData.camHeight);
                newCameraData.camFrameGrayImage.allocate(newCameraData.camWidth, newCameraData.camHeight);
                newCameraData.camFrameCurrentImage.allocate(newCameraData.camWidth, newCameraData.camHeight);
                newCameraData.camFrameLastImage.allocate(newCameraData.camWidth, newCameraData.camHeight);
                newCameraData.camFrameActivityImage.allocate(newCameraData.camWidth, newCameraData.camHeight);
            }
            newCameraData.activityLevel = 0;
            currentData.IPCameras.push_back(newCameraData);
        }
    currentXML.popTag();

    
    if (currentData.configType == "ipcameras") {state = APP_IP_CAMERAS; currentData.numberObjects = currentData.IPCameras.size();}

    currentXML.pushTag("videos");
    tag = "video";
    int numVideos = currentXML.getNumTags(tag);
    for(int n = 0; n < numVideos; n++){
        
        videoData newVideoData;
        newVideoData.videoPath = currentXML.getAttribute(tag, "path", "", n);
        newVideoData.videoSpeed = currentXML.getAttribute(tag, "speed", 1.0, n);
        newVideoData.videoWidth = currentXML.getAttribute(tag, "width", DEFAULT_IP_CAM_WIDTH, n);
        newVideoData.videoHeight = currentXML.getAttribute(tag, "height", DEFAULT_IP_CAM_HEIGHT, n);
        newVideoData.videoPosX = currentXML.getAttribute(tag, "posx", 0, n);
        newVideoData.videoPosY = currentXML.getAttribute(tag, "posy", 0, n);
        newVideoData.videoRoiMinX = currentXML.getAttribute(tag, "roiminx", 0, n);
        newVideoData.videoRoiMinY = currentXML.getAttribute(tag, "roiminy", 0, n);
        newVideoData.videoRoiMaxX = currentXML.getAttribute(tag, "roimaxx", DEFAULT_IP_CAM_WIDTH, n);
        newVideoData.videoRoiMaxY = currentXML.getAttribute(tag, "roimaxy", DEFAULT_IP_CAM_HEIGHT, n);
        
        if (currentData.configType == "videos"){
            ofVideoPlayer newVideo;
            newVideo.loadMovie(newVideoData.videoPath);
            newVideo.setSpeed(1.0);
            newVideo.play();
            videos.push_back(newVideo);
            newVideoData.videoFrameImage.allocate(newVideoData.videoWidth, newVideoData.videoHeight);
            newVideoData.videoFrameGrayImage.allocate(newVideoData.videoWidth, newVideoData.videoHeight);
            newVideoData.videoFrameCurrentImage.allocate(newVideoData.videoWidth, newVideoData.videoHeight);
            newVideoData.videoFrameLastImage.allocate(newVideoData.videoWidth, newVideoData.videoHeight);
            newVideoData.videoFrameActivityImage.allocate(newVideoData.videoWidth, newVideoData.videoHeight);
        }
        
        bool newFrame = false;
        bNewVideoFrame.push_back(newFrame);
        newVideoData.activityLevel = 0;
        currentData.videos.push_back(newVideoData);
    }
    currentXML.popTag();

    if (currentData.configType == "videos") { currentData.numberObjects = currentData.videos.size(); state = APP_VIDEOS;}
    
    currentXML.pushTag("localcamera");
        currentData.localCamera.localWidth = currentXML.getAttribute("camera", "width", 640);
        currentData.localCamera.localHeight = currentXML.getAttribute("camera", "height", 480);
        currentData.localCamera.localPosX = currentXML.getAttribute("camera", "posx", 0);
        currentData.localCamera.localPosY = currentXML.getAttribute("camera", "posy", 0);
        currentData.localCamera.localRoiMinX = currentXML.getAttribute("camera", "roiminx", 0);
        currentData.localCamera.localRoiMinY = currentXML.getAttribute("camera", "roiminy", 0);
        currentData.localCamera.localRoiMaxX = currentXML.getAttribute("camera", "roimaxx", currentData.localCamera.localWidth);
        currentData.localCamera.localRoiMaxY = currentXML.getAttribute("camera", "roimaxy", currentData.localCamera.localHeight);
    currentXML.popTag();
    
    if (currentData.configType == "localcamera"){
        bNewLocalFrame = false;
        localCamera.initGrabber(currentData.localCamera.localWidth,currentData.localCamera.localHeight);
        currentData.localCamera.localFrameImage.allocate(currentData.localCamera.localWidth,currentData.localCamera.localHeight);
        currentData.localCamera.localFrameGrayImage.allocate(currentData.localCamera.localWidth,currentData.localCamera.localHeight);
        currentData.localCamera.localFrameCurrentImage.allocate(currentData.localCamera.localWidth,currentData.localCamera.localHeight);
        currentData.localCamera.localFrameLastImage.allocate(currentData.localCamera.localWidth,currentData.localCamera.localHeight);
        currentData.localCamera.localFrameActivityImage.allocate(currentData.localCamera.localWidth,currentData.localCamera.localHeight);
        currentData.localCamera.activityLevel = 0;
        state = APP_LOCAL_CAMERA;
        currentData.numberObjects = 1; currentData.activeObject = 0;
    }
    
    currentXML.pushTag("osc");
        tag = "oscsend";
        int numOSCSends = currentXML.getNumTags(tag);
        for(int n = 0; n < numOSCSends; n++){
            oscSendUser newSend;
            newSend.ip      = currentXML.getAttribute(tag, "ip", "", n);
            newSend.port    = currentXML.getAttribute(tag, "port", 57120, n);
            currentData.oscSends.push_back(newSend);
            flexcamOSC *newOscSend = new flexcamOSC(newSend.ip,newSend.port,currentData.blobName);
            oscSends.push_back(newOscSend);
        }
    currentXML.popTag();

    if (currentData.bConfigDisplay == true) {
        
        bConfigDisplay = currentData.bConfigDisplay;
        state = APP_CONFIGURE_LAYOUT;
    }
    
    stitchedGrayImage.allocate(currentData.canvasWidth,currentData.canvasHeight);
    backgroundGrayImage.allocate(currentData.canvasWidth,currentData.canvasHeight);
    croppedStitchedGrayImage.allocate(currentData.canvasWidth,currentData.canvasHeight);
    croppedBackgroundGrayImage.allocate(currentData.canvasWidth,currentData.canvasHeight);
    differenceGrayImage.allocate(currentData.canvasWidth,currentData.canvasHeight);
    thresholdGrayImage.allocate(currentData.canvasWidth,currentData.canvasHeight);
    dataImage.allocate(currentData.canvasWidth,currentData.canvasHeight);
    chB.allocate(currentData.canvasWidth,currentData.canvasHeight);
    labelImage 	= cvCreateImage(cvSize(currentData.canvasWidth,currentData.canvasHeight), IPL_DEPTH_LABEL, 1);
    
    bShowLabels = bLoadedConfig = true;
    noMotionTimer = 0;
}

//--------------------------------------------------------------
void flexcamApp::saveConfig(){
    
    bSaveConfig = false;
    ofxXmlSettings saveXML;
    
    saveXML.addTag("global");
    saveXML.pushTag("global");
        saveXML.addTag("name");
            saveXML.addAttribute("name", "config", currentData.configName, 0);
        saveXML.addTag("type");
            saveXML.addAttribute("type", "inputtype", currentData.configType, 0);
        saveXML.addTag("blob");
            saveXML.addAttribute("blob", "blobname", currentData.blobName, 0);
        saveXML.addTag("canvas");
            saveXML.addAttribute("canvas", "width", currentData.canvasWidth, 0);
            saveXML.addAttribute("canvas", "height", currentData.canvasHeight, 0);
        saveXML.addTag("roi");
            saveXML.addAttribute("roi", "roiminx", currentData.runROIMinX, 0);
            saveXML.addAttribute("roi", "roiminy", currentData.runROIMinY, 0);
            saveXML.addAttribute("roi", "roimaxx", currentData.runROIMaxX, 0);
            saveXML.addAttribute("roi", "roimaxy", currentData.runROIMaxY, 0);
        saveXML.addTag("modeinit");
            saveXML.addAttribute("modeinit", "configdisplay", currentData.bConfigDisplay ? "true":"false", 0);
            saveXML.addAttribute("modeinit", "stitchedmode", ofToString(currentData.stitchedDisplayMode), 0);
            saveXML.addAttribute("modeinit", "activeobject", currentData.activeObject + 1, 0);
        saveXML.addTag("autoback");
            saveXML.addAttribute("autoback", "autoback", currentData.bAutoBackgrounding ? "true":"false", 0);
    saveXML.popTag();

    saveXML.addTag("processing");
    saveXML.pushTag("processing");
        saveXML.addTag("threshold");
            saveXML.addAttribute("threshold", "threshold", currentData.threshold, 0);
            saveXML.addAttribute("threshold", "thresholdmax", currentData.thresholdMax, 0);
        saveXML.addTag("blur");
            saveXML.addAttribute("blur", "blur", currentData.blur , 0);
            saveXML.addAttribute("blur", "blurmax", currentData.blurMax, 0);
        saveXML.addTag("dilate");
            saveXML.addAttribute("dilate", "dilate", currentData.dilate, 0);
            saveXML.addAttribute("dilate", "dilatemax", currentData.dilateMax, 0);
        saveXML.addTag("erode");
            saveXML.addAttribute("erode", "erode", currentData.erode, 0);
            saveXML.addAttribute("erode", "erodemax", currentData.erodeMax, 0);
        saveXML.addTag("blob");
            saveXML.addAttribute("blob", "filterminarea", currentData.blobMinArea, 0);
            saveXML.addAttribute("blob", "filtermaxarea", currentData.blobMaxArea, 0);
            saveXML.addAttribute("blob", "blobsmoothing", currentData.blobSmoothing, 0);
    saveXML.popTag();
    
    saveXML.addTag("ipcameras");
    saveXML.pushTag("ipcameras");
        for (int i = 0; i < currentData.IPCameras.size(); i++){
            saveXML.addTag("camera");
            saveXML.addAttribute("camera", "name", currentData.IPCameras[i].camName , i);
            saveXML.addAttribute("camera", "ip", currentData.IPCameras[i].camIP , i);
            saveXML.addAttribute("camera", "url", currentData.IPCameras[i].camURL , i);
            saveXML.addAttribute("camera", "type", currentData.IPCameras[i].camType , i);
            saveXML.addAttribute("camera", "username", currentData.IPCameras[i].camUsername , i);
            saveXML.addAttribute("camera", "password", currentData.IPCameras[i].camPassword , i);
            saveXML.addAttribute("camera", "width", currentData.IPCameras[i].camWidth , i);
            saveXML.addAttribute("camera", "height", currentData.IPCameras[i].camHeight , i);
            saveXML.addAttribute("camera", "posx", currentData.IPCameras[i].camPosX , i);
            saveXML.addAttribute("camera", "posy", currentData.IPCameras[i].camPosY , i);
            saveXML.addAttribute("camera", "roiminx", currentData.IPCameras[i].camRoiMinX , i);
            saveXML.addAttribute("camera", "roiminy", currentData.IPCameras[i].camRoiMinY , i);
            saveXML.addAttribute("camera", "roimaxx", currentData.IPCameras[i].camRoiMaxX , i);
            saveXML.addAttribute("camera", "roimaxy", currentData.IPCameras[i].camRoiMaxY , i);
        }
    saveXML.popTag();
    
    saveXML.addTag("videos");
    saveXML.pushTag("videos");
    for (int i = 0; i < currentData.videos.size(); i++){
        saveXML.addTag("video");
        saveXML.addAttribute("video", "path", currentData.videos[i].videoPath , i);
        saveXML.addAttribute("video", "speed", currentData.videos[i].videoSpeed , i);
        saveXML.addAttribute("video", "width", currentData.videos[i].videoWidth , i);
        saveXML.addAttribute("video", "height", currentData.videos[i].videoHeight , i);
        saveXML.addAttribute("video", "posx", currentData.videos[i].videoPosX , i);
        saveXML.addAttribute("video", "posy", currentData.videos[i].videoPosY , i);
        saveXML.addAttribute("video", "roiminx", currentData.videos[i].videoRoiMinX , i);
        saveXML.addAttribute("video", "roiminy", currentData.videos[i].videoRoiMinY , i);
        saveXML.addAttribute("video", "roimaxx", currentData.videos[i].videoRoiMaxX , i);
        saveXML.addAttribute("video", "roimaxy", currentData.videos[i].videoRoiMaxY , i);
    }
    saveXML.popTag();
    
    saveXML.addTag("localcamera");
    saveXML.pushTag("localcamera");
        saveXML.addTag("camera");
        saveXML.addAttribute("camera", "width", currentData.localCamera.localWidth , 0);
        saveXML.addAttribute("camera", "height", currentData.localCamera.localHeight , 0);
        saveXML.addAttribute("camera", "posx", currentData.localCamera.localPosX , 0);
        saveXML.addAttribute("camera", "posy", currentData.localCamera.localPosY , 0);
        saveXML.addAttribute("camera", "roiminx", currentData.localCamera.localRoiMinX , 0);
        saveXML.addAttribute("camera", "roiminy", currentData.localCamera.localRoiMinY , 0);
        saveXML.addAttribute("camera", "roimaxx", currentData.localCamera.localRoiMaxX , 0);
        saveXML.addAttribute("camera", "roimaxy", currentData.localCamera.localRoiMaxY , 0);
    saveXML.popTag();

    saveXML.addTag("osc");
    saveXML.pushTag("osc");
    for (int i = 0; i < currentData.oscSends.size(); i++){
        saveXML.addTag("oscsend");
        saveXML.addAttribute("oscsend", "ip", currentData.oscSends[i].ip , i);
        saveXML.addAttribute("oscsend", "port", currentData.oscSends[i].port , i);
    }
    saveXML.popTag();
    
    saveXML.saveFile(currentPath);
}


//--------------------------------------------------------------
void flexcamApp::clearAllData(){
    
    //clear IP cameras
    for (int i = 0; i < currentData.IPCameras.size(); i++ ){
     
        currentData.IPCameras[i].camFrameGrayImage.clear();
        currentData.IPCameras[i].camFrameCurrentImage.clear();
        currentData.IPCameras[i].camFrameLastImage.clear();
        currentData.IPCameras[i].camFrameActivityImage.clear();
    }
    currentData.IPCameras.clear();
    IPCameras.clear();
    IPCameraGrabbers.clear();

    //clear videos
    for (int i = 0; i < currentData.videos.size(); i++ ){
        
        currentData.videos[i].videoFrameImage.clear();
        currentData.videos[i].videoFrameGrayImage.clear();
    }
    currentData.videos.clear();
    videos.clear();
    bNewVideoFrame.clear();
    
    //clear local camera
    currentData.localCamera.localFrameImage.clear();
    currentData.localCamera.localFrameGrayImage.clear();
    currentData.localCamera.localFrameCurrentImage.clear();
    currentData.localCamera.localFrameLastImage.clear();
    currentData.localCamera.localFrameActivityImage.clear();
    
    // initialize global variables
    currentData.configName = currentData.configType = currentData.blobName = "";
    currentData.canvasWidth = currentData.canvasHeight = 0;
    currentData.runROIMinX = currentData.runROIMinY = currentData.runROIMaxX = currentData.runROIMaxY = currentData.activeObject = 0;
    currentData.threshold = currentData.blur = currentData.dilate = currentData.erode = 0;
    currentData.thresholdMax = currentData.blurMax = currentData.dilateMax = currentData.erodeMax = 0;
    currentData.blobMinArea = currentData.blobMaxArea = currentData.blobSmoothing = 0;
    currentData.stitchedDisplayMode = NULL;
    currentData.bConfigDisplay = false;
    currentData.bAutoBackgrounding = false;
    
    //clear
    stitchedGrayImage.clear();
    backgroundGrayImage.clear();
    croppedStitchedGrayImage.clear();
    croppedBackgroundGrayImage.clear();
    differenceGrayImage.clear();
    thresholdGrayImage.clear();
    dataImage.clear();
    chB.clear();
    if (labelImage != NULL) cvReleaseImage(&labelImage);
    
    oscSends.clear();
    currentData.oscSends.clear();

    
}


