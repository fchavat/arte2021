#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"


//========================================================================
int main( ){

	ofAppGLFWWindow win;
	win.setNumSamples(8);
	// win.setMultiDisplayFullscreen(true);
	//win.setDepthBits(24);

	ofSetupOpenGL(&win, 3840, 1080, OF_WINDOW);	// <-------- setup the GL context

	ofRunApp( new ofApp());

}
