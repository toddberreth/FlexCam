#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxXmlSettings.h"
#include "IPVideoGrabber.h"

#include "flexcamDataTypes.h"
#include "flexcamIPCam.h"
#include "flexcamOSC.h"
#include "flexcamBlob.h"

#include "cvblob.h"
#include "defines.h"

using ofx::Video::IPVideoGrabber;
using ofx::Video::SharedIPVideoGrabber;

typedef enum{
    APP_JUST_LAUNCHED,
    APP_WAITING_FOR_CONFIG,
    APP_VIDEOS,
    APP_IP_CAMERAS,
    APP_LOCAL_CAMERA,
    APP_CONFIGURE_LAYOUT,
    APP_HELP,
}appState;

using namespace cvb;								// use cvBlob namespace

class flexcamApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void					drawInfo();
    
        appState				state;
        int						windowWidth, windowHeight;
        bool					bLearnBackground, bConfigDisplay, bConfigMoveObject, bCropROI, bShowLabels, bShowFramerate, bShowIPCamerasInfo;
    
        //config ///////////////////////////////////////////////////////////////
    
        void					setupMedia();
        void					autoLoadConfig(string path);
        void					clearAllData();
        void					loadConfig();
        void					saveConfig();
        void					processConfigFile();
        void                    drawConfigDisplay();
    
        bool					bProcessConfig, bLoadConfig, bLoadedConfig, bSaveConfig;
        string					processPath, currentPath;
        ofxXmlSettings			processXML;
        ofxXmlSettings			currentXML;
    
        configData				currentData;
    
        //general ///////////////////////////////////////////////////////////////
    
        ofTrueTypeFont			veramono10;
        ofTrueTypeFont			verdana24;
        ofImage					configImage;
    
        //image processing//////////////////////////////////////////////////////
    
        void					updateStitchedImage();
        void					updateProcessedImage();
        void					drawImage();
    
        ofxCvGrayscaleImage		stitchedGrayImage;
        ofxCvGrayscaleImage		backgroundGrayImage;
        ofxCvGrayscaleImage		croppedStitchedGrayImage;
        ofxCvGrayscaleImage		croppedBackgroundGrayImage;
        ofxCvGrayscaleImage		differenceGrayImage;
        ofxCvGrayscaleImage		thresholdGrayImage;
        ofxCvColorImage			dataImage;
    
        //ip cameras ///////////////////////////////////////////////////////////////
    
        void                    loadIPCameras();
        void					updateIPCameras();
        void                    drawIPCamerasInfo();
    
        vector<flexcamIPCam>            IPCameras;
        vector<SharedIPVideoGrabber>    IPCameraGrabbers;
    
        //videos ////////////////////////////////////////////////////////////////
    
        void					updateVideos();
        vector<ofVideoPlayer>	videos;
        vector<bool>			bNewVideoFrame;
    
        //local camera /////////////////////////////////////////////////////////////////
    
    
        void					updateLocalCamera();
    
        ofVideoGrabber			localCamera;
        bool					bNewLocalFrame;
    
        //osc   ///////////////////////////////////////////////////////////////////
    
        void					setupOscSend();
        void					updateOscSend();
        vector<flexcamOSC*>		oscSends;
    
        //blobs/////////////////////////////////////////////////////////////////
    
        void					updateBlobTracking();
    
        vector<flexcamBlob>		flexcamBlobs;
        CvBlobs                 blobs;
        CvTracks				tracks;
        IplImage*				labelImage;
        ofxCvGrayscaleImage     chB;

        //mouse ////////////////////////////////////////////////////////////////
    
        void					drawRunROI();
        int						runROIStartX, runROIStartY, startMoveObjectX, startMoveObjectY;
    
        //get activity level ///////////////////////////////////////////////////
    
        float					updateActivityLevel(ofxCvGrayscaleImage *activityFrame);
        float					activityLevel;
    
        //auto backgrounding ///////////////////////////////////////////////////
    
        void					updateAutoBackgrounding();
        int						noMotionTimer;
};
