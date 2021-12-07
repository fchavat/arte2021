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
    


    // Gui de vinculaciones 
    vinculacionesGui.setup();
    // Cargar el video
    cargarVideo();

    // Setup de sistema de figuras
    setupSistemaFiguras();

	ss.setup(1848, 1016, ofxScreenSetup::WINDOWED);
    // ss.cycleToNextScreenMode();
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
        int tagNum = XML.addTag("LINK_VIDEO");
        XML.setValue("LINK_VIDEO", (string)nombreVideoTxtInput, tagNum);
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
                XML.popTag();
            }
            XML.popTag();
            XML.popTag();
        }
        XML.popTag();
        XML.addTag("FACTORES_FEEDBACK");
        XML.pushTag("FACTORES_FEEDBACK");
        int i = 0;
        for (const char* nombre : parametrosValidos) {
            XML.addTag("FACTOR");
            XML.pushTag("FACTOR", i);
            std::string nombreStr = std::string(nombre);
            XML.addValue("NOMBRE", nombreStr);
            XML.addValue("VALOR", feedbackVinculaciones[nombreStr]);
            XML.popTag();
            i += 1;
        }
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
        nombreVideoTxtInput = XML.getValue("LINK_VIDEO", "");
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
        int cantFactores = XML.getNumTags("FACTOR");
        for (int i=0; i<cantFactores; ++i) {
            XML.pushTag("FACTOR", i);
            std::string nombreFactor = XML.getValue("NOMBRE", "");
            feedbackVinculaciones[nombreFactor] = XML.getValue("VALOR", 0.0);
            XML.popTag();
        }
        // Imprimo todo para corroborar:
        for (vinculacion v : vinculaciones) {
            std::cout << "Vinculacion - " << v.indEntrada <<" - " << v.entrada << " - " << v.indEntrada << "\n";
            std::cout << "Parametros [" << v.parametrosAfectados.size() << "]\n";
            for (afectacion p : v.parametrosAfectados) {
                std::cout << "  p:" << p.parametroAfectado << " - a:" << p.amplificacion << " - e" << p.valorEntrada << "\n";
            }
        }

        XML.popTag();
        // Cargamos el video
        videoPlayer.setPixelFormat(OF_PIXELS_RGBA);
        videoPlayer.load(nombreVideoTxtInput);
        videoPlayer.setVolume(0);
        videoPlayer.setUseTexture(true);
        videoPlayer.play();
    
        // Inicializamos sistema de figuras para el video.
        setupSistemaFiguras();
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
            contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, false, true);	// find holes

            float maxPorcentaje = -1;
            for (int i = 0; i < contourFinder.nBlobs; i++){
                    float areaBlob = contourFinder.blobs[i].area;
                    float porcentajeOcupado = ofMap(areaBlob, 20, (340*240)/3, 0, 100);
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
	colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	grayBg.allocate(320,240);
	grayDiff.allocate(320,240);
	bLearnBakground = true;
	threshold = 80;
    webCamFBO.allocate(320, 240);
    ofTextureData texData = webCamFBO.getTexture().getTextureData();
    glBindTexture(webCamTextureSourceID, (GLuint)texData.textureID);
}
//--------------------------------------------------------------
void ofApp::draw(){
    // Reseteo los factores para no acumular valores
    factoresVinculaciones["Profundidad figuras"] = feedbackVinculaciones["Profundidad figuras"]*factoresVinculaciones["Profundidad figuras"];
	factoresVinculaciones["Tamano figuras"] = feedbackVinculaciones["Tamano figuras"]*factoresVinculaciones["Tamano figuras"];
	factoresVinculaciones["Despl. camara X"] = feedbackVinculaciones["Despl. camara X"]*factoresVinculaciones["Despl. camara X"];
	factoresVinculaciones["Despl. camara Y"] = feedbackVinculaciones["Despl. camara Y"]*factoresVinculaciones["Despl. camara Y"];
	factoresVinculaciones["Despl. camara Z"] = feedbackVinculaciones["Despl. camara Z"]*factoresVinculaciones["Despl. camara Z"];
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
            factoresVinculaciones[nombreParametro] += af.valorEntrada*af.amplificacion;
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

    float factoredNavX = (float)navX + factoresVinculaciones["Despl. camara X"]*(-1000);
    float factoredNavY = (float)navY + factoresVinculaciones["Despl. camara Y"]*(-1000);
    float factoredNavZ = (float)navZ + factoresVinculaciones["Despl. camara Z"]*(-1000);
    cam.setPosition(ofVec3f(factoredNavX, factoredNavY, factoredNavZ));

    if (lookAtCenter) {
        cam.lookAt(ofVec3f(videoPlayer.getWidth()/2, videoPlayer.getHeight()/2, 0));
    }
    cam.begin();    
	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
    ofEnableDepthTest();
    // Actualizamos color y dibujamos
    for (Figura* f : refsSistemaFiguras) {
        f->draw(this->factoresVinculaciones, tamanoPorBrillo, tamanoPorBrilloMinimo, tamanoPorBrilloMaximo);
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
                        vinculaciones[i].parametrosAfectados.push_back(pAfectado);
                    }
                    ImGui::PopStyleColor(1);

                    for (int j=0; j<vinculaciones[i].parametrosAfectados.size(); j++) {
                        string nombreParametro = "parametro" + to_string(j);
                        if (ImGui::TreeNode(nombreParametro.c_str())) {
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
            ImGui::Text("Feedback de los factores");
            for (auto const & [k, v] : feedbackVinculaciones) {
                ImGui::SliderFloat(k.c_str(), &feedbackVinculaciones[k], 0.0f, 1.0f, "ratio = %.4f");
            }
        ImGui::Begin("Vinculaciones de entradas");
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