#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
//		ofToggleFullscreen();
		videoPlayers[0].loadMovie("mask.mov");
		videoPlayers[1].loadMovie("omikuji.mov");
		videoPlayers[2].loadMovie("shuriken.mov");
		videoPlayers[3].loadMovie("umbrella.mov");
		
		waitingVideoPlayer.loadMovie("waiting.mov");
		waitingVideoPlayer.setLoopState(OF_LOOP_NORMAL);
		waitingVideoPlayer.setVolume(0);
		waitingVideoPlayer.play();
		
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofBackground(ofColor::lightBlue);
		for (int i = 0; i<VIDEO_NUMBER; i++) {
				videoPlayers[i].setLoopState(OF_LOOP_NONE);
		}
		ofHideCursor();
		
		//arduino
		arduino.connect("/dev/tty.usbmodemfd121", 57600);
//		arduino.connect("/dev/cu.usbserial-A5025WK7", 57600);
		ofAddListener(arduino.EInitialized, this, &ofApp::setupArduino);
		bSetupArduino	= false;
		valueA0 = 0;
		valueD2 = 0;
		
		waitingImage.loadImage("waiting.jpg");
}

void ofApp::setupArduino(const int & version) {
		
		// remove listener because we don't need it anymore
		ofRemoveListener(arduino.EInitialized, this, &ofApp::setupArduino);
    
    // it is now safe to send commands to the Arduino
    bSetupArduino = true;
    
    // print firmware name and version to the console
    ofLogNotice() << arduino.getFirmwareName();
    ofLogNotice() << "firmata v" << arduino.getMajorFirmwareVersion() << "." << arduino.getMinorFirmwareVersion();
		
    // Note: pins A0 - A5 can be used as digital input and output.
    // Refer to them as pins 14 - 19 if using StandardFirmata from Arduino 1.0.
    // If using Arduino 0022 or older, then use 16 - 21.
    // Firmata pin numbering changed in version 2.3 (which is included in Arduino 1.0)
    
    // set pins D2 and A5 to digital input
    arduino.sendDigitalPinMode(2, ARD_INPUT);
    arduino.sendDigitalPinMode(3, ARD_INPUT);
		arduino.sendDigitalPinMode(4, ARD_INPUT);
		arduino.sendDigitalPinMode(5, ARD_INPUT);
		
		arduino.sendDigitalPinMode(19, ARD_INPUT);  // pin 21 if using StandardFirmata from Arduino 0022 or older
		
		
    // set pin A0 to analog input
//    arduino.sendAnalogPinReporting(0, ARD_ANALOG);
    
    // set pin D13 as digital output
		arduino.sendDigitalPinMode(13, ARD_OUTPUT);
    // set pin A4 as digital output
    arduino.sendDigitalPinMode(18, ARD_OUTPUT);  // pin 20 if using StandardFirmata from Arduino 0022 or older
		
    // set pin D11 as PWM (analog output)
		arduino.sendDigitalPinMode(11, ARD_PWM);
    
    // attach a servo to pin D9
    // servo motors can only be attached to pin D3, D5, D6, D9, D10, or D11
    arduino.sendServoAttach(9);
		
    // Listen for changes on the digital and analog pins
    ofAddListener(arduino.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
    ofAddListener(arduino.EAnalogPinChanged, this, &ofApp::analogPinChanged);
		
}

//--------------------------------------------------------------
void ofApp::update(){
		for (int i = 0; i<VIDEO_NUMBER; i++) {
				videoPlayers[i].update();
		}
		waitingVideoPlayer.update();
		updateArduino();
}

void ofApp::updateArduino(){
		arduino.update();
		if (bSetupArduino) {
				valueA0 = arduino.getAnalog(0);
				valueD2 = arduino.getDigital(2);
				valueD3 = arduino.getDigital(3);
				valueD4 = arduino.getDigital(4);
				valueD5 = arduino.getDigital(5);
//				if(valueA0 < 0 || valueA0 > 800) valueA0 = 0;
		}
}

void ofApp::analogPinChanged(const int & pinNum) {
    // do something with the analog input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    potValue = "analog pin: " + ofToString(pinNum) + " = " + ofToString(arduino.getAnalog(pinNum));
}

void ofApp::digitalPinChanged(const int & pinNum) {
    // do something with the digital input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    buttonState = "digital pin: " + ofToString(pinNum) + " = " + ofToString(arduino.getDigital(pinNum));
		ofLogNotice("digitalPinChanged!"+buttonState);
		
		if (!arduino.getDigital(pinNum)) {
				switch (pinNum) {
						case 2:
								videoPlayers[0].setPosition(0);
								videoPlayers[0].play();
								break;
						case 3:
								videoPlayers[1].setPosition(0);
								videoPlayers[1].play();
								break;
						case 4:
								videoPlayers[2].setPosition(0);
								videoPlayers[2].play();
								break;
						case 5:
								videoPlayers[3].setPosition(0);
								videoPlayers[3].play();
								break;
						default:
								break;
				}
		}else if(arduino.getDigital(pinNum)){
				switch (pinNum) {
						case 2:
								videoPlayers[0].stop();
								videoPlayers[0].setPosition(0);
								break;
						case 3:
								videoPlayers[1].stop();
								videoPlayers[1].setPosition(0);
								break;
						case 4:
								videoPlayers[2].stop();
								videoPlayers[2].setPosition(0);
								break;
						case 5:
								videoPlayers[3].stop();
								videoPlayers[3].setPosition(0);
								break;
						default:
								break;
				}
		}
}


//--------------------------------------------------------------
void ofApp::draw(){
		// waiting
		int playingVideoNumber = 0;
		for (int i = 0 ; i<VIDEO_NUMBER; i++) {
				playingVideoNumber += videoPlayers[i].isPlaying();
		}
		if (!playingVideoNumber) {
				waitingVideoPlayer.draw(ofGetWidth()/2.0, ofGetHeight()/2.0, ofGetWidth(), ofGetHeight());
		}else if(playingVideoNumber == 1){
				for (int i = 0; i<VIDEO_NUMBER; i++) {
						if (videoPlayers[i].isPlaying()) {
								videoPlayers[i].draw(ofGetWidth()/2.0, ofGetHeight()/2.0, ofGetWidth(), ofGetHeight());
						}
				}
		}else if(playingVideoNumber > 1 || playingVideoNumber <= 4){
				if (videoPlayers[0].isPlaying()) {
						videoPlayers[0].draw(ofGetWidth()/4.0, ofGetHeight()/4.0, ofGetWidth()/2.0, ofGetHeight()/2.0);
				}else{
						waitingImage.draw(ofGetWidth()/4.0, ofGetHeight()/4.0, ofGetWidth()/2.0, ofGetHeight()/2.0);
				}
				if (videoPlayers[1].isPlaying()) {
						videoPlayers[1].draw(ofGetWidth()*3.0/4.0, ofGetHeight()/4.0, ofGetWidth()/2.0, ofGetHeight()/2.0);
				}else{
						waitingImage.draw(ofGetWidth()*3.0/4.0, ofGetHeight()/4.0, ofGetWidth()/2.0, ofGetHeight()/2.0);
				}
				if (videoPlayers[2].isPlaying()) {
						videoPlayers[2].draw(ofGetWidth()/4.0, ofGetHeight()*3.0/4.0, ofGetWidth()/2.0, ofGetHeight()/2.0);
				}else{
						waitingImage.draw(ofGetWidth()/4.0, ofGetHeight()*3.0/4.0, ofGetWidth()/2.0, ofGetHeight()/2.0);
				}
				if (videoPlayers[3].isPlaying()) {
						videoPlayers[3].draw(ofGetWidth()*3.0/4.0, ofGetHeight()*3.0/4.0, ofGetWidth()/2.0, ofGetHeight()/2.0);
				}else{
						waitingImage.draw(ofGetWidth()*3.0/4.0, ofGetHeight()*3.0/4.0, ofGetWidth()/2.0, ofGetHeight()/2.0);
				}
		}
		
		ofLogNotice(ofToString(valueA0)+ofToString(bSetupArduino)+" D2:"+ofToString(valueD2)+"/"+" D3:"+ofToString(valueD3)+"/"+" D4:"+ofToString(valueD4)+"/"+" D5:"+ofToString(valueD5));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
		switch (key) {
				case 'f':
						ofToggleFullscreen();
						break;
				case '1':
						videoPlayers[0].setPosition(0);
						videoPlayers[0].play();
						break;
				case '2':
						videoPlayers[1].setPosition(0);
						videoPlayers[1].play();
						break;
				case '3':
						videoPlayers[2].setPosition(0);
						videoPlayers[2].play();
						break;
				case '4':
						videoPlayers[3].setPosition(0);
						videoPlayers[3].play();
						break;
				case ' ':
						for (int i = 0; i<VIDEO_NUMBER; i++) {
								videoPlayers[i].stop();
								videoPlayers[i].setPosition(0);
						}
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
