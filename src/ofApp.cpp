#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
		ofToggleFullscreen();
		videoPlayers[0].loadMovie("mask.mov");
		videoPlayers[1].loadMovie("umbrella.mov");
		
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofBackground(ofColor::lightBlue);
		for (int i = 0; i<4; i++) {
				videoPlayers[i].setLoopState(OF_LOOP_NONE);
		}
		ofHideCursor();
}

//--------------------------------------------------------------
void ofApp::update(){
		if (currentPlayerId != -1) {
				videoPlayers[currentPlayerId].update();
		}
}

//--------------------------------------------------------------
void ofApp::draw(){
		if (currentPlayerId != -1) {
				videoPlayers[currentPlayerId].draw(ofGetWidth()/2.0, ofGetHeight()/2.0, ofGetWidth(), ofGetHeight());
		}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
		switch (key) {
				case 'f':
						ofToggleFullscreen();
						break;
				case '0':
						videoPlayers[currentPlayerId].stop();
						videoPlayers[currentPlayerId].setPosition(0);
						currentPlayerId = 0;
						videoPlayers[currentPlayerId].play();
						break;
				case '1':
						videoPlayers[currentPlayerId].stop();
						videoPlayers[currentPlayerId].setPosition(0);
						currentPlayerId = 1;
						videoPlayers[currentPlayerId].play();
						break;
				case ' ':
						//						videoPlayers[currentPlayerId].stop();
						currentPlayerId = -1;
						//						ofPushStyle();
						//						ofSetColor(ofColor::black);
						//						ofRect(0, 0, ofGetWidth(), ofGetHeight());
						//						ofPopStyle();
				default:
						break;
		}
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
