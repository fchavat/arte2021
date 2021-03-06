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

    parametrosAtributosFigura.setup("VIDEO 1 - Figuras, Smoothing y otros");

    parametrosAtributosFigura.add(sizeFigura.setup("Tamano figura", 8, 2, 32));
    parametrosAtributosFigura.add(profundidad.set("Profundidad maxima", 0, -1000, 1000));
    parametrosAtributosFigura.add(factorSmoothingSlider.setup("Velocidad smooth movimiento", 0.5, 0.01, 1));
    parametrosAtributosFigura.add(factorColorSmoothingSlider.setup("Velocidad smooth color", 0.5, 0.01, 1));
    parametrosAtributosFigura.add(desordenInicialSlider.setup("Desorden inicial", 0, 0, 10));

    parametrosTipoFigura.setup("Tipo de figura");
    parametrosTipoFigura.add(cubosButton.setup("Cubos"));
    parametrosTipoFigura.add(prismasButton.setup("Prismas"));
    parametrosTipoFigura.add(esferasButton.setup("Esferas"));
    parametrosAtributosFigura.add(&parametrosTipoFigura);

    parametrosFactorTamanoPorBrillo.setup("Parametros tamano segun brillo");
    parametrosFactorTamanoPorBrillo.add(tamanoPorBrillo.setup("Tamano segun brillo", false));
    parametrosFactorTamanoPorBrillo.add(tamanoPorBrilloMinimo.setup("Tamano minimo", 0.5, 0.1, 5));
    parametrosFactorTamanoPorBrillo.add(tamanoPorBrilloMaximo.setup("Tamano maximo", tamanoPorBrilloMinimo, tamanoPorBrilloMinimo, 10));
    parametrosAtributosFigura.add(&parametrosFactorTamanoPorBrillo);
    
    // Setup GUI - Navegacion
    parametrosNavegacion.setup("Parametros para navegar");
    parametrosNavegacion.add(mouseNav.setup("Navegar con mouse (M)", false));
    parametrosNavegacion.add(lookAtCenter.setup("Centrar visual al centro (C)", false));
    parametrosNavegacion.add(bypass.setup("Bypass video 1", true));
    parametrosNavegacion.add(showVideo2.setup("Show video 2", true));
    parametrosNavegacion.add(navX.set("Eje X", 100, -1500, 3000));
    parametrosNavegacion.add(navY.set("Eje Y", 100, -1500, 3000));
    parametrosNavegacion.add(navZ.set("Eje Z", 100, -1500, 3000));

    // Setup GUI - Video 2
    parametrosVideo2.setup("Parametros para Video 2");

    cargarVideoButton2.addListener(this, &ofApp::cargarVideoButtonPressed2);
    parametrosManejadorVideo2.add(nombreVideoTxtInput2.setup("Nombre video:", "video.mp4"));
    parametrosManejadorVideo2.add(cargarVideoButton2.setup("Cargar video"));
    parametrosVideo2.add(&parametrosManejadorVideo2);
 
    parametrosVideo2.add(sizeFigura2.setup("Tamano figura", 8, 2, 32));
    parametrosVideo2.add(profundidad2.set("Profundidad maxima", 0, -1000, 1000));
    parametrosVideo2.add(factorSmoothingSlider2.setup("Velocidad smooth movimiento", 0.5, 0.01, 1));
    parametrosVideo2.add(factorColorSmoothingSlider2.setup("Velocidad smooth color", 0.5, 0.01, 1));
    parametrosVideo2.add(desordenInicialSlider2.setup("Desorden inicial", 0, 0, 10));

    traslacionX.addListener(this, &ofApp::actualizarTraslacionVideo);
    traslacionY.addListener(this, &ofApp::actualizarTraslacionVideo);
    traslacionZ.addListener(this, &ofApp::actualizarTraslacionVideo);

    sizeFigura2.addListener(this, &ofApp::sizeFiguraChanged2);
    profundidad2.addListener(this, &ofApp::profundidadChanged2);
    cubosButton2.addListener(this, &ofApp::cubosButtonPressed2);
    prismasButton2.addListener(this, &ofApp::prismasButtonPressed2);
    esferasButton2.addListener(this, &ofApp::esferasButtonPressed2);
    factorSmoothingSlider2.addListener(this, &ofApp::factorSmoothingChanged2);
    factorColorSmoothingSlider2.addListener(this, &ofApp::factorColorSmoothingChanged2);
    desordenInicialSlider2.addListener(this, &ofApp::desordenInicialSliderChanged2);

    parametrosTipoFigura2.setup("Tipo de figura");
    parametrosTipoFigura2.add(cubosButton2.setup("Cubos"));
    parametrosTipoFigura2.add(prismasButton2.setup("Prismas"));
    parametrosTipoFigura2.add(esferasButton2.setup("Esferas"));
    parametrosVideo2.add(&parametrosTipoFigura2);


    //Traslacion video 2
    parametrosVideo2.add(traslacionX.set("Traslacion X", 0, -1000, 1000));
    parametrosVideo2.add(traslacionY.set("Traslacion Y", 0, -1000, 1000));
    parametrosVideo2.add(traslacionZ.set("Traslacion Z", 1, -1000, 1000));  
    
    gui.setup();
    gui.setSize(480, 600);
    gui.add(&parametrosNavegacion);    
    gui.add(&parametrosManejadorVideo);
    gui.add(&parametrosAtributosFigura);
    gui.add(&parametrosVideo2);



    // Gui de vinculaciones 
    vinculacionesGui.setup();
    // Cargar el video
    cargarVideo();
    cargarVideo2();

    //Ajustar camara
    navX = videoPlayer.getWidth() / 2;
    navY = videoPlayer.getHeight() / 2;
    navZ = 1500;



    // Setup de sistema de figuras
    setupSistemaFiguras();
    setupSistemaFiguras2();

	ss.setup(1848, 1016, ofxScreenSetup::WINDOWED);
    // ss.cycleToNextScreenMode();

    ofDisableArbTex();
}

