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
    sizeFigura.addListener(this, &ofApp::sizeFiguraChanged);
    profundidad.addListener(this, &ofApp::profundidadChanged);
    cubosButton.addListener(this, &ofApp::cubosButtonPressed);
    prismasButton.addListener(this, &ofApp::prismasButtonPressed);
    esferasButton.addListener(this, &ofApp::esferasButtonPressed);
    factorSmoothing.addListener(this, &ofApp::factorSmoothingChanged);
    factorColorSmoothing.addListener(this, &ofApp::factorColorSmoothingChanged);
    desordenInicialSlider.addListener(this, &ofApp::desordenInicialSliderChanged);
    distanciaEntreBloques.addListener(this, &ofApp::distanciaEntreBloquesChanged);
    parametrosAtributosFigura.setup("Figuras, Smoothing y otros");
    parametrosTipoFigura.setup("Tipo de figura");
    parametrosTipoFigura.add(cubosButton.setup("Cubos"));
    parametrosTipoFigura.add(prismasButton.setup("Prismas"));
    parametrosTipoFigura.add(esferasButton.setup("Esferas"));
    parametrosAtributosFigura.add(&parametrosTipoFigura);
    parametrosAtributosFigura.add(sizeFigura.setup("Tamano figura", 8, 2, 32));
    parametrosAtributosFigura.add(profundidad.set("Profundidad maxima", 0, -1000, 1000));
    parametrosAtributosFigura.add(factorSmoothing.setup("Velocidad smooth movimiento", 0.5, 0.01, 1));
    parametrosAtributosFigura.add(factorColorSmoothing.setup("Velocidad smooth color", 0.5, 0.01, 1));
    parametrosAtributosFigura.add(desordenInicialSlider.setup("Desorden inicial", 0, 0, 10));
    parametrosAtributosFigura.add(distanciaEntreBloques.setup("Distancia entre bloques", 0, 0, 100));

    parametrosFactorTamanoPorBrillo.setup("Parametros tamano segun brillo");
    parametrosFactorTamanoPorBrillo.add(tamanoPorBrillo.setup("Tamano segun brillo", false));
    parametrosFactorTamanoPorBrillo.add(tamanoPorBrilloMinimo.setup("Tamano minimo", 0.5, 0.1, 5));
    parametrosFactorTamanoPorBrillo.add(tamanoPorBrilloMaximo.setup("Tamano maximo", tamanoPorBrilloMinimo, tamanoPorBrilloMinimo, 10));
    parametrosAtributosFigura.add(&parametrosFactorTamanoPorBrillo);
    
    // Setup GUI - Navegacion
    parametrosNavegacion.setup("Parametros para navegar");
    parametrosNavegacion.add(mouseNav.setup("Navegar con mouse (M)", false));
    parametrosNavegacion.add(lookAtCenter.setup("Centrar visual al centro (C)", false));
    parametrosNavegacion.add(navX.set("Eje X", 100, -1500, 3000));
    parametrosNavegacion.add(navY.set("Eje Y", 100, -1500, 3000));
    parametrosNavegacion.add(navZ.set("Eje Z", 100, -1500, 3000));
    
    gui.setup();
    gui.setSize(480, 600);
    gui.add(&parametrosManejadorVideo);
    gui.add(&parametrosAtributosFigura);
    gui.add(&parametrosNavegacion);

    cargarVideo();

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

void ofApp::sizeFiguraChanged(int &sizeFigura) {
    setupSistemaFiguras();
}

void ofApp::profundidadChanged(int &profundidad) {
    actualizarProfundidad();
}

void ofApp::factorColorSmoothingChanged(float &factor) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            sistemaFiguras[i][j].updateFactorColorSmoothing(factor);
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
    videoPlayer.setPixelFormat(OF_PIXELS_RGBA);
    videoPlayer.load(nombreVideoTxtInput);
    videoPlayer.setVolume(0);
    videoPlayer.setUseTexture(true);
    videoPlayer.play();
    
    navX = videoPlayer.getWidth() / 2;
    navY = videoPlayer.getHeight() / 2;
    navZ = 1500;
    setupSistemaFiguras();
}

//--------------------------------------------------------------
void ofApp::update(){
    videoPlayer.update();
    if (videoPlayer.isFrameNew())
        framePixels = videoPlayer.getPixels();
    std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(ofColor(0,0,0));
    if (!mouseNav) {
        cam.setupPerspective();
        cam.setPosition(ofVec3f(navX,navY,navZ));
    }
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
                ofColor color = framePixels.getColor(x, y);
                sistemaFiguras[i][j].draw(color.r, color.g, color.b, color.a, tamanoPorBrillo, tamanoPorBrilloMinimo, tamanoPorBrilloMaximo);
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
        navZ += 100;
    }

    if (key == '-') {
        navZ -= 100;
    }

    if (key == 'd') {
        navX += 20;
    }

    if (key == 'a') {
        navX -= 20;
    }

    if (key == 'w') {
        navY -= 20;
    }

    if (key == 's') {
        navY += 20;
    }

    if (key == 'b') {
        bw = !bw;
    }

    if (key == 'c') {
        lookAtCenter = !lookAtCenter;
    }

    if (key == 'm') {
        mouseNav = !mouseNav;
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