#include "ofApp.h"
#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);

    // Setup de la GUI
    // Setup GUI - Video
    parametrosManejadorVideo.setup("Manejo de video");
    cargar.addListener(this, &ofApp::cargarVideo);
    parametrosManejadorVideo.add(nombreVideoTxtInput.set("Nombre video:", "video.m4v"));
    parametrosManejadorVideo.add(cargar.setup("Cargar."));

    // Setup GUI - Figuras y Smoothing
    cubosButton.addListener(this, &ofApp::cubosButtonPressed);
    prismasButton.addListener(this, &ofApp::prismasButtonPressed);
    esferasButton.addListener(this, &ofApp::esferasButtonPressed);
    factorSmoothing.addListener(this, &ofApp::factorSmoothingChanged);
    desordenInicialSlider.addListener(this, &ofApp::desordenInicialSliderChanged);
    distanciaEntreBloques.addListener(this, &ofApp::distanciaEntreBloquesChanged);
    parametrosTipoFigura.setup("Figuras, Smoothing y otros");
    parametrosTipoFigura.add(factorSmoothing.setup("Vel. cambio", 0.5, 0.01, 1));
    parametrosTipoFigura.add(cubosButton.setup("Cubos"));
    parametrosTipoFigura.add(prismasButton.setup("Prismas"));
    parametrosTipoFigura.add(esferasButton.setup("Esferas"));
    parametrosTipoFigura.add(desordenInicialSlider.setup("Desorden inicial", 0, 0, 10));
    parametrosTipoFigura.add(distanciaEntreBloques.setup("Distancia entre bloques", 0, 0, 100));
    
    // Setup GUI - Instrucciones
    parametrosInstrucciones.setup("Instrucciones");
    const string instrString = "Instrucciones:";
    const string instrString2 = "+ - Aleja o acerca la camara";
    const string instrString3 = "flecha arriba / abajo modifica profundidad";
    const string instrString4 = "W / S mueve camara en eje Y";
    const string instrString9 = "D / A mueve camara en eje X";
    const string instrString5 = "C Activar/desactivar rotacion camara";
    const string instrString6 = "B Activa/desactiva a modo blanco y negro";
    const string instrString7 = "Click mouse en eje X Cambia el tamano de los cubos";
    parametrosInstrucciones.add(instrucciones.setup(instrString));
    parametrosInstrucciones.add(instrucciones2.setup(instrString2));
    parametrosInstrucciones.add(instrucciones3.setup(instrString3));
    parametrosInstrucciones.add(instrucciones4.setup(instrString4));
    parametrosInstrucciones.add(instrucciones9.setup(instrString9));
    parametrosInstrucciones.add(instrucciones5.setup(instrString5));
    parametrosInstrucciones.add(instrucciones6.setup(instrString6));
    parametrosInstrucciones.add(instrucciones7.setup(instrString7));

    gui.setup();
    gui.setSize(480, 600);
    gui.add(&parametrosManejadorVideo);
    gui.add(&parametrosTipoFigura);
    gui.add(&parametrosInstrucciones);

    cargarVideo();

    // Defino la posicion inicial de la camara centrada en el video.
    camPosition.x = videoPlayer.getWidth() / 2;
    camPosition.y = videoPlayer.getHeight() / 2;
    cam.disableMouseInput();

    // Setup de sistema de figuras
    setupSistemaFiguras();
}

void ofApp::setupSistemaFiguras() {
    std::cout << "[i] Setup de sistema de Figuras\n";
    std::vector<std::vector<Figura>> auxiliar;
    columnas = videoPlayer.getWidth()/sizeFigura;
    filas = videoPlayer.getHeight()/sizeFigura;
    for (int i = 0; i < filas; i++) {
        std::vector<Figura> fila;
        for (int j = 0; j < columnas; j++) {
            int offsetDistX = (j+1)*distanciaEntreBloques;
            int offsetDistY = (i+1)*distanciaEntreBloques;
            ofVec3f posicion = ofVec3f(j*sizeFigura+(random()/(RAND_MAX/((int)desordenInicialSlider+1)))+offsetDistX, i*sizeFigura+(random()/(RAND_MAX/((int)desordenInicialSlider+1)))+offsetDistY, 0);
            fila.push_back(Figura(posicion, sizeFigura, profundidad, (float)factorSmoothing, this->tipoFigura));
        }
        auxiliar.push_back(fila);
    }
    sistemaFiguras = auxiliar;
    std::cout << "[i] Termino de configurar sistema de Figuras\n";
}

