#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){    
    image.loadImage("demo.jpg");
    w = image.width;
    h = image.height;
    pixels = w*h;
    w2 = w/2;
    h2 = h/2;
    pixels2 = w2*h2;
    updatedImage.allocate(w, h, OF_IMAGE_COLOR);
    
    light.normalize();
    
    tempV.resize(pixels2);
    odata.resize(pixels2);
    ndata.resize(pixels2);
    cdata.resize(pixels2);
    
    for (int i=0; i<pixels2; i++) {
        tempV[i] = 0.0;
        odata[i] = 0.0;
        ndata[i] = 0.0;
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    printf("Framerate: %f \n", ofGetFrameRate());
    
    /*if (ofRandom(1) < 0.05) {
        int rx = (int)ofRandom(w2 - 10) + 5;
        int ry = (int)ofRandom(h2 - 10) + 5;
        for (int x = -5; x < 5; x++){
            for (int y = -5; y < 5; y++){
                int targetPix = (rx + x) + (w2 * (ry + y));
                odata[targetPix] = 100;
            }
        }
    };*/
    
    //--------------------------------------------------------------
    for (int i=0; i<pixels2; i++) {
        tempV[i] = odata[i];
    }
    for (int i=0; i<pixels2; i++) {
        odata[i] = ndata[i];
    }
    for (int i=0; i<pixels2; i++) {
        ndata[i] = tempV[i];
    }
    
    for (int i=0; i<pixels2; i++) {
        int x = i % w2;
        int y = i / w2;

        int threathold = 1;
        if (x > threathold && y > threathold && x < w2 - threathold && y < h2 - threathold){
            float val = (odata[(x-1)+y*w2] + odata[(x+1)+y*w2] + odata[x+(y-1)*w2] + odata[x+(y+1)*w2]) / 2;
            val = val - ndata[x+y*w2];
            val *= 0.96875;
            ndata[x+y*w2] = val;
        }
    }
    //--------------------------------------------------------------
    
    for (int i=0; i<pixels2; i++) {
        int x = i % w2;
        int y = i / w2;
        
        ofVec3f n = ofVec3f(getVal(x - eps, y) - getVal(x + eps, y), getVal(x, y - eps) - getVal(x, y + eps), eps * 2.0);
        n.normalize();
        float spec = (1 - (light.x + n.x)) + (1 - (light.y + n.y));
        spec /= 2;
        
        if (spec > z)
            spec = (spec - z) / (1 - z);
        else
            spec = 0;
        
        spec *= 255.0;
        
        ofColor c = image.getColor(x*2 + n.x * 60, y*2 + n.y * 60);
        c += spec;
        cdata[i] = c;
    }
    unsigned char *pixels = image.getPixels();
    for (int i = 0; i < w; i++){
        for (int j = 0; j < h; j++){
            int ii = (float)i/(float)2;
            int jj = (float)j/(float)2;
            if(ii * jj <= pixels2){
                updatedImage.setColor(i, j, cdata[ii + w2*jj]);
            }
        }
    }
    updatedImage.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    updatedImage.draw(0, 0);
}

//--------------------------------------------------------------
float ofApp::getVal(int x, int y){
    int threathold = 1;
    if (x > threathold && y > threathold && x < w2 - threathold && y < h2 - threathold){
        if(x+y*w2 > pixels2){
            //printf("x: %i, y: %i, e?: %d \n", x,y,x+y*w2);
            return 0;
        }else{
            float a = odata[x+y*w2];
            return a;
        }
    }else{
        return 0;
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //Make ripple
    for (int x = -10; x < 10; x++) {
        for (int y = -10; y < 10; y++) {
            int targetPix = (mouseX/2 + x) + (w/2 * (mouseY/2 + y));
            odata[targetPix] = 5.0;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
