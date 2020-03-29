#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    prev.allocate(ofGetWidth(), ofGetHeight());
    prev.begin();
    ofClear(0, 0, 0, 255);
    ofFill();
    // ofColor(ofColor::white);
    ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, 1);
    prev.end();

    curr.allocate(ofGetWidth(), ofGetHeight());
    curr.begin();
    ofClear(0, 0, 0, 255);
    curr.end();
}

//--------------------------------------------------------------
void ofApp::update(){
    curr.readToPixels(currPixels);
    prev.readToPixels(prevPixels);

    for (int x = 1; x < ofGetWidth(); x++) {
        for (int y = 1; y < ofGetHeight(); y++) {
            ofColor newColor = ((
                prevPixels.getColor(x, y - 1) +
                prevPixels.getColor(x, y + 1) +
                prevPixels.getColor(x - 1, y) +
                prevPixels.getColor(x + 1, y)
            ) / 2 - currPixels.getColor(x, y)) * dampening;
            
            currPixels.setColor(x, y, newColor);
        }
    }

    ofPixels tempPixels = prevPixels;
    prevPixels = currPixels;
    currPixels = tempPixels;

    currImage.setFromPixels(currPixels);
    currImage.update();

    prevImage.setFromPixels(prevPixels);
    prevImage.update();

    curr.begin();
    ofClear(0, 0, 0, 255);
    currImage.draw(0, 0);
    curr.end();

    prev.begin();
    ofClear(0, 0, 0, 255);
    prevImage.draw(0, 0);
    prev.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);

    curr.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}
