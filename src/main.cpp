#include "ofMain.h"
#include "flexcamApp.h"
#include "defines.h"

//========================================================================
int main( ){
	
    ofSetupOpenGL(INFO_WINDOW_WIDTH, INFO_WINDOW_HEIGHT, OF_WINDOW);
	ofRunApp(new flexcamApp());
}
