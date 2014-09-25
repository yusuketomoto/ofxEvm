#include "ofMain.h"

#include "ofxEvm.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp
{
    
	ofVideoGrabber video;
	ofxEvm evm;
    
	ofxPanel gui;
	ofxFloatSlider alpha;
	ofxFloatSlider cutoff;
	ofxFloatSlider min_freq;
	ofxFloatSlider max_freq;
	ofxFloatSlider chrome;
    
public:
	void setup()
	{
		ofSetFrameRate(60);
		ofSetVerticalSync(true);
		ofBackground(0);
        
		video.initGrabber(640, 480);
        
		gui.setup();
		gui.add(alpha.setup("alpha", 10, 0, 20));
		gui.add(cutoff.setup("cutoff", 16, 0, 20));
		gui.add(min_freq.setup("min_freq", 0.05, 0, 2));
		gui.add(max_freq.setup("max_freq", 0.4, 0, 2));
		gui.add(chrome.setup("chrome", 0.1, 0, 2));
	}
	void update()
	{
		video.update();
        
		updateParams();
		if (video.isFrameNew())
		{
			evm.update(video);
		}
	}
	void draw()
	{
		video.draw(0, 0);
		gui.draw();
		ofTranslate(video.width, 0);
		evm.draw();
	}
	void updateParams()
	{
		evm.amplification(alpha);
		evm.cutoff(cutoff);
		evm.freqMin(min_freq);
		evm.freqMax(max_freq);
		evm.chromeAttenuation(chrome);
	}
};

//========================================================================
int main()
{
	ofSetupOpenGL(1280, 480, OF_WINDOW);  // <-------- setup the GL context
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
}
