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

	ofParameter<int> profundidad = 100;
	int zoom = 300;
	ofPixels framePixels;
	void profundidadChanged(int &profundidad);
	bool bw = false;
	bool cubes = true;

	// ##############################
	// ##########   ofxGui    #######
	// ##############################
	ofxPanel gui;
	void cargarPresetConfiguracion();
	void guardarPresetConfiguracion();
	// Grupo Parametros carga de video y guardar/cargar configuracion
	ofxGuiGroup parametrosManejadorVideo;
	ofParameter<std::string> nombreVideoTxtInput;
	ofxButton cargarVideoButton;
	void cargarVideoButtonPressed();
	ofxButton guardarConfiguracion;
	ofxButton cargarConfiguracion;

	// Grupo Parametros tipo de figura y smoothing
	ofxGuiGroup parametrosAtributosFigura;
	ofxGuiGroup parametrosTipoFigura;
	ofxIntSlider sizeFigura;
	void sizeFiguraChanged(int &sizeFigura);
	ofxButton cubosButton;
	void cubosButtonPressed();
	ofxButton prismasButton;
	void prismasButtonPressed();
	ofxButton esferasButton;
	void esferasButtonPressed();
	void actualizarTipoFigura(Figura::enumTipoFigura);
	ofxFloatSlider factorSmoothingSlider;
	void factorSmoothingChanged(float &factor);
	ofxFloatSlider factorColorSmoothingSlider;
	void factorColorSmoothingChanged(float &factor);
	ofxIntSlider desordenInicialSlider;
	void desordenInicialSliderChanged(int &desorden);
	ofxIntSlider distanciaEntreBloques;

	ofxGuiGroup parametrosFactorTamanoPorBrillo;
	void distanciaEntreBloquesChanged(int &distancia);
	ofxToggle tamanoPorBrillo;
	ofxFloatSlider tamanoPorBrilloMinimo;
	ofxFloatSlider tamanoPorBrilloMaximo;

	// Grupo parametros para navegacion
	ofxGuiGroup parametrosNavegacion;
	ofParameter<float> navX;
	ofParameter<float> navY;
	ofParameter<float> navZ;
	ofxToggle mouseNav;
	ofxToggle lookAtCenter;

	// Sistema de figuras independientes
	int columnas;
	int filas;
	Figura::enumTipoFigura tipoFigura = Figura::enumTipoFigura::CUBO;
	std::vector<std::vector<Figura>> sistemaFiguras;
	std::vector<Figura*> refsSistemaFiguras;

};
