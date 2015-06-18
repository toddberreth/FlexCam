#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "IPVideoGrabber.h"
#include "defines.h"

using ofx::Video::IPVideoGrabber;
using ofx::Video::SharedIPVideoGrabber;

class flexcamIPCam 
{
public:
    flexcamIPCam()
    {
    }
    
    flexcamIPCam(const string& url): _url(url)
    {
    }
    
    flexcamIPCam(
                 const string&  name,
                 const string&  ip,
                 const string&  camtype,
                 const string&  username,
                 const string&  password,
                 const int&     width,
                 const int&     height,
                 const int&     posX,
                 const int&     posY,
                 const int&     roiMinX,
                 const int&     roiMinY,
                 const int&     roiMaxX,
                 const int&     roiMaxY):
                _name(name),_ip(ip),_camtype(camtype),_username(username),_password(password),_width(width), _height(height),_posX(posX),_posY(posY),_roiMinX(roiMinX),_roiMinY(roiMinY),_roiMaxX(roiMaxX),_roiMaxY(roiMaxY)
    {
        _roiX		=		0;
        _roiY       =		0;
        _roiWidth   =		width;
        _roiHeight  =		height;
        
        _camFrame.allocate(width, height);
        
        bNewFrame = bReadyFrame = false;
        bGetFrame = true;
    }
    
    ~flexcamIPCam();
    
    void setName(const std::string& name) { _name = name; }
    string getName() const { return _name; }
    
    void setIP(const std::string& ip) { _ip = ip; }
    string getIP() const { return _ip; }
    
    void setURL(const std::string& url) { _url = url; }
    string getURL() const { return _url; }
    
    void setUsername(const std::string& username) { _username = username; }
    string getUsername() const { return _username; }
    
    void setPassword(const std::string& password) { _password = password; }
    string getPassword() const { return _password; }
    
    ofxCvColorImage getCurrentFrame() const { return _camFrame; }
    
    
    SharedIPVideoGrabber&  setGrabber() {
        
        _videoGrabber = IPVideoGrabber::makeShared();
        _videoGrabber->setCameraName(_name);
        _videoGrabber->setURI(setCameraTypeURL());
        _videoGrabber->setUsername(_username);
        _videoGrabber->setPassword(_password);
        _videoGrabber->connect();
        return _videoGrabber;
    }

    string setCameraTypeURL(){
        string camTypeURL;
        if (_camtype == "axis_206") camTypeURL = "http://"+ _ip + "/axis-cgi/mjpg/video.cgi?resolution=" + ofToString(_width) + "x" + ofToString(_width);
        return camTypeURL;
    }
    
    void update();
    void updateActivityLevel();
    
    bool                        bNewFrame, bGetFrame, bReadyFrame;
    
private:
    string                      _name;
    string                      _ip;
    string                      _url;
    string                      _camtype;
    string                      _username;
    string                      _password;
    int                         _number;
    int                         _width, _height;
    int                         _posX, _posY;                       //position and roi for overall stitching of multiple cameras
    int                         _roiX, _roiY, _roiWidth, _roiHeight;
    int                         _roiMinX, _roiMinY, _roiMaxX, _roiMaxY, _roiStartX, _roiStartY;
    
    bool                        _bCamDown;
    int                         _noFrameCounter;
    
    ofxCvColorImage             _camFrame;                            //most recent valid camera image grabbed - available for stitching
    
    SharedIPVideoGrabber        _videoGrabber;
};