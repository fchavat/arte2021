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
	void setupSistemaFiguras2();
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

	// Para la traslacion de los videos
	ofParameter<int> traslacionX = 0;
	ofParameter<int> traslacionY = 0;
	ofParameter<int> traslacionZ = 20;
	void actualizarTraslacionVideo(int &aux);

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
	ofxToggle bypass;
	ofxToggle showVideo2;

	// Grupo parametros video 2
	ofxGuiGroup parametrosVideo2;

	// Grupo Parametros carga de video 2 y guardar/cargar configuracion
	ofxGuiGroup parametrosManejadorVideo2;
	ofxLabel nombreVideoTxtInput2;
	ofxButton cargarVideoButton2;
	void cargarVideoButtonPressed2();
	// ofxButton guardarConfiguracion2;
	// ofxButton cargarConfiguracion2;

	ofParameter<int> profundidad2 = 100;
	void profundidadChanged2(int &profundidad2);
	void actualizarProfundidad2();

	ofxIntSlider sizeFigura2;
	void sizeFiguraChanged2(int &sizeFigura2);

	ofxFloatSlider factorSmoothingSlider2;
	void factorSmoothingChanged2(float &factor);
	ofxFloatSlider factorColorSmoothingSlider2;
	void factorColorSmoothingChanged2(float &factor);
	ofxIntSlider desordenInicialSlider2;
	void desordenInicialSliderChanged2(int &desorden);

	ofxGuiGroup parametrosTipoFigura2;
	
	ofxButton cubosButton2;
	void cubosButtonPressed2();
	ofxButton prismasButton2;
	void prismasButtonPressed2();
	ofxButton esferasButton2;
	void esferasButtonPressed2();
	void actualizarTipoFigura2(Figura::enumTipoFigura);


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
		int capa;
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
	std::map<int, std::map<std::string, float>> factoresVinculaciones = {
		{1, {
			{"Profundidad figuras", 0}, 
			{"Tamano figuras", 0}, 
			{"Despl. camara X", 0}, 
			{"Despl. camara Y", 0}, 
			{"Despl. camara Z", 0}
		}},
		{2, {
			{"Profundidad figuras", 0}, 
			{"Tamano figuras", 0}, 
			{"Despl. camara X", 0}, 
			{"Despl. camara Y", 0}, 
			{"Despl. camara Z", 0}
		}}
	};
	std::map<int, std::map<std::string, float>> feedbackVinculaciones = {
		{1, {
			{"Profundidad figuras", 0.99}, 
			{"Tamano figuras", 0.99}, 
		}},
		{2, {
			{"Profundidad figuras", 0.99}, 
			{"Tamano figuras", 0.99},
		}},
	};
	std::map<std::string, float> feedbackVinculacionesGeneral = {
		{"Despl. camara X", 0.99}, 
		{"Despl. camara Y", 0.99}, 
		{"Despl. camara Z", 0.99}
	};

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

	// ##############################
	// ##########   2nd video #######
	// ##############################
	ofVideoPlayer videoPlayer2;
	ofPixels framePixels2;
	
	// Sistema de figuras independientes 2
	void cargarVideo2();
	int columnas2;
	int filas2;
	Figura::enumTipoFigura tipoFigura2 = Figura::enumTipoFigura::CUBO;
	std::vector<std::vector<Figura>> sistemaFiguras2;
	std::vector<Figura*> refsSistemaFiguras2;

	//Video bypass
	ofPlanePrimitive plane;
	ofImage texture;
};
