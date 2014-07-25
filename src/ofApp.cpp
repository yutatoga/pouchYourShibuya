#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
//		ofToggleFullscreen();
		videoPlayers[0].loadMovie("mask.mov");
		videoPlayers[1].loadMovie("umbrella.mov");
		videoPlayers[2].loadMovie("shuriken.mov");
		videoPlayers[3].loadMovie("keyboard.mov");
		
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofBackground(ofColor::lightBlue);
		for (int i = 0; i<4; i++) {
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
		if (currentPlayerId != -1) {
				videoPlayers[currentPlayerId].update();
		}
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
		ofLogNotice("digitalPinChanged!");
		
		if (!arduino.getDigital(pinNum)) {
				switch (pinNum) {
						case 2:
								videoPlayers[currentPlayerId].stop();
								videoPlayers[currentPlayerId].setPosition(0);
								currentPlayerId = 0;
								videoPlayers[currentPlayerId].play();
								break;
						case 3:
								videoPlayers[currentPlayerId].stop();
								videoPlayers[currentPlayerId].setPosition(0);
								currentPlayerId = 1;
								videoPlayers[currentPlayerId].play();
								break;
						case 4:
								videoPlayers[currentPlayerId].stop();
								videoPlayers[currentPlayerId].setPosition(0);
								currentPlayerId = 2;
								videoPlayers[currentPlayerId].play();
								break;
						case 5:
								videoPlayers[currentPlayerId].stop();
								videoPlayers[currentPlayerId].setPosition(0);
								currentPlayerId = 3;
								videoPlayers[currentPlayerId].play();
								break;
						default:
								break;
				}
		}else if(arduino.getDigital(pinNum)){
				videoPlayers[currentPlayerId].stop();
				videoPlayers[currentPlayerId].setPosition(0);
		}
}


//--------------------------------------------------------------
void ofApp::draw(){
		if (currentPlayerId != -1) {
				videoPlayers[currentPlayerId].draw(ofGetWidth()/2.0, ofGetHeight()/2.0, ofGetWidth(), ofGetHeight());
		}
		if (!videoPlayers[0].isPlaying() && !videoPlayers[1].isPlaying() && !videoPlayers[2].isPlaying() && !videoPlayers[3].isPlaying()) {
				waitingImage.draw(ofGetWidth()/2.0, ofGetHeight()/2.0, ofGetWidth(), ofGetHeight());
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
						videoPlayers[currentPlayerId].stop();
						videoPlayers[currentPlayerId].setPosition(0);
						currentPlayerId = 0;
						videoPlayers[currentPlayerId].play();
						break;
				case '2':
						videoPlayers[currentPlayerId].stop();
						videoPlayers[currentPlayerId].setPosition(0);
						currentPlayerId = 1;
						videoPlayers[currentPlayerId].play();
						break;
				case '3':
						videoPlayers[currentPlayerId].stop();
						videoPlayers[currentPlayerId].setPosition(0);
						currentPlayerId = 2;
						videoPlayers[currentPlayerId].play();
						break;
				case '4':
						videoPlayers[currentPlayerId].stop();
						videoPlayers[currentPlayerId].setPosition(0);
						currentPlayerId = 3;
						videoPlayers[currentPlayerId].play();
						break;
				case ' ':
						for (int i = 0; i<4; i++) {
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
