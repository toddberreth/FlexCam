#include "flexcamApp.h"

//--------------------------------------------------------------
void flexcamApp::updateIPCameras()
{
    for (int i = 0; i < IPCameraGrabbers.size(); i++){IPCameraGrabbers[i]->update();}
    for (int i = 0; i < IPCameras.size(); i++){IPCameras[i].update();}
}

//--------------------------------------------------------------
void flexcamApp::updateLocalCamera()
{
    localCamera.update();
    bNewLocalFrame = localCamera.isFrameNew();
}

//--------------------------------------------------------------
void flexcamApp::updateVideos()
{
    for (int i = 0; i < videos.size(); i++){
        videos[i].update();
        bNewVideoFrame[i] = videos[i].isFrameNew();
    }
}

//--------------------------------------------------------------
void flexcamApp::updateStitchedImage()
{
    if (currentData.configType == "videos"){
        float allVideoActivity = 0;
        for (int i = 0; i < videos.size(); i++){
            if (bNewVideoFrame[i]){
                currentData.videos[i].videoFrameImage.setFromPixels(videos[i].getPixels(), videos[i].getWidth(),videos[i].getHeight());
                currentData.videos[i].videoFrameGrayImage = currentData.videos[i].videoFrameImage;
                
                currentData.videos[i].videoFrameLastImage = currentData.videos[i].videoFrameCurrentImage;
                currentData.videos[i].videoFrameCurrentImage = currentData.videos[i].videoFrameGrayImage;
                currentData.videos[i].videoFrameActivityImage.absDiff(currentData.videos[i].videoFrameLastImage,currentData.videos[i].videoFrameCurrentImage);
                currentData.videos[i].videoFrameActivityImage.threshold(ACTIVITY_THRESHOLD);
                currentData.videos[i].activityLevel = updateActivityLevel(&currentData.videos[i].videoFrameActivityImage);
                if (currentData.videos[i].activityLevel > 0){
                    for (int j = 0; j < oscSends.size(); j++ ) oscSends[j]->messageActivitySend("video", i ,currentData.videos[i].activityLevel);
                    allVideoActivity = allVideoActivity + currentData.videos[i].activityLevel;
                }
                
                currentData.videos[i].videoFrameGrayImage.setROI(currentData.videos[i].videoRoiMinX,
                                                                 currentData.videos[i].videoRoiMinY,
                                                                 currentData.videos[i].videoRoiMaxX - currentData.videos[i].videoRoiMinX,
                                                                 currentData.videos[i].videoRoiMaxY - currentData.videos[i].videoRoiMinY);
                stitchedGrayImage.setROI						(currentData.videos[i].videoPosX + currentData.videos[i].videoRoiMinX,
                                                                 currentData.videos[i].videoPosY + currentData.videos[i].videoRoiMinY,
                                                                 currentData.videos[i].videoRoiMaxX - currentData.videos[i].videoRoiMinX,
                                                                 currentData.videos[i].videoRoiMaxY - currentData.videos[i].videoRoiMinY);
                
                cvAddWeighted(currentData.videos[i].videoFrameGrayImage.getCvImage(), ADD_WEIGHTED, stitchedGrayImage.getCvImage(),(1-ADD_WEIGHTED), 0, stitchedGrayImage.getCvImage());
                currentData.videos[i].videoFrameGrayImage.resetROI();
            }
        }
        
        if (allVideoActivity > 0) {
            for (int j = 0; j < oscSends.size(); j++ ) oscSends[j]->messageAllActivitySend((float)allVideoActivity/videos.size());
        }
        
        activityLevel = (float)allVideoActivity/videos.size();
        
        stitchedGrayImage.resetROI();
    }
    
    if (currentData.configType == "ipcameras"){
        
        bool bAllReady = true;
        for (int i = 0; i < IPCameras.size(); i++){if (IPCameras[i].bReadyFrame == false) bAllReady = false;}
        
        if (bAllReady){
            cvSet(stitchedGrayImage.getCvImage(), cvScalar(0,0,0));
            float allCameraActivity = 0;
            for (int i = 0; i < IPCameras.size(); i++){
                currentData.IPCameras[i].camFrameGrayImage = IPCameras[i].getCurrentFrame();
                currentData.IPCameras[i].camFrameLastImage = currentData.IPCameras[i].camFrameCurrentImage;
                currentData.IPCameras[i].camFrameCurrentImage = currentData.IPCameras[i].camFrameGrayImage;
                currentData.IPCameras[i].camFrameActivityImage.absDiff(currentData.IPCameras[i].camFrameLastImage,currentData.IPCameras[i].camFrameCurrentImage);
                currentData.IPCameras[i].camFrameActivityImage.threshold(ACTIVITY_THRESHOLD);
                currentData.IPCameras[i].activityLevel = updateActivityLevel(&currentData.IPCameras[i].camFrameActivityImage);
                if (currentData.IPCameras[i].activityLevel > 0){
                    for (int j = 0; j < oscSends.size(); j++ ) oscSends[j]->messageActivitySend("cam", i ,currentData.IPCameras[i].activityLevel);
                    allCameraActivity = allCameraActivity + currentData.IPCameras[i].activityLevel;
                }
                
                currentData.IPCameras[i].camFrameGrayImage.setROI(currentData.IPCameras[i].camRoiMinX,
                                                                currentData.IPCameras[i].camRoiMinY,
                                                                currentData.IPCameras[i].camRoiMaxX - currentData.IPCameras[i].camRoiMinX,
                                                                currentData.IPCameras[i].camRoiMaxY - currentData.IPCameras[i].camRoiMinY);
                stitchedGrayImage.setROI						(currentData.IPCameras[i].camPosX + currentData.IPCameras[i].camRoiMinX,
                                                                 currentData.IPCameras[i].camPosY + currentData.IPCameras[i].camRoiMinY,
                                                                 currentData.IPCameras[i].camRoiMaxX - currentData.IPCameras[i].camRoiMinX,
                                                                 currentData.IPCameras[i].camRoiMaxY - currentData.IPCameras[i].camRoiMinY);
                
                cvAddWeighted(currentData.IPCameras[i].camFrameGrayImage.getCvImage(), ADD_WEIGHTED, stitchedGrayImage.getCvImage(), (1-ADD_WEIGHTED), 0, stitchedGrayImage.getCvImage());
                currentData.IPCameras[i].camFrameGrayImage.resetROI();
                IPCameras[i].bGetFrame = true;
            }
            
            if (allCameraActivity > 0){
                for (int j = 0; j < oscSends.size(); j++ ) oscSends[j]->messageAllActivitySend((float)allCameraActivity/IPCameras.size());
            }
            
            activityLevel = (float)allCameraActivity/IPCameras.size();
            
            stitchedGrayImage.resetROI();
        }
    }
    
    if (currentData.configType == "localcamera"){
        if (bNewLocalFrame){
            cvSet(stitchedGrayImage.getCvImage(), cvScalar(0,0,0));
            currentData.localCamera.localFrameImage.setFromPixels(localCamera.getPixels(), localCamera.getWidth(),localCamera.getHeight());
            currentData.localCamera.localFrameGrayImage = currentData.localCamera.localFrameImage;
            
            currentData.localCamera.localFrameLastImage = currentData.localCamera.localFrameCurrentImage;
            currentData.localCamera.localFrameCurrentImage = currentData.localCamera.localFrameGrayImage;
            currentData.localCamera.localFrameActivityImage.absDiff(currentData.localCamera.localFrameLastImage,currentData.localCamera.localFrameCurrentImage);
            currentData.localCamera.localFrameActivityImage.threshold(ACTIVITY_THRESHOLD);
            currentData.localCamera.activityLevel = updateActivityLevel(&currentData.localCamera.localFrameActivityImage);
            activityLevel = currentData.localCamera.activityLevel;
            
            if (currentData.localCamera.activityLevel > 0){
                for (int j = 0; j < oscSends.size(); j++ ){
                    oscSends[j]->messageActivitySend("local", 0 ,currentData.localCamera.activityLevel);
                    oscSends[j]->messageAllActivitySend(currentData.localCamera.activityLevel);
                }
            }
            
            currentData.localCamera.localFrameGrayImage.setROI(currentData.localCamera.localRoiMinX,
                                                         currentData.localCamera.localRoiMinY,
                                                         currentData.localCamera.localRoiMaxX - currentData.localCamera.localRoiMinX,
                                                         currentData.localCamera.localRoiMaxY - currentData.localCamera.localRoiMinY);
            stitchedGrayImage.setROI						(currentData.localCamera.localPosX + currentData.localCamera.localRoiMinX,
                                                             currentData.localCamera.localPosY + currentData.localCamera.localRoiMinY,
                                                             currentData.localCamera.localRoiMaxX - currentData.localCamera.localRoiMinX,
                                                             currentData.localCamera.localRoiMaxY - currentData.localCamera.localRoiMinY);
            
            cvAddWeighted(currentData.localCamera.localFrameGrayImage.getCvImage(), ADD_WEIGHTED, stitchedGrayImage.getCvImage(), (1-ADD_WEIGHTED), 0, stitchedGrayImage.getCvImage());
            currentData.localCamera.localFrameGrayImage.resetROI();
        }
        stitchedGrayImage.resetROI();
    }
}