void ofApp::setupChanged(ofxScreenSetup::ScreenSetupArg &arg){

	ofLogNotice()	<< "ofxScreenSetup setup changed from " << ss.stringForMode(arg.oldMode)
	<< " (" << arg.oldWidth << "x" << arg.oldHeight << ") "
	<< " to " << ss.stringForMode(arg.newMode)
	<< " (" << arg.newWidth << "x" << arg.newHeight << ")";
}

void ofApp::cargarVideoButtonPressed() {
    ofFileDialogResult res;
    res = ofSystemLoadDialog( "Cargar preset de configuracion" );
    if ( res.bSuccess ) {
        nombreVideoTxtInput = res.filePath;
        cargarVideo();
    }
}

void ofApp::cargarVideoButtonPressed2() {
    ofFileDialogResult res;
    res = ofSystemLoadDialog( "Cargar preset de configuracion" );
    if ( res.bSuccess ) {
        nombreVideoTxtInput2 = res.filePath;
        cargarVideo2();
    }
}

void ofApp::guardarPresetConfiguracion() {
    ofFileDialogResult res;
    res = ofSystemSaveDialog("preset.xml", "Guardar preset de configuracion");
    if ( res.bSuccess ) {
        gui.saveToFile( res.filePath );

        ofxXmlSettings XML;
        XML.loadFile(res.filePath);
        XML.removeTag("CONFIGURACION"); // Eliminamos el TAG configuracion porque lo vamos a volver a generar
        XML.addTag("CONFIGURACION");
        XML.pushTag("CONFIGURACION");
        int tagNum = XML.addTag("LINK_VIDEO_1");
        XML.setValue("LINK_VIDEO_1", (string)nombreVideoTxtInput, tagNum);
        tagNum = XML.addTag("LINK_VIDEO_2");
        XML.setValue("LINK_VIDEO_2", (string)nombreVideoTxtInput2, tagNum);
        XML.addTag("VINCULACIONES");
        XML.pushTag("VINCULACIONES");
        // Este seria el loop para cada vinculacion vvv
        for (int i=0; i < vinculaciones.size(); ++i) {
            XML.addTag("VINCULACION");
            XML.pushTag("VINCULACION", i);
            XML.setValue("ENTRADA", std::string(entradasValidas[vinculaciones[i].indEntrada]));
            XML.addTag("PARAMETROS");
            XML.pushTag("PARAMETROS");
            for (int j=0; j < vinculaciones[i].parametrosAfectados.size(); ++j) {
                XML.addTag("PARAMETRO");
                XML.pushTag("PARAMETRO", j);
                std::string nombreParametro = std::string(parametrosValidos[vinculaciones[i].parametrosAfectados[j].parametroAfectado]);
                XML.addValue("NOMBRE", nombreParametro);
                XML.addValue("SUAVIDAD", vinculaciones[i].parametrosAfectados[j].amplificacion);
                XML.addValue("CAPA", vinculaciones[i].parametrosAfectados[j].capa);
                XML.popTag();
            }
            XML.popTag();
            XML.popTag();
        }
        XML.popTag();
        XML.addTag("FACTORES_FEEDBACK");
        XML.pushTag("FACTORES_FEEDBACK");
        for (int capa=1; capa<3; capa++) {
            XML.addTag("CAPA_"+std::to_string(capa));
            XML.pushTag("CAPA_"+std::to_string(capa));
            int i = 0;
            for (const auto& [nombre, valor] : feedbackVinculaciones[capa]) {
                XML.addTag("FACTOR");
                XML.pushTag("FACTOR", i);
                // std::string nombreStr = std::string(nombre);
                XML.addValue("NOMBRE", nombre);
                XML.addValue("VALOR", valor); //feedbackVinculaciones[1][nombreStr]);
                XML.popTag();
                i += 1;
            }
            XML.popTag();
        }
        XML.addTag("GENERAL");
        XML.pushTag("GENERAL");
        int i = 0;
        for (const auto& [nombre, valor] : feedbackVinculacionesGeneral) {
            XML.addTag("FACTOR");
            XML.pushTag("FACTOR", i);
            // std::string nombreStr = std::string(nombre);
            XML.addValue("NOMBRE", nombre);
            XML.addValue("VALOR", valor); //feedbackVinculaciones[1][nombreStr]);
            XML.popTag();
            i += 1;
        }
        XML.popTag();
        XML.popTag();
        XML.saveFile();
    }
}

