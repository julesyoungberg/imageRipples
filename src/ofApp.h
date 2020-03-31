#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();
 
		void mousePressed(int x, int y, int button);
 
	private:
		void applyOutline();
        float getVal(int x, int y);
        void ripple();
		int pixelIndex(int x, int y);
    
        int imgWidth, imgHeight, numPixels;
        ofImage image, updatedImage, outline, object;
    
		// ripple buffers
        vector<float> prev, curr, temp;

		ofVec3f light = ofVec3f(1, 1, 0);

		ofxPanel gui;
		ofxFloatSlider eps, z, dampening, factor;
};