//--------------------------------------------------------------
void flexcamApp::updateProcessedImage()
{
    if (bCropROI){
        if (((currentData.runROIMaxX - currentData.runROIMinX)>0) &&  ((currentData.runROIMaxY - currentData.runROIMinY)>0)){
            
            cvSet(croppedStitchedGrayImage.getCvImage(), cvScalar(0,0,0)); cvSet(croppedBackgroundGrayImage.getCvImage(), cvScalar(0,0,0));
            cvSetImageROI(stitchedGrayImage.getCvImage(),
                          cvRect(currentData.runROIMinX,currentData.runROIMinY,currentData.runROIMaxX - currentData.runROIMinX,currentData.runROIMaxY - currentData.runROIMinY));
            cvSetImageROI(croppedStitchedGrayImage.getCvImage(),
                          cvRect(currentData.runROIMinX,currentData.runROIMinY,currentData.runROIMaxX - currentData.runROIMinX,currentData.runROIMaxY - currentData.runROIMinY));
            cvCopy(stitchedGrayImage.getCvImage(), croppedStitchedGrayImage.getCvImage(), NULL);
            
            stitchedGrayImage.resetROI(); croppedStitchedGrayImage.resetROI();
            cvSetImageROI(backgroundGrayImage.getCvImage(),
                          cvRect(currentData.runROIMinX,currentData.runROIMinY,currentData.runROIMaxX - currentData.runROIMinX,currentData.runROIMaxY - currentData.runROIMinY));
            cvSetImageROI(croppedBackgroundGrayImage.getCvImage(),
                          cvRect(currentData.runROIMinX,currentData.runROIMinY,currentData.runROIMaxX - currentData.runROIMinX,currentData.runROIMaxY - currentData.runROIMinY));
            cvCopy(backgroundGrayImage.getCvImage(), croppedBackgroundGrayImage.getCvImage(), NULL);
            backgroundGrayImage.resetROI(); croppedBackgroundGrayImage.resetROI();
            differenceGrayImage.absDiff(croppedBackgroundGrayImage, croppedStitchedGrayImage);
        }
    }
    else differenceGrayImage.absDiff(backgroundGrayImage, stitchedGrayImage);
    
    thresholdGrayImage = differenceGrayImage;
    
    if ((currentData.blur > 0)&&(currentData.blur<=currentData.blurMax))	cvSmooth(thresholdGrayImage.getCvImage(), thresholdGrayImage.getCvImage(), CV_GAUSSIAN, currentData.blur, currentData.blur);
    if ((currentData.dilate > 0)&&(currentData.dilate<=currentData.dilateMax))	cvDilate(thresholdGrayImage.getCvImage(), thresholdGrayImage.getCvImage(), NULL, currentData.dilate);
    if ((currentData.erode > 0)&&(currentData.erode<=currentData.erodeMax))		cvErode(thresholdGrayImage.getCvImage(), thresholdGrayImage.getCvImage(), NULL, currentData.erode);
    
    thresholdGrayImage.threshold(currentData.threshold);
    
    if (bLearnBackground == true){  backgroundGrayImage = stitchedGrayImage; bLearnBackground = false; activityLevel;}
}