void ofApp::cargarPresetConfiguracion() {
    ofFileDialogResult res;
    res = ofSystemLoadDialog( "Cargar preset de configuracion" );
    if ( res.bSuccess ) {
        gui.loadFromFile( res.filePath );
        // Levantamos el XML de la configuracion
        ofxXmlSettings XML;
        XML.loadFile(res.filePath);
        XML.pushTag("CONFIGURACION");
        // Primero cargamos el video
        nombreVideoTxtInput = XML.getValue("LINK_VIDEO_1", "");
        nombreVideoTxtInput2 = XML.getValue("LINK_VIDEO_2", "");
        // Ahora vamos a cargar las vinculaciones
        vinculaciones.clear();
        XML.pushTag("VINCULACIONES");
        int cantVinculaciones = XML.getNumTags("VINCULACION");
        if (cantVinculaciones > 0) {
            for (int i=0; i<cantVinculaciones; ++i) {
                XML.pushTag("VINCULACION", i);
                // cargamos la vinculacion con sus parametros
                vinculacion nuevaVinculacion;
                nuevaVinculacion.entrada = XML.getValue("ENTRADA", "");
                int n = sizeof(entradasValidas)/sizeof(entradasValidas[0]);
                int ind = distance(entradasValidas, find(entradasValidas, entradasValidas+n, nuevaVinculacion.entrada));
                nuevaVinculacion.indEntrada = ind;
                XML.pushTag("PARAMETROS");
                int cantParametros = XML.getNumTags("PARAMETRO");
                if (cantParametros > 0) {
                    for (int j=0; j<cantParametros; ++j) {
                        if (XML.pushTag("PARAMETRO", j)) {
                            afectacion pAfectado;
                            int n = sizeof(parametrosValidos)/sizeof(parametrosValidos[0]);
                            int ind = distance(parametrosValidos, find(parametrosValidos, parametrosValidos+n, XML.getValue("NOMBRE", "")));
                            pAfectado.parametroAfectado = ind;
                            pAfectado.amplificacion = XML.getValue("SUAVIDAD", 0.0);
                            pAfectado.capa = XML.getValue("CAPA", 0);
                            nuevaVinculacion.parametrosAfectados.push_back(pAfectado);
                            XML.popTag();
                        }
                    }
                }
                XML.popTag();
                XML.popTag();
                vinculaciones.push_back(nuevaVinculacion);
            }
        }
        XML.popTag();

        // Ahora vamos a cargar los factores de feedback
        XML.pushTag("FACTORES_FEEDBACK");
        for (int capa=1; capa<3; capa++) {
            XML.addTag("CAPA_"+std::to_string(capa));
            XML.pushTag("CAPA_"+std::to_string(capa));
            int cantFactores = XML.getNumTags("FACTOR");
            for (int i=0; i<cantFactores; ++i) {
                XML.pushTag("FACTOR", i);
                std::string nombreFactor = XML.getValue("NOMBRE", "");
                feedbackVinculaciones[capa][nombreFactor] = XML.getValue("VALOR", 0.0);
                XML.popTag();
            }
            XML.popTag();
        }
        XML.pushTag("GENERAL");
        int cantFactores = XML.getNumTags("FACTOR");
        for (int i=0; i<cantFactores; ++i) {
            XML.pushTag("FACTOR", i);
            std::string nombreFactor = XML.getValue("NOMBRE", "");
            feedbackVinculacionesGeneral[nombreFactor] = XML.getValue("VALOR", 0.0);
            XML.popTag();
        }
        XML.popTag();
    
        // Imprimo todo para corroborar:
        for (vinculacion v : vinculaciones) {
            std::cout << "Vinculacion - " << v.indEntrada <<" - " << v.entrada << " - " << v.indEntrada << "\n";
            std::cout << "Parametros [" << v.parametrosAfectados.size() << "]\n";
            for (afectacion p : v.parametrosAfectados) {
                std::cout << "  p:" << p.parametroAfectado << " - a:" << p.amplificacion << " - e" << p.valorEntrada << "\n";
            }
        }

        XML.popTag();

        // Cargamos los videos
        cargarVideo();
        cargarVideo2();
    
        // Inicializamos sistema de figuras para el video.
        setupSistemaFiguras();
        setupSistemaFiguras2();
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
            Figura fig = Figura(posicion, sizeFigura, profundidad, (float)factorSmoothingSlider, this->tipoFigura, 0, 0, 0);
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

void ofApp::setupSistemaFiguras2() {
    std::cout << "[i] Setup de sistema de Figuras\n";
    std::vector<std::vector<Figura>> auxiliar2;
    columnas2 = videoPlayer2.getWidth()/sizeFigura2;
    filas2 = videoPlayer2.getHeight()/sizeFigura2;
    for (int i = 0; i < filas2; i++) {
        std::vector<Figura> fila2;
        for (int j = 0; j < columnas2; j++) {
            ofVec3f posicion2 = ofVec3f(j*sizeFigura2+(random()/(RAND_MAX/((int)desordenInicialSlider2+1))),i*sizeFigura2+(random()/(RAND_MAX/((int)desordenInicialSlider2+1))), 0);
            Figura fig2 = Figura(posicion2, sizeFigura2, profundidad2, (float)factorSmoothingSlider2, this->tipoFigura, this->traslacionX, this->traslacionY, this->traslacionZ);
            fila2.push_back(fig2);
        }
        auxiliar2.push_back(fila2);
    }
    sistemaFiguras2 = auxiliar2;
    // Punteros a los elementos del sistema de figuras (se usa para reordenarlo y dibujar segun alpha)
    refsSistemaFiguras2.clear();
    for (int i=0; i<filas2; i++) {
        for (int j=0; j<columnas2; j++) {
            refsSistemaFiguras2.push_back(&sistemaFiguras2[i][j]);
        }
    }
    std::cout << "[i] Termino de configurar sistema de Figuras\n";
}

void ofApp::actualizarTraslacionVideo(int &aux) {
    for (int i = 0; i < filas2; i++) {
        for (int j = 0; j < columnas2; j++) {
            sistemaFiguras2[i][j].updateTraslacionPosicion(this->traslacionX, this->traslacionY, this->traslacionZ);
        }
    }
}

void ofApp::actualizarProfundidad() {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            sistemaFiguras[i][j].updateProfundidadMaxima(profundidad);
        }
    }
}

