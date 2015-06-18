#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#include "defines.h"

#include "flexcamBlob.h"


using namespace std;

class flexcamOSC {

	public:

	flexcamOSC();
	flexcamOSC(string ipAddress, int port, string blobName);
	~flexcamOSC();

	void messageBlobSend(flexcamBlob myBlob);
	void messageActivitySend(string header, int number, float activityLevel);
	void messageAllActivitySend(float activityLevel);

	private:

	UdpTransmitSocket *transmitSocket;
	
	string 	oscSendIP;
	int     oscSendPort;
	string 	oscBlobName;
};