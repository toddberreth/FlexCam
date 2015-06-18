#include "flexcamOSC.h"

//--------------------------------------------------------------
flexcamOSC::flexcamOSC()
{
}

//--------------------------------------------------------------
flexcamOSC::flexcamOSC(string ipAddress, int port, string blobName)
{
	oscSendIP = ipAddress;
	oscSendPort = port;
	oscBlobName = "/" + blobName;
	transmitSocket = new UdpTransmitSocket( IpEndpointName(oscSendIP.c_str(), oscSendPort) );
}

//--------------------------------------------------------------
flexcamOSC::~flexcamOSC()
{
	if (transmitSocket != NULL) delete transmitSocket;
}

//--------------------------------------------------------------
void flexcamOSC::messageBlobSend(flexcamBlob myBlob)
{
	char buffer[OUTPUT_BUFFER_SIZE];
	osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

	p << osc::BeginBundleImmediate; p << osc::BeginMessage(oscBlobName.c_str());
		p << (float)myBlob.id << (float)myBlob.centroidX << (float)myBlob.centroidY;
		p << (float)myBlob.centroidXDiff << (float)myBlob.centroidYDiff << (float)myBlob.centroidDiffDist;
		p << (float)myBlob.aspectRatio << (float)myBlob.areaROI << (float)myBlob.areaDiffROI;
		p << (float)myBlob.angle << (float)myBlob.angleDiff << (float)myBlob.lifetime << (float)myBlob.blobActive;
	p << osc::EndMessage; p << osc::EndBundle;
	transmitSocket->Send( p.Data(), p.Size() );
}

//--------------------------------------------------------------
void flexcamOSC::messageActivitySend(string header, int number, float activityLevel)
{
	char buffer[OUTPUT_BUFFER_SIZE];
	osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );
	
	p << osc::BeginBundleImmediate; p << osc::BeginMessage(header.c_str());
		p << (float)number << activityLevel;
	p << osc::EndMessage; p << osc::EndBundle;
    transmitSocket->Send( p.Data(), p.Size() );
}

//--------------------------------------------------------------
void flexcamOSC::messageAllActivitySend(float activityLevel)
{
	char buffer[OUTPUT_BUFFER_SIZE];
	osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );
	
	p << osc::BeginBundleImmediate; p << osc::BeginMessage("all");
		p << activityLevel;
	p << osc::EndMessage; p << osc::EndBundle;
    transmitSocket->Send( p.Data(), p.Size() );
}