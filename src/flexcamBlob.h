#pragma once

struct flexcamBlob{
	
    u_int		id;								// blob id for tracking												<- output to client

    double 		centroidX, centroidY ; 		    // centroid - X/Y  0.0<->1.0 within ROI	(top left ROI is 0,0)		<- output to client
    double 		lastCentroidX, lastCentroidY;   // previous centroid X/Y - last frame
    double 		centroidXDiff, centroidYDiff; 	// centroid change between frames - X/Y axis -1.0<->1.0 within ROI	<- output to client
    double 		centroidDiffDist; 		        // centroid movement distance between frames						<- output to client
												// !!! needs to be fixed to accommodate for irreg coord space/aspect ratio
    u_int 		width;							// bounding box width in pixels
    u_int 		height;							// bounding box height in pixels
    u_int 		area;							// bounding box area in pixels

    double		aspectRatio;					// ratio of width to height											<- output to client

    double		widthROI;						// width   	0.0<->1.0 within region of interest (ROI)
    double		heightROI;						// height  	0.0<->1.0 within ROI

    double		areaROI;						// area	   	0.0<->1.0 within ROI									<- output to client
												// i.e. a value of 0.25 would mean blob takes up 25% of ROI area, etc.
    double		lastAreaROI;					// previous area - last frame
    double		areaDiffROI;					// +/- area change -1.0<->1.0 within ROI							<- output to client

    double 		angle;							// angle in degrees  -90->90										<- output to client
    double 		lastAngle;						// previous angle - last frame
    double 		angleDiff;						// +/- angle change (in degrees)									<- output to client
    double 		angleRadians; 
    double 		lastAngleRadians; 

    u_int		lifetime;						// indicates how many frames the blob has been in scene.			<- output to client
    u_int		active;							// indicates number of frames that blob has been active from last inactive period.
    u_int 		inactive;						// indicates number of frames that blob has been missing.

    u_int		blobActive;						//																	<- output to client

    u_int		positionX, positionY;
    u_int		minX, minY;				
    u_int		maxX, maxY;
    double		lineX1, lineY1;
    double		lineX2, lineY2;
    double 		lengthLine;

    u_int 		lostTracking;					// Indicates number of frames camBlobs and cvTracking are not reconciled
    u_int 		unmoved;						// Indicates number of frames area and centroid position haven't changed
};

/*
CURRENTLY OUTPUT TO CLIENTS
/blob  (blob object osc "header" can be named in .xml config file, ie /blob, /object, /dancer, /actor etc. etc.)

id
centroidX
centroidY
centroidXDiff
centroidYDiff
centroidDiffDist
aspectRatio
areaROI
areaDiffROI
angle
angleDiff
lifetime
blobActive
*/
