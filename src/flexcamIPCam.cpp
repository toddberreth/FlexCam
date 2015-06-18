#include "flexcamIPCam.h"

//--------------------------------------------------------------
flexcamIPCam::~flexcamIPCam()
{
}

//--------------------------------------------------------------
void flexcamIPCam::update()
{
    if (_videoGrabber->isConnected()){
        if (bGetFrame){
            if(_videoGrabber->isFrameNew()){_camFrame.setFromPixels(_videoGrabber->getPixels(), _width, _height); }
            bReadyFrame = true;
            bGetFrame = false;
        }
    }
    
    _roiX = _roiMinX;
    _roiY = _roiMinY;
    _roiWidth  = _roiMaxX - _roiMinX;
    _roiHeight = _roiMaxY - _roiMinY;
}