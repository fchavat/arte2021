#pragma once

#include "ofMain.h"
#include <vector>
#include "figura.h"
#include "ofxGui.h"
#include "ofxImGui.h"
#include "ofxOpenCv.h"
#include "ofxXmlSettings.h"
#include "ofxScreenSetup.h"



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

	bool hideGUI = false;
	// ##############################
	// ##########   ofxGui    #######
	// ##############################
	ofxPanel gui;
	void cargarPresetConfiguracion();
	void guardarPresetConfiguracion();
	// Grupo Parametros carga de video y guardar/cargar configuracion
	ofxGuiGroup parametrosManejadorVideo;
	ofxLabel nombreVideoTxtInput;
	// ofParameter<std::string> nombreVideoTxtInput;
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

	ofxGuiGroup parametrosFactorTamanoPorBrillo;
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

	// Sistema de modulacion de entradas y parametros
	struct afectacion {
		int parametroAfectado;
		float valorEntrada;
		float amplificacion;
	};

	struct vinculacion {
		string entrada;
		int indEntrada;
		std::vector<afectacion> parametrosAfectados;
	};
	std::vector<vinculacion> vinculaciones;
	std::vector<int> cosas;
	const char* entradasValidas[2] = {"Sonido", "Camara"};
	const char* parametrosValidos[5] = {"Profundidad figuras", "Tamano figuras", "Despl. camara X", "Despl. camara Y", "Despl. camara Z"};
	std::map<std::string, float> factoresVinculaciones = {
		{"Profundidad figuras", 0}, 
		{"Tamano figuras", 0}, 
		{"Despl. camara X", 0}, 
		{"Despl. camara Y", 0}, 
		{"Despl. camara Z", 0}
	};
	std::map<std::string, float> feedbackVinculaciones = {
		{"Profundidad figuras", 0.99}, 
		{"Tamano figuras", 0.99}, 
		{"Despl. camara X", 0.99}, 
		{"Despl. camara Y", 0.99}, 
		{"Despl. camara Z", 0.99}
	};;
	float factorDesplazamientoX = 0;
	float factorDesplazamientoY = 0;
	float factorDesplazamientoZ = 0;
	float factorProfundidad = 0;
	float factorTamanoFigura = 0;
	// std::map<string, string> entradasVinculadas; // El key va a ser el nombre del vinculo, el valor va a ser 
	// std::map<string, string> entradasCargadas;
	std::map<string, float> entradasSonido;
	bool entradaSonidoIniciada = false;
	void audioIn(ofSoundBuffer & input);
	void configurar_entrada_audio();
	ofSoundStream soundStream;
	vector <float> left;
	vector <float> right;
	vector <float> volHistory;
	int 	bufferCounter;
	int 	drawCounter;
	float smoothedVol;
	float scaledVol;

	// Entrada video
	ofVideoGrabber webCam;
	float camaraCercania = -1;
	bool entradaCamaraIniciada = false;
	void configurar_entrada_camara();
	ofxCvColorImage			colorImg;
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage 	grayDiff;
	ofxCvContourFinder 	contourFinder;
	int 				threshold;
	bool				bLearnBakground;
	ofFbo webCamFBO;
	GLuint webCamTextureSourceID;
	

	ofxImGui::Gui vinculacionesGui;

	ofxScreenSetup ss;
	void setupChanged(ofxScreenSetup::ScreenSetupArg &arg);

};
