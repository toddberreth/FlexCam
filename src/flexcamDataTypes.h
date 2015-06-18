struct localCameraData {
    
    int					localWidth, localHeight, localPosX, localPosY, localRoiMinX, localRoiMinY, localRoiMaxX, localRoiMaxY;
    ofxCvColorImage		localFrameImage;
    ofxCvGrayscaleImage	localFrameGrayImage;
    ofxCvGrayscaleImage	localFrameCurrentImage;
    ofxCvGrayscaleImage	localFrameLastImage;
    ofxCvGrayscaleImage	localFrameActivityImage;
    float				activityLevel;
};

struct IPCameraData {
    
    string				camName, camIP, camURL, camType, camUsername, camPassword;
    int					camWidth, camHeight, camPosX, camPosY, camRoiMinX, camRoiMinY, camRoiMaxX, camRoiMaxY;
    ofxCvColorImage     camFrameImage;
    ofxCvGrayscaleImage	camFrameGrayImage;
    ofxCvGrayscaleImage	camFrameCurrentImage;
    ofxCvGrayscaleImage	camFrameLastImage;
    ofxCvGrayscaleImage	camFrameActivityImage;
    float				activityLevel;
};

struct videoData {
    
    string				videoPath;
    int					videoWidth, videoHeight, videoPosX, videoPosY, videoRoiMinX, videoRoiMinY, videoRoiMaxX, videoRoiMaxY;
    float				videoSpeed;
    ofxCvColorImage		videoFrameImage;
    ofxCvGrayscaleImage	videoFrameGrayImage;
    ofxCvGrayscaleImage	videoFrameCurrentImage;
    ofxCvGrayscaleImage	videoFrameLastImage;
    ofxCvGrayscaleImage	videoFrameActivityImage;
    float				activityLevel;
};

struct oscSendUser
{
    string				ip;
    int					port;
};

struct configData
{
    
    string				configName, configType, blobName;
    int					activeObject, numberObjects, canvasWidth, canvasHeight, runROIMinX, runROIMinY, runROIMaxX, runROIMaxY;
    int					threshold, blur, dilate, erode;
    int					thresholdMax, blurMax, dilateMax, erodeMax;
    int					blobMinArea, blobMaxArea;
    float				blobSmoothing;
    char				stitchedDisplayMode;
    bool				bConfigDisplay;
    bool				bAutoBackgrounding;
    
    vector<oscSendUser>     oscSends;
    vector<IPCameraData>    IPCameras;
    vector<videoData>       videos;
    localCameraData         localCamera;
};