//--------------------------------------------------------------
void flexcamApp::updateBlobTracking()
{
    cvSplit(thresholdGrayImage.getCvImage(),chB.getCvImage(),NULL,NULL,NULL);
    cvLabel(chB.getCvImage(), labelImage, blobs);
    
    cvFilterByArea(blobs, currentData.blobMinArea, currentData.blobMaxArea);
    cvFilterByRegion(blobs, currentData.runROIMinX, currentData.runROIMinY, currentData.runROIMaxX - currentData.runROIMinX, currentData.runROIMaxY - currentData.runROIMinY);
    cvConstrainBlobMinMax(blobs, currentData.runROIMinX, currentData.runROIMinY, currentData.runROIMaxX - currentData.runROIMinX, currentData.runROIMaxY - currentData.runROIMinY);
    cvUpdateTracks(blobs, tracks, 5, 10);
    
    if (bShowLabels){
        
        if (currentData.stitchedDisplayMode == 's') dataImage = stitchedGrayImage;
        if (currentData.stitchedDisplayMode == 'd') dataImage = differenceGrayImage;
        if (currentData.stitchedDisplayMode == 't') dataImage = thresholdGrayImage;
        
        cvRenderBlobs(labelImage, blobs, dataImage.getCvImage(), dataImage.getCvImage());
        cvRenderTracks(tracks, dataImage.getCvImage(), dataImage.getCvImage(), CV_TRACK_RENDER_ID|CV_TRACK_RENDER_BOUNDING_BOX|CV_TRACK_RENDER_TO_LOG);
    }
}

