#include "ofApp.h"
#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);

    // Setup de la GUI
    // Setup GUI - Video
    parametrosManejadorVideo.setup("Manejo de video");
    cargarVideoButton.addListener(this, &ofApp::cargarVideoButtonPressed);
    parametrosManejadorVideo.add(nombreVideoTxtInput.setup("Nombre video:", "video.m4v"));
    parametrosManejadorVideo.add(cargarVideoButton.setup("Cargar video"));
    guardarConfiguracion.addListener(this, &ofApp::guardarPresetConfiguracion);
    cargarConfiguracion.addListener(this, &ofApp::cargarPresetConfiguracion);
    parametrosManejadorVideo.add(guardarConfiguracion.setup("Guardar configuracion (tecla s)"));
    parametrosManejadorVideo.add(cargarConfiguracion.setup("Cargar configuracion (tecla l)"));
    // Setup GUI - Figuras y Smoothing
    sizeFigura.addListener(this, &ofApp::sizeFiguraChanged);
    profundidad.addListener(this, &ofApp::profundidadChanged);
    cubosButton.addListener(this, &ofApp::cubosButtonPressed);
    prismasButton.addListener(this, &ofApp::prismasButtonPressed);
    esferasButton.addListener(this, &ofApp::esferasButtonPressed);
    factorSmoothingSlider.addListener(this, &ofApp::factorSmoothingChanged);
    factorColorSmoothingSlider.addListener(this, &ofApp::factorColorSmoothingChanged);
    desordenInicialSlider.addListener(this, &ofApp::desordenInicialSliderChanged);
    parametrosAtributosFigura.setup("Figuras, Smoothing y otros");
    parametrosTipoFigura.setup("Tipo de figura");
    parametrosTipoFigura.add(cubosButton.setup("Cubos"));
    parametrosTipoFigura.add(prismasButton.setup("Prismas"));
    parametrosTipoFigura.add(esferasButton.setup("Esferas"));
    parametrosAtributosFigura.add(&parametrosTipoFigura);
    parametrosAtributosFigura.add(sizeFigura.setup("Tamano figura", 8, 2, 32));
    parametrosAtributosFigura.add(profundidad.set("Profundidad maxima", 0, -1000, 1000));
    parametrosAtributosFigura.add(factorSmoothingSlider.setup("Velocidad smooth movimiento", 0.5, 0.01, 1));
    parametrosAtributosFigura.add(factorColorSmoothingSlider.setup("Velocidad smooth color", 0.5, 0.01, 1));
    parametrosAtributosFigura.add(desordenInicialSlider.setup("Desorden inicial", 0, 0, 10));

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

void ofApp::cargarVideoButtonPressed() {
    ofFileDialogResult res;
    res = ofSystemLoadDialog( "Cargar preset de configuracion" );
    if ( res.bSuccess ) {
        nombreVideoTxtInput = res.filePath;
        cargarVideo();
    }
}

void ofApp::guardarPresetConfiguracion() {
    ofFileDialogResult res;
    res = ofSystemSaveDialog("preset.xml", "Guardar preset de configuracion");
    if ( res.bSuccess ) {
        gui.saveToFile( res.filePath );

        // Ahora guardamos el path al video (a ofxGUI no le gusta guardar los ofxLabel :facepalm:)
        std::ofstream file;
        file.open(res.filePath, std::ios_base::app);
        file << (string)nombreVideoTxtInput;
    }
}

void ofApp::cargarPresetConfiguracion() {
    ofFileDialogResult res;
    res = ofSystemLoadDialog( "Cargar preset de configuracion" );
    if ( res.bSuccess ) {
        gui.loadFromFile( res.filePath );
        // obtenemos aparte el path al video
        std::ifstream conf(res.filePath);
        if (conf.is_open()) {
            std::string line;
            while (std::getline(conf, line)) {}
            // Asignamos el nombre del video a nombreVideoTxtInput
            nombreVideoTxtInput = line;
            conf.close();
        }

        cargarVideo();
    }
}

void ofApp::setupSistemaFiguras() {
    std::cout << "[i] Setup de sistema de Figuras\n";
    std::vector<std::vector<Figura>> auxiliar;
    columnas = videoPlayer.getWidth()/sizeFigura;
    filas = videoPlayer.getHeight()/sizeFigura;
    for (int i = 0; i < filas; i++) {
        std::vector<Figura> fila;
        for (int j = 0; j < columnas; j++) {
            ofVec3f posicion = ofVec3f(j*sizeFigura+(random()/(RAND_MAX/((int)desordenInicialSlider+1))), i*sizeFigura+(random()/(RAND_MAX/((int)desordenInicialSlider+1))), 0);
            Figura fig = Figura(posicion, sizeFigura, profundidad, (float)factorSmoothingSlider, this->tipoFigura);
            fila.push_back(fig);
        }
        auxiliar.push_back(fila);
    }
    sistemaFiguras = auxiliar;

    // Punteros a los elementos del sistema de figuras (se usa para reordenarlo y dibujar segun alpha)
    refsSistemaFiguras.clear();
    for (int i=0; i<filas; i++) {
        for (int j=0; j<columnas; j++) {
            refsSistemaFiguras.push_back(&sistemaFiguras[i][j]);
        }
    }
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
    if (videoPlayer.isFrameNew()) {
        framePixels = videoPlayer.getPixels();
        // Ordenamos las referencias segun alpha para dibujar primero los mas opacos
        std::sort(refsSistemaFiguras.begin(), refsSistemaFiguras.end(), [](const Figura* lhs, const Figura* rhs) {
            return lhs->colorActual.a > rhs->colorActual.a;
        });
        // Actualizamos color y dibujamos
        for (Figura* f : refsSistemaFiguras) {
            ofVec3f pos = f->posActual;
            ofColor color = framePixels.getColor(pos.x, pos.y);
            f->actualizarColorObjetivo(color);
        }
    }
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
    } else {
        navX = cam.getPosition().x;
        navY = cam.getPosition().y;
        navZ = cam.getPosition().z;
    }
    if (lookAtCenter) {
        cam.lookAt(ofVec3f(videoPlayer.getWidth()/2, videoPlayer.getHeight()/2, 0));
    }
    cam.begin();    
	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
    ofEnableDepthTest();
    // Actualizamos color y dibujamos
    for (Figura* f : refsSistemaFiguras) {
        f->draw(tamanoPorBrillo, tamanoPorBrilloMinimo, tamanoPorBrilloMaximo);
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
    if ( key == 's' ) {
        guardarPresetConfiguracion();
    }
    if ( key == 'l' ) {
        cargarPresetConfiguracion();
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