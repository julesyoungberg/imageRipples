#include <cmath>

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    image.load("cat.jpg");
    imgWidth = image.getWidth();
    imgHeight = image.getHeight();
    numPixels = imgWidth * imgHeight;
    updatedImage.allocate(imgWidth, imgHeight, OF_IMAGE_COLOR);

    outline.load("outline.jpg");
    outline.setImageType(OF_IMAGE_GRAYSCALE);

    object.load("object.png");
    
    light.normalize();
    
    temp.resize(numPixels);
    prev.resize(numPixels);
    curr.resize(numPixels);
    
    for (int i = 0; i < numPixels; i++) {
        temp[i] = prev[i] = curr[i] = 0.0;
    }

    gui.setup();
    gui.add(eps.setup("eps", 10.0, 0.0, 20.0));
    gui.add(z.setup("z", 1, 0.0, 1.0));
    gui.add(dampening.setup("dampening", 0.9999999999, 0.9, 0.99999999999999999999));
    gui.add(factor.setup("factor", 100, 0, 200));

    applyOutline();
}
 
//--------------------------------------------------------------
void ofApp::update() {
    if ((ofGetFrameNum() % (60 * 1)) == 0) applyOutline();
    
    ripple();
    
    // Make each first ripple
    for (int i = 0; i < numPixels; i++) {
        int x = i % imgWidth;
        int y = i / imgWidth;
        
        ofVec3f n = ofVec3f(
            getVal(x - eps, y) - getVal(x + eps, y), 
            getVal(x, y - eps) - getVal(x, y + eps),
            eps * 2.0
        ).normalize();

        float spec = (1 - (light.x + n.x)) + (1 - (light.y + n.y));
        spec /= 2;
        
        if (spec > z)
            spec = (spec - z) / (1 - z);
        else
            spec = 0;
        
        spec *= 255.0;
        
        // Get "edge" of ripple
        int sourceX = x + n.x * factor;
        int sourceY = y + n.y * factor;
        
        if (sourceX < 0) sourceX = 0;
        else if (sourceX > imgWidth) sourceX = imgWidth;

        if (sourceY < 0) sourceY = 0;
        else if (sourceY > imgHeight) sourceY = imgHeight;

        ofColor c = image.getColor(sourceX, sourceY);
        c += spec; // Make "edge" brighter
        updatedImage.setColor(x, y, c);
    }

    updatedImage.update();
}
 
//--------------------------------------------------------------
void ofApp::draw() {
    updatedImage.draw(0, 0);
    object.draw(0, 0);
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::applyOutline() {
    for (int x = 0; x < imgWidth; x++) {
        for (int y = 0; y < imgHeight; y++) {
            if (x < outline.getWidth() && y < outline.getHeight()) {
                if (outline.getColor(x, y) == ofColor::white) {
                    prev[pixelIndex(x, y)] = 100;
                }
            }
        }
    }
}
 
//--------------------------------------------------------------
// Retrieves value from previous buffer at x and y
float ofApp::getVal(int x, int y) {
    if (x < 1 || y < 1 || x >= imgWidth - 1 || y >= imgHeight - 1) return 0;
    return prev[x + y * imgWidth];
}
 
//--------------------------------------------------------------
void ofApp::ripple() {
    // Store current situation then update.
    temp = prev;
    prev = curr;
    curr = temp;
    
    // Spread
    for (int x = 1; x < imgWidth - 1; x++) {
        for (int y = 1; y < imgHeight - 1; y++) {
            float val = (
                prev[(x - 1) + y * imgWidth] + 
                prev[(x + 1) + y * imgWidth] + 
                prev[x + (y - 1) * imgWidth] + 
                prev[x + (y + 1) * imgWidth]
            ) / 2 - curr[x + y * imgWidth];
            val *= dampening;
            curr[x + y * imgWidth] = val;
        }
    }
}

int ofApp::pixelIndex(int x, int y) {
    return x + y * imgWidth;
}
 
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    // Make ripple 
    for (int x = -10; x < 10; x++) {
        for (int y = -10; y < 10; y++) {
            int targetPix = (mouseX + x) + (imgWidth * (mouseY + y));
            prev[targetPix] = 100;
        }
    }
}
