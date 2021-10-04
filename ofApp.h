#pragma once

#include "ofMain.h"
#include <vector>
#include "figura.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
	public:
		
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void cargarVideo();
	void setupSistemaFiguras();
	void actualizarProfundidad();


	ofEasyCam cam;
	ofShader shader;
	ofFbo fbo;
	ofVideoPlayer movie;
	ofVideoPlayer videoPlayer;

	int profundidad = 100;
	int zoom = 300;
	float sizeFigura = 16;
	ofVec3f lookAtNode = ofVec3f(ofGetWidth()/2, ofGetHeight()/2, 0);
	ofVec3f camPosition = ofVec3f(ofGetWidth()/2, ofGetHeight()/2, 600);
	unsigned char * framePixels;
	bool bw = false;
	bool lookAtCenter = true;
	bool cubes = true;

	// ##############################
	// ##########   ofxGui    #######
	// ##############################
	ofxPanel gui;
	// Grupo Parametros carga de video
	ofxGuiGroup parametrosManejadorVideo;
	ofParameter<std::string> nombreVideoTxtInput;
	ofxButton cargar;

	// Grupo Parametros tipo de figura y smoothing
	ofxGuiGroup parametrosTipoFigura;
	ofxButton cubosButton;
	void cubosButtonPressed();
	ofxButton prismasButton;
	void prismasButtonPressed();
	ofxButton esferasButton;
	void esferasButtonPressed();
	void actualizarTipoFigura(Figura::enumTipoFigura);
	ofxFloatSlider factorSmoothing;
	void factorSmoothingChanged(float &factor);
	ofxIntSlider desordenInicialSlider;
	void desordenInicialSliderChanged(int &desorden);
	ofxIntSlider distanciaEntreBloques;

	ofxGuiGroup parametrosFactorTamanoPorBrillo;
	void distanciaEntreBloquesChanged(int &distancia);
	ofxToggle tamanoPorBrillo;
	ofxFloatSlider tamanoPorBrilloMinimo;
	ofxFloatSlider tamanoPorBrilloMaximo;

	// Grupo parametros de instrucciones
	ofxGuiGroup parametrosInstrucciones;
	ofxLabel instrucciones;
	ofxLabel instrucciones2;
	ofxLabel instrucciones3;
	ofxLabel instrucciones4;
	ofxLabel instrucciones5;
	ofxLabel instrucciones6;
	ofxLabel instrucciones7;
	ofxLabel instrucciones9;
	// ##############################
	// ##############################
	// ##############################

	// Sistema de figuras independientes
	int columnas;
	int filas;
	Figura::enumTipoFigura tipoFigura = Figura::enumTipoFigura::CUBO;
	std::vector<std::vector<Figura>> sistemaFiguras;

};
