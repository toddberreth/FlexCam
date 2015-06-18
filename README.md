# FlexCam
Motion tracking application. Flexcam provides an mechanism to configure an array of IP cameras, and stitch their collected video output into a single field. The application is also able to use a computer's internal camera as video input, or load and play a local video file.  Integrates background substraction and simple image processing to provide effective "blob" tracking of objects in the field, and send motion data to another local or remote process via OSC messaging. Easy to use and lean.  Camera processes threaded for performance.

Flexible configuration via and .xml configuration file. 

FlexCam was written in C++ using openFrameworks 008+

FlexCam required addon ofxIPVideoGrabber



