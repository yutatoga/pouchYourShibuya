#pragma once

#define VIDEO_NUMBER 4
#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofVideoPlayer videoPlayers[VIDEO_NUMBER];
		ofVideoPlayer waitingVideoPlayer;
		
		//arduino
		int valueA0;
		int valueD2;
		int valueD3;
		int valueD4;
		int valueD5;
		
		ofArduino arduino;
		bool		bSetupArduino;			// flag variable for setting up arduino once
private:
    
    void setupArduino(const int & version);
    void digitalPinChanged(const int & pinNum);
    void analogPinChanged(const int & pinNum);
		void updateArduino();
		string potValue;
		string buttonState;
		
		ofImage waitingImage;
};