void ofApp::actualizarProfundidad() {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            sistemaFiguras[i][j].updateProfundidadMaxima(profundidad);
        }
    }
}

void ofApp::actualizarTipoFigura(Figura::enumTipoFigura tipoFigura) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            sistemaFiguras[i][j].updateTipoFigura(tipoFigura);
        }
    }
}

void ofApp::factorSmoothingChanged(float &factor) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            sistemaFiguras[i][j].updateFactorSmoothing(factor);
        }
    }
}

void ofApp::desordenInicialSliderChanged(int &desorden) {
    setupSistemaFiguras();
}

void ofApp::distanciaEntreBloquesChanged(int &desorden) {
    setupSistemaFiguras();
}

void ofApp::cubosButtonPressed() {
    this->tipoFigura = Figura::enumTipoFigura::CUBO;
    this->actualizarTipoFigura(Figura::enumTipoFigura::CUBO);
}

void ofApp::prismasButtonPressed() {
    this->tipoFigura = Figura::enumTipoFigura::PRISMA;
    this->actualizarTipoFigura(Figura::enumTipoFigura::PRISMA);
}

void ofApp::esferasButtonPressed() {
    this->tipoFigura = Figura::enumTipoFigura::ESFERA;
    this->actualizarTipoFigura(Figura::enumTipoFigura::ESFERA);
}

void ofApp::cargarVideo() {
    videoPlayer.load(nombreVideoTxtInput);
    videoPlayer.setVolume(0);
    videoPlayer.setUseTexture(true);
    videoPlayer.play();
    camPosition.x = videoPlayer.getWidth() / 2;
    camPosition.y = videoPlayer.getHeight() / 2;
    cam.setPosition(camPosition);
    setupSistemaFiguras();
}

//--------------------------------------------------------------
void ofApp::update(){
    videoPlayer.update();
    framePixels = videoPlayer.getPixels().getData();
    std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(ofColor(0,0,0));
    cam.setupPerspective();
    cam.setPosition(camPosition);
    if (lookAtCenter) {
        cam.lookAt(ofVec3f(videoPlayer.getWidth()/2, videoPlayer.getHeight()/2, 0));
    }
    cam.begin();    
	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
	ofEnableDepthTest();
        for (int i=0; i<filas; i++) {
            for (int j=0; j<columnas; j++) {
                int x = j*sizeFigura;
                int y = i*sizeFigura;
                int indexBase = y*videoPlayer.getWidth()*3 + x*3;
                float R = framePixels[indexBase];
                float G = framePixels[indexBase+1];
                float B = framePixels[indexBase+2];
                // float brightness = 0.2126 * R + 0.7152 * G + 0.0722 * B;
                sistemaFiguras[i][j].draw(R,G,B);
            }
        }
    ofDisableDepthTest();
    cam.end();
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == OF_KEY_UP) {
        profundidad += 100;
        actualizarProfundidad();
    }

    if (key == OF_KEY_DOWN) {
        profundidad -= 100;
        actualizarProfundidad();
    }

    if (key == OF_KEY_RIGHT) {
        profundidad = -profundidad;
        actualizarProfundidad();
    }

    if (key == '+') {
        camPosition.z += 100;
    }

    if (key == '-') {
        camPosition.z -= 100;
    }

    if (key == 'd') {
        camPosition.x += 100;
    }

    if (key == 'a') {
        camPosition.x -= 100;
    }

    if (key == 'w') {
        camPosition.y -= 20;
    }

    if (key == 's') {
        camPosition.y += 20;
    }

    if (key == 'b') {
        bw = !bw;
    }

    if (key == 'c') {
        lookAtCenter = !lookAtCenter;
    }

    if (key == 'e') {
        cubes = !cubes;
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    sizeFigura = ofMap(ofGetMouseX(), 0, ofGetWidth(), 4, 32, true);
    setupSistemaFiguras();
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