//--------------------------------------------------------------
void flexcamApp::updateOscSend()
{
    for (map<CvID, CvTrack *>::iterator it = tracks.begin(); it!=tracks.end(); ++it){
        
        bool sendBlob = false;
        bool foundBlob = false;
        bool ignoreBlob = false;
        
        flexcamBlob thisBlob;
        thisBlob.id 				= (*it).first;
        thisBlob.width 				= (*it).second->maxx-(*it).second->minx;
        thisBlob.height 			= (*it).second->maxy-(*it).second->miny;
        thisBlob.centroidX 			= (double)((*it).second->centroid.x-currentData.runROIMinX)/(currentData.runROIMaxX - currentData.runROIMinX);
        thisBlob.centroidY 			= (double)((*it).second->centroid.y-currentData.runROIMinY)/(currentData.runROIMaxY - currentData.runROIMinY);
        
        thisBlob.angle 				= (double)(*it).second->angleDegrees;
        thisBlob.angleRadians 			= (double)(*it).second->angle;
        
        thisBlob.lifetime  			= (*it).second->lifetime;
        thisBlob.active    			= (*it).second->active;
        thisBlob.inactive  			= (*it).second->inactive;
        thisBlob.positionX			= (int)(*it).second->centroid.x;
        thisBlob.positionY			= (int)(*it).second->centroid.y;
        
        thisBlob.minX				= (int)(*it).second->minx;
        thisBlob.minY				= (int)(*it).second->miny;
        thisBlob.maxX				= (int)(*it).second->maxx;
        thisBlob.maxY				= (int)(*it).second->maxy;
        
        thisBlob.lostTracking		= 0;
        thisBlob.unmoved			= 0;
        thisBlob.blobActive			= 1;
        
        for (int x = 0; x < flexcamBlobs.size(); x++){
            if (flexcamBlobs[x].id == (*it).first){
                foundBlob = true;
                if (flexcamBlobs[x].width != thisBlob.width){
                    if (abs((int)(thisBlob.width - flexcamBlobs[x].width)) < 1) flexcamBlobs[x].width = thisBlob.width;
                    else flexcamBlobs[x].width = (int)(flexcamBlobs[x].width*(1-currentData.blobSmoothing)+thisBlob.width*currentData.blobSmoothing);
                    sendBlob = true;
                }
                if (flexcamBlobs[x].height != thisBlob.height){
                    if (abs((int)(thisBlob.height - flexcamBlobs[x].height)) < 1) flexcamBlobs[x].height = thisBlob.height;
                    else flexcamBlobs[x].height = (int)(flexcamBlobs[x].height*(1-currentData.blobSmoothing) + thisBlob.height*currentData.blobSmoothing);
                    sendBlob = true;
                }
                
                if (flexcamBlobs[x].centroidX != thisBlob.centroidX){
                    if (abs(thisBlob.centroidX - flexcamBlobs[x].centroidX) < 0.00001){
                        flexcamBlobs[x].centroidX = thisBlob.centroidX;
                        flexcamBlobs[x].lastCentroidX = flexcamBlobs[x].centroidX;
                    }
                    else {
                        flexcamBlobs[x].lastCentroidX = flexcamBlobs[x].centroidX;
                        flexcamBlobs[x].centroidX = flexcamBlobs[x].centroidX*(1-currentData.blobSmoothing) + thisBlob.centroidX*currentData.blobSmoothing;
                        sendBlob = true;
                    }
                }
                
                if (flexcamBlobs[x].centroidY != thisBlob.centroidY){
                    if (abs(thisBlob.centroidY - flexcamBlobs[x].centroidY) < 0.00001){
                        flexcamBlobs[x].centroidY = thisBlob.centroidY;
                        flexcamBlobs[x].lastCentroidY = flexcamBlobs[x].centroidY;
                    }
                    else {
                        flexcamBlobs[x].lastCentroidY = flexcamBlobs[x].centroidY;
                        flexcamBlobs[x].centroidY = flexcamBlobs[x].centroidY*(1-currentData.blobSmoothing) + thisBlob.centroidY*currentData.blobSmoothing;
                        sendBlob = true;
                    }
                }
                
                if (flexcamBlobs[x].angle != thisBlob.angle){
                    
                    if (abs(thisBlob.angle - flexcamBlobs[x].angle) < 0.1){
                        flexcamBlobs[x].angle = thisBlob.angle;
                        flexcamBlobs[x].lastAngle = flexcamBlobs[x].angle;
                    }
                    else {
                        flexcamBlobs[x].lastAngle = flexcamBlobs[x].angle;
                        if ((thisBlob.angle > 45) && (flexcamBlobs[x].lastAngle < -45)){
                            flexcamBlobs[x].angle =  flexcamBlobs[x].lastAngle*(1-currentData.blobSmoothing) + (thisBlob.angle - 180)*currentData.blobSmoothing;  //thisBlob.angle - 180
                            if (flexcamBlobs[x].angle < -90) flexcamBlobs[x].angle = flexcamBlobs[x].angle + 180;
                        }
                        else if ((thisBlob.angle < -45) && (flexcamBlobs[x].lastAngle > 45)){
                            flexcamBlobs[x].angle =  flexcamBlobs[x].lastAngle*(1-currentData.blobSmoothing) + (thisBlob.angle + 180)*currentData.blobSmoothing;  //thisBlob.angle - 180
                            if (flexcamBlobs[x].angle > 90) flexcamBlobs[x].angle = flexcamBlobs[x].angle - 180;
                        }
                        else	flexcamBlobs[x].angle = flexcamBlobs[x].lastAngle*(1-currentData.blobSmoothing) + thisBlob.angle*currentData.blobSmoothing;
                        
                        sendBlob = true;
                    }
                    flexcamBlobs[x].angleRadians = thisBlob.angleRadians;
                }
                
                if (sendBlob){
                    flexcamBlobs[x].area = (int)(flexcamBlobs[x].width*flexcamBlobs[x].height);
                    flexcamBlobs[x].aspectRatio = (double)flexcamBlobs[x].width/(double)flexcamBlobs[x].height;
                    flexcamBlobs[x].widthROI = (double)((double)flexcamBlobs[x].width/(currentData.runROIMaxX - currentData.runROIMinX));
                    flexcamBlobs[x].heightROI = (double)((double)flexcamBlobs[x].height/(currentData.runROIMaxY - currentData.runROIMinY));
                    flexcamBlobs[x].lastAreaROI = flexcamBlobs[x].areaROI;
                    flexcamBlobs[x].areaROI = (double)flexcamBlobs[x].area/((currentData.runROIMaxX - currentData.runROIMinX)*(currentData.runROIMaxY - currentData.runROIMinY));
                    flexcamBlobs[x].areaDiffROI = flexcamBlobs[x].areaROI - flexcamBlobs[x].lastAreaROI;
                    if ((flexcamBlobs[x].areaDiffROI < 0.00001)&& (flexcamBlobs[x].areaDiffROI > -0.00001)) flexcamBlobs[x].areaDiffROI = 0;
                    
                    flexcamBlobs[x].centroidXDiff = flexcamBlobs[x].centroidX-flexcamBlobs[x].lastCentroidX;
                    if ((flexcamBlobs[x].centroidXDiff < 0.0001) && (flexcamBlobs[x].centroidXDiff > -0.0001)) flexcamBlobs[x].centroidXDiff = 0;
                    
                    flexcamBlobs[x].centroidYDiff = flexcamBlobs[x].centroidY-flexcamBlobs[x].lastCentroidY;
                    if ((flexcamBlobs[x].centroidYDiff < 0.0001) && (flexcamBlobs[x].centroidYDiff > -0.0001)) flexcamBlobs[x].centroidYDiff = 0;
                    
                    if ((flexcamBlobs[x].centroidDiffDist < 0.0001) && (flexcamBlobs[x].centroidDiffDist > -0.0001)) flexcamBlobs[x].centroidDiffDist = 0;
                    flexcamBlobs[x].centroidDiffDist = (double)sqrt(pow(flexcamBlobs[x].centroidXDiff,2) + pow(flexcamBlobs[x].centroidYDiff,2));
                    
                    if ((flexcamBlobs[x].lastAngle < -45) && (flexcamBlobs[x].angle > 45))
                        flexcamBlobs[x].angleDiff = (flexcamBlobs[x].angle - 180) - flexcamBlobs[x].lastAngle;
                    else if ((flexcamBlobs[x].lastAngle > 45) && (flexcamBlobs[x].angle < -45))
                        flexcamBlobs[x].angleDiff = (flexcamBlobs[x].angle + 180) - flexcamBlobs[x].lastAngle;
                    else flexcamBlobs[x].angleDiff  = flexcamBlobs[x].angle - flexcamBlobs[x].lastAngle;
                    
                    if ((flexcamBlobs[x].angleDiff < 0.01) && (flexcamBlobs[x].angleDiff > -0.01)) flexcamBlobs[x].angleDiff = 0;
                    
                    if (flexcamBlobs[x].lifetime != thisBlob.lifetime){flexcamBlobs[x].lifetime = thisBlob.lifetime;}
                    if (flexcamBlobs[x].active != thisBlob.active){flexcamBlobs[x].active = thisBlob.active;}
                    if (flexcamBlobs[x].inactive != thisBlob.inactive){flexcamBlobs[x].inactive = thisBlob.inactive;}
                    if (flexcamBlobs[x].positionX != thisBlob.positionX){flexcamBlobs[x].positionX = (int)(flexcamBlobs[x].positionX*(1-currentData.blobSmoothing) + thisBlob.positionX*currentData.blobSmoothing);}
                    if (flexcamBlobs[x].positionY != thisBlob.positionY){flexcamBlobs[x].positionY = (int)(flexcamBlobs[x].positionY*(1-currentData.blobSmoothing) + thisBlob.positionY*currentData.blobSmoothing);}
                    
                    if (flexcamBlobs[x].minX != thisBlob.minX){flexcamBlobs[x].minX = (int)(flexcamBlobs[x].minX*(1-currentData.blobSmoothing) + thisBlob.minX*currentData.blobSmoothing);}
                    if (flexcamBlobs[x].minY != thisBlob.minY){flexcamBlobs[x].minY = (int)(flexcamBlobs[x].minY*(1-currentData.blobSmoothing) + thisBlob.minY*currentData.blobSmoothing);}
                    if (flexcamBlobs[x].maxX != thisBlob.maxX){flexcamBlobs[x].maxX = (int)(flexcamBlobs[x].maxX*(1-currentData.blobSmoothing) + thisBlob.maxX*currentData.blobSmoothing);}
                    if (flexcamBlobs[x].minY != thisBlob.maxY){flexcamBlobs[x].maxY = (int)(flexcamBlobs[x].maxY*(1-currentData.blobSmoothing) + thisBlob.maxY*currentData.blobSmoothing);}
                    
                    flexcamBlobs[x].lengthLine = sqrt((flexcamBlobs[x].maxX-flexcamBlobs[x].minX)*(flexcamBlobs[x].maxX-flexcamBlobs[x].minX) +
                                                      (flexcamBlobs[x].maxY-flexcamBlobs[x].minY)*(flexcamBlobs[x].maxY-flexcamBlobs[x].minY));
                    
                    flexcamBlobs[x].lineX1 = flexcamBlobs[x].positionX - (flexcamBlobs[x].lengthLine/2*cos(flexcamBlobs[x].angleRadians));
                    flexcamBlobs[x].lineY1 = flexcamBlobs[x].positionY - (flexcamBlobs[x].lengthLine/2*sin(flexcamBlobs[x].angleRadians));
                    flexcamBlobs[x].lineX2 = flexcamBlobs[x].positionX + (flexcamBlobs[x].lengthLine/2*cos(flexcamBlobs[x].angleRadians));
                    flexcamBlobs[x].lineY2 = flexcamBlobs[x].positionY + (flexcamBlobs[x].lengthLine/2*sin(flexcamBlobs[x].angleRadians));
                }
                flexcamBlobs[x].lostTracking = thisBlob.lostTracking;
            }
            //update existing blob////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if ((foundBlob)&&(sendBlob)) {
                for (int i = 0; i < oscSends.size(); i++ ) oscSends[i]->messageBlobSend(flexcamBlobs[x]);
                break;
            }
        }
        
        //new blob////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
        if (!foundBlob) {
            
            thisBlob.area				    = thisBlob.width * thisBlob.height;
            thisBlob.aspectRatio			    = (double)((double)thisBlob.width/(double)thisBlob.height);
            thisBlob.widthROI 			    = (double)((double)thisBlob.width/(currentData.runROIMaxX - currentData.runROIMinX));
            thisBlob.heightROI			    = (double)((double)thisBlob.height/(currentData.runROIMaxY - currentData.runROIMinY));
            thisBlob.areaROI			    = (double)(*it).second->area/((currentData.runROIMaxX - currentData.runROIMinX)*(currentData.runROIMaxY - currentData.runROIMinY));
            thisBlob.areaDiffROI			= 0;
            thisBlob.centroidXDiff			= 0;
            thisBlob.centroidYDiff			= 0;
            thisBlob.centroidDiffDist		= 0;
            thisBlob.angleDiff			    = 0;
            
            flexcamBlobs.push_back(thisBlob);
            for (int i = 0; i < oscSends.size(); i++ ) oscSends[i]->messageBlobSend(thisBlob);
        }
    }
    
    for (int x = 0; x < flexcamBlobs.size(); x++){
        flexcamBlobs[x].lostTracking++;
        if ((flexcamBlobs[x].centroidDiffDist == 0) && (flexcamBlobs[x].areaDiffROI == 0)) flexcamBlobs[x].unmoved++;
        else flexcamBlobs[x].unmoved = 0;
        
        if (flexcamBlobs[x].lostTracking >= UINT_MAX) flexcamBlobs[x].lostTracking = 0;
        if (flexcamBlobs[x].unmoved >= UINT_MAX) flexcamBlobs[x].unmoved = 0;
    }
    
    //delete blob////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (int x = 0; x < flexcamBlobs.size(); x++){
        if(flexcamBlobs[x].lostTracking > 2){
            flexcamBlobs[x].blobActive = 0;
            for (int i = 0; i < oscSends.size(); i++ ) oscSends[i]->messageBlobSend(flexcamBlobs[x]);
            flexcamBlobs.erase(flexcamBlobs.begin()+x);
            break;
        }
    }
}

float flexcamApp::updateActivityLevel(ofxCvGrayscaleImage *activityFrame)
{
    bool		isWhite;
    uchar		*data 	= (uchar *) activityFrame->getCvImage()->imageData;
    int			pixelCount = 0;
    
    for(int i=0; i < activityFrame->height; i++){
        for(int j=0; j < activityFrame->width; j++){
            isWhite = false;
            if (data[i*activityFrame->getCvImage()->widthStep+j]==255) isWhite = true;
            if (isWhite) pixelCount++;
        }
    }
    
    return (float)((float)pixelCount/(float)(activityFrame->width*activityFrame->height));
}

void flexcamApp::updateAutoBackgrounding()
{
    noMotionTimer++;
    
    if ((activityLevel > AUTO_BACK_ACTIVITY)||(flexcamBlobs.size() == 0)) noMotionTimer = 0;
    
    if ((noMotionTimer > APP_FRAMERATE*AUTO_BACK_TIMER) && (flexcamBlobs.size() > 0)){
        noMotionTimer = 0;
        bLearnBackground = true;
    }
}