void ofApp::actualizarProfundidad2() {
    for (int i = 0; i < filas2; i++) {
        for (int j = 0; j < columnas2; j++) {
            sistemaFiguras2[i][j].updateProfundidadMaxima(profundidad2);
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

void ofApp::actualizarTipoFigura2(Figura::enumTipoFigura tipoFigura2) {
    for (int i = 0; i < filas2; i++) {
        for (int j = 0; j < columnas2; j++) {
            sistemaFiguras2[i][j].updateTipoFigura(tipoFigura2);
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

void ofApp::factorSmoothingChanged2(float &factor) {
    for (int i = 0; i < filas2; i++) {
        for (int j = 0; j < columnas2; j++) {
            sistemaFiguras2[i][j].updateFactorSmoothing(factor);
        }
    }
}

void ofApp::sizeFiguraChanged(int &sizeFigura) {
    setupSistemaFiguras();
}

void ofApp::sizeFiguraChanged2(int &sizeFigura) {
    setupSistemaFiguras2();
}

void ofApp::profundidadChanged(int &profundidad) {
    actualizarProfundidad();
}

void ofApp::profundidadChanged2(int &profundidad) {
    actualizarProfundidad2();
}

void ofApp::factorColorSmoothingChanged(float &factor) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            sistemaFiguras[i][j].updateFactorColorSmoothing(factor);
        }
    }
}

void ofApp::factorColorSmoothingChanged2(float &factor) {
    for (int i = 0; i < filas2; i++) {
        for (int j = 0; j < columnas2; j++) {
            sistemaFiguras2[i][j].updateFactorColorSmoothing(factor);
        }
    }
}

void ofApp::desordenInicialSliderChanged(int &desorden) {
    setupSistemaFiguras();
}

void ofApp::desordenInicialSliderChanged2(int &desorden) {
    setupSistemaFiguras2();
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

void ofApp::cubosButtonPressed2() {
    this->tipoFigura2 = Figura::enumTipoFigura::CUBO;
    this->actualizarTipoFigura2(Figura::enumTipoFigura::CUBO);
}

void ofApp::prismasButtonPressed2() {
    this->tipoFigura2 = Figura::enumTipoFigura::PRISMA;
    this->actualizarTipoFigura2(Figura::enumTipoFigura::PRISMA);
}

void ofApp::esferasButtonPressed2() {
    this->tipoFigura2 = Figura::enumTipoFigura::ESFERA;
    this->actualizarTipoFigura2(Figura::enumTipoFigura::ESFERA);
}

void ofApp::cargarVideo() {
    videoPlayer.setPixelFormat(OF_PIXELS_RGBA);
    videoPlayer.load(nombreVideoTxtInput);
    videoPlayer.setVolume(0);
    videoPlayer.setUseTexture(true);
    videoPlayer.play();

    setupSistemaFiguras();
}

void ofApp::cargarVideo2() {
    videoPlayer2.setPixelFormat(OF_PIXELS_RGBA);
    videoPlayer2.load(nombreVideoTxtInput2);
    videoPlayer2.setVolume(0);
    videoPlayer2.setUseTexture(true);
    videoPlayer2.play();

    setupSistemaFiguras2();
}

//--------------------------------------------------------------
void ofApp::update(){
    videoPlayer.update();
    if (videoPlayer.isFrameNew()) {
        framePixels = videoPlayer.getPixels();
        texture.setFromPixels(framePixels);
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
    videoPlayer2.update();
    if (videoPlayer2.isFrameNew()) {
        framePixels2 = videoPlayer2.getPixels();
        // Ordenamos las referencias segun alpha para dibujar primero los mas opacos
        std::sort(refsSistemaFiguras2.begin(), refsSistemaFiguras2.end(), [](const Figura* lhs, const Figura* rhs) {
            return lhs->colorActual.a > rhs->colorActual.a;
        });
        // Actualizamos color y dibujamos
        for (Figura* f : refsSistemaFiguras2) {
            ofVec3f pos2 = f->posActual;
            ofColor color2 = framePixels2.getColor(pos2.x, pos2.y);
            f->actualizarColorObjetivo(color2);
        }
    }
    std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());

    // Esto es para el control de la entrada Camara
    if (webCam.isInitialized()) {
        webCam.update();

        bool bNewFrame = false;
        bNewFrame = webCam.isFrameNew();
        if (bNewFrame){
            ofTextureData texData = webCamFBO.getTexture().getTextureData();
            glBindTexture(webCamTextureSourceID, (GLuint)texData.textureID);
            colorImg.setFromPixels(webCam.getPixels());
            grayImage = colorImg;
            if (bLearnBakground == true){
                grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
                bLearnBakground = false;
            }
            // take the abs value of the difference between background and incoming and then threshold:
            grayDiff.absDiff(grayBg, grayImage);
            grayDiff.threshold(threshold);
            // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
            // also, find holes is set to true so we will get interior contours as well....
            contourFinder.findContours(grayDiff, 20, (webCam.getWidth()*webCam.getHeight())/3, 10, false, true);	// find holes

            float maxPorcentaje = -1;
            for (int i = 0; i < contourFinder.nBlobs; i++){
                    float areaBlob = contourFinder.blobs[i].area;
                    float porcentajeOcupado = ofMap(areaBlob, 20, (webCam.getWidth()*webCam.getHeight())/3, 0, 100);
                    if (porcentajeOcupado > maxPorcentaje)
                            maxPorcentaje = porcentajeOcupado;
            }
            camaraCercania = maxPorcentaje/100;
        }
    }
}

void ofApp::audioIn(ofSoundBuffer & input){
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;	

	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume	
	for (size_t i = 0; i < input.getNumFrames(); i++){
		left[i]		= input[i*2]*0.5;
		right[i]	= input[i*2+1]*0.5;

		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted+=2;
	}
	
	//this is how we get the mean of rms :) 
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :) 
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.99;
	smoothedVol += 0.005 * curVol;
	
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    entradasSonido["Sonido"] = scaledVol;
}

void ofApp::configurar_entrada_audio() {
    std::cout << "[i] Realizando setup de entrada de audio\n";
    int bufferSize = 256;

	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	
	smoothedVol     = 0.0;
	scaledVol		= 0.0;

    ofSoundStreamSettings settings;
    auto devices = soundStream.getMatchingDevices("default");
    if(!devices.empty()){
        settings.setInDevice(devices[0]);
    }
    settings.setInListener(this);
    settings.numOutputChannels = 0;
    settings.numInputChannels = 1;
    soundStream.setup(settings);
    std::cout << "[i] Entrada de audio finalizada\n";
    entradaSonidoIniciada = true;
}
void ofApp::configurar_entrada_camara() {
    webCam.setDeviceID(0);
    webCam.setDesiredFrameRate(10);
    webCam.initGrabber(320, 240);
	colorImg.allocate(webCam.getWidth(),webCam.getHeight());
	grayImage.allocate(webCam.getWidth(),webCam.getHeight());
	grayBg.allocate(webCam.getWidth(),webCam.getHeight());
	grayDiff.allocate(webCam.getWidth(),webCam.getHeight());
	bLearnBakground = true;
	threshold = 80;
    webCamFBO.allocate(webCam.getWidth(), webCam.getHeight());
    ofTextureData texData = webCamFBO.getTexture().getTextureData();
    glBindTexture(webCamTextureSourceID, (GLuint)texData.textureID);
}
//--------------------------------------------------------------
void ofApp::draw(){
    // Reseteo los factores para no acumular valores
    for (int capa=1; capa<3; capa++) {
        factoresVinculaciones[capa]["Profundidad figuras"] = feedbackVinculaciones[capa]["Profundidad figuras"]*factoresVinculaciones[capa]["Profundidad figuras"];
        factoresVinculaciones[capa]["Tamano figuras"] = feedbackVinculaciones[capa]["Tamano figuras"]*factoresVinculaciones[capa]["Tamano figuras"];
        factoresVinculaciones[capa]["Despl. camara X"] = feedbackVinculacionesGeneral["Despl. camara X"]*factoresVinculaciones[capa]["Despl. camara X"];
        factoresVinculaciones[capa]["Despl. camara Y"] = feedbackVinculacionesGeneral["Despl. camara Y"]*factoresVinculaciones[capa]["Despl. camara Y"];
        factoresVinculaciones[capa]["Despl. camara Z"] = feedbackVinculacionesGeneral["Despl. camara Z"]*factoresVinculaciones[capa]["Despl. camara Z"];
    }
    // Primero lo que voy a hacer es procesar las vinculaciones y tomar decisiones o datos en funcion de estos
    for (int i=0; i<vinculaciones.size(); i++) {
        if (std::string(entradasValidas[vinculaciones[i].indEntrada]) == "Camara") {
            if (!webCam.isInitialized())
                this->configurar_entrada_camara();
        }
        
        if (std::string(entradasValidas[vinculaciones[i].indEntrada]) == "Sonido") {
            // Es vinculacion de sonido
            // Buscamos si ya se esta procesando esta entrada, si no, la creamos
            if (!entradaSonidoIniciada) {
                std::cout << "[V] Entrada sonido aun no iniciada, iniciando.\n";
                // No se esta procesando aun, la iniciamos
                entradasSonido["Sonido"] = 0.0;
                this->configurar_entrada_audio();
            }
            for (int j=0; j<vinculaciones[i].parametrosAfectados.size(); j++) {
                vinculaciones[i].parametrosAfectados[j].valorEntrada = entradasSonido["Sonido"];
                // std::cout << vinculaciones[i].parametrosAfectados[j].valorEntrada <<"\n";
            }
        }
        if (std::string(entradasValidas[vinculaciones[i].indEntrada]) == "Camara") {
            for (int j=0; j<vinculaciones[i].parametrosAfectados.size(); j++) {
                vinculaciones[i].parametrosAfectados[j].valorEntrada = camaraCercania;
                // std::cout << vinculaciones[i].parametrosAfectados[j].valorEntrada <<"\n";
            }
        }
        // Ahora vamos a calcular los factores de atributos afectados
        for (int j=0; j<vinculaciones[i].parametrosAfectados.size(); j++) {
            afectacion af = vinculaciones[i].parametrosAfectados[j];
            std::string nombreParametro = std::string(parametrosValidos[vinculaciones[i].parametrosAfectados[j].parametroAfectado]);
            factoresVinculaciones[af.capa][nombreParametro] += af.valorEntrada*af.amplificacion;
        }
    }


    ofBackground(ofColor(0,0,0));
    navX = navX;
    navY = navY;
    navZ = navZ;
    ofVec3f camPosition = cam.getPosition();
    if (!mouseNav) {
        cam.setupPerspective();
        cam.setPosition(ofVec3f(navX,navY,navZ));
    } else {
        navX = camPosition.x;
        navY = camPosition.y;
        navZ = camPosition.z;
    }

    float factoredNavX = (float)navX + factoresVinculaciones[1]["Despl. camara X"]*(-1000) + factoresVinculaciones[2]["Despl. camara X"]*(-1000);
    float factoredNavY = (float)navY + factoresVinculaciones[1]["Despl. camara Y"]*(-1000) + factoresVinculaciones[2]["Despl. camara Y"]*(-1000);
    float factoredNavZ = (float)navZ + factoresVinculaciones[1]["Despl. camara Z"]*(-1000) + factoresVinculaciones[2]["Despl. camara Z"]*(-1000);
    cam.setPosition(ofVec3f(factoredNavX, factoredNavY, factoredNavZ));

    if (lookAtCenter) {
        cam.lookAt(ofVec3f(videoPlayer.getWidth()/2, videoPlayer.getHeight()/2, 0));
    }
    cam.begin();    
	// this uses depth information for occlusion
	// rather than always drawing things on top of each other

    ofEnableDepthTest();
    if (bypass) {
        //Setting video 1 texture to plane
        plane.setPosition(	0, 0, 0);
        plane.resizeToTexture( texture.getTexture() );
        ofPushMatrix();  
            // rotacion del plano para que la textura este al derecho y traslacion para volver a la posicion   
            ofRotateXDeg(180);
            ofTranslate(videoPlayer.getWidth()/2, -videoPlayer.getHeight()/2,0); 

            texture.getTexture().bind();

            ofSetColor(255, 255, 255);
            plane.draw();

            texture.getTexture().unbind();
        ofPopMatrix();  
    }
     else {
        //Dibujamos sistemas de figuras
        for (Figura* f : refsSistemaFiguras) {
            f->draw(this->factoresVinculaciones[1], tamanoPorBrillo, tamanoPorBrilloMinimo, tamanoPorBrilloMaximo);
        }
    }

    // 2nd video
    if (showVideo2) {
        for (Figura* f : refsSistemaFiguras2) {
            f->draw(this->factoresVinculaciones[2], tamanoPorBrillo, tamanoPorBrilloMinimo, tamanoPorBrilloMaximo);
            // f->draw_sin_vinculacion(tamanoPorBrillo, tamanoPorBrilloMinimo, tamanoPorBrilloMaximo);
        }
    } 
    ofDisableDepthTest();
    cam.end();
    if (!hideGUI) {
        gui.draw();

        vinculacionesGui.begin();
            ImGui::SetNextWindowSize(ImVec2(550, 500));
            ImGui::Begin("Vinculaciones de entradas");
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.48f, 1.0f, 0.56f));
            if (ImGui::Button("Agregar vinculacion")) {
                vinculacion nuevaVinculacion;
                nuevaVinculacion.entrada = "Sonido";
                nuevaVinculacion.indEntrada = 0;
                vinculaciones.push_back(nuevaVinculacion);
            }
            ImGui::PopStyleColor(1);
            // Mostramos cada vinculacion
            for (int i=0; i<vinculaciones.size(); i++) {
                string nombre = "vinculacion" + to_string(i);
                if (ImGui::TreeNode(nombre.c_str())) {
                    ImGui::Text(entradasValidas[vinculaciones[i].indEntrada]);
                    ImGui::SameLine();
                    ImGui::Text(" vinculado a ");
                    for (int j=0; j<vinculaciones[i].parametrosAfectados.size(); j++) {
                        ImGui::Text(" - ");
                        ImGui::SameLine();
                        ImGui::Text(parametrosValidos[vinculaciones[i].parametrosAfectados[j].parametroAfectado]);
                    }
                    ImGui::Combo("Entrada usada", &vinculaciones[i].indEntrada, entradasValidas, IM_ARRAYSIZE(entradasValidas));
                    if (std::strcmp(entradasValidas[vinculaciones[i].indEntrada], "Camara") == 0) {
                        if (ImGui::Button("SET FONDO (tecla espacio)")) {
                            bLearnBakground = true;
                        }
                        ImGui::Text(to_string(camaraCercania).c_str());
                    } else if (std::strcmp(entradasValidas[vinculaciones[i].indEntrada], "Sonido") == 0) {
                        ImGui::Text(to_string(entradasSonido["Sonido"]).c_str());
                    }
                    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.48f, 1.0f, 0.56f));
                    if (ImGui::Button("Agregar parametro afectado")) {
                        afectacion pAfectado;
                        pAfectado.amplificacion = 0.0;
                        pAfectado.capa = 1;
                        vinculaciones[i].parametrosAfectados.push_back(pAfectado);
                    }
                    ImGui::PopStyleColor(1);

                    for (int j=0; j<vinculaciones[i].parametrosAfectados.size(); j++) {
                        string nombreParametro = "parametro" + to_string(j);
                        if (ImGui::TreeNode(nombreParametro.c_str())) {
                            const char* items[] = { "1", "2" };
                            int capaElegida = vinculaciones[i].parametrosAfectados[j].capa -1;
                            ImGui::Combo("Capa afectada", &capaElegida, items, 2);
                            vinculaciones[i].parametrosAfectados[j].capa = capaElegida+1;
                            ImGui::Combo("Parametro afectado", &vinculaciones[i].parametrosAfectados[j].parametroAfectado, parametrosValidos, IM_ARRAYSIZE(parametrosValidos));
                            ImGui::SliderFloat("Suavidad", &vinculaciones[i].parametrosAfectados[j].amplificacion, 0.0f, 1.0f, "ratio = %.4f");

                            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
                            if (ImGui::Button("Eliminar parametro")) {
                                vinculaciones[i].parametrosAfectados.erase(vinculaciones[i].parametrosAfectados.begin() + j);
                            }
                            ImGui::PopStyleColor(1);
                            ImGui::TreePop();
                        }
                    }
                    
                    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
                    if (ImGui::Button("Eliminar vinculacion")) {
                        vinculaciones.erase(vinculaciones.begin() + i);
                    }
                    ImGui::PopStyleColor(1);
                    ImGui::TreePop();

                }
            }
            ImGui::Text("CAPA 1 -Feedback de los factores");
            for (auto const & [k, v] : feedbackVinculaciones[1]) {
                ImGui::SliderFloat((k+" c1").c_str(), &feedbackVinculaciones[1][k], 0.0f, 1.0f, "ratio = %.4f");
            }
            ImGui::Text("CAPA 2 -Feedback de los factores");
            for (auto const & [k, v] : feedbackVinculaciones[2]) {
                ImGui::SliderFloat((k+" c2").c_str(), &feedbackVinculaciones[2][k], 0.0f, 1.0f, "ratio = %.4f");
            }
            ImGui::Text("Feedback de factores globales");
            for (auto const & [k, v] : feedbackVinculacionesGeneral) {
                ImGui::SliderFloat((k+" c2").c_str(), &feedbackVinculacionesGeneral[k], 0.0f, 1.0f, "ratio = %.4f");
            }
        ImGui::End();
        vinculacionesGui.end();
    }

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
    if (key == ' ')
        bLearnBakground = true;

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
    if (key == 'g') {
        hideGUI = !hideGUI;
    }
    if (key == 'f')     
	    ofToggleFullscreen();

    if(key == 'k'){
		// ss.cycleToNextScreenMode();
        if (ss.currentMode == ofxScreenSetup::WINDOWED)
            ss.setup(3840, 1080, ofxScreenSetup::FULL_ALL_MONITORS);
        else
            ss.setup(3840, 1080, ofxScreenSetup::WINDOWED);
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