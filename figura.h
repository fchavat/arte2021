#pragma once
#include "ofMain.h"

class Figura{

	public:
    // enums
     enum enumModoMovimiento{
      DIRECTO,
      SMOOTH
    };
    enum enumTipoFigura{
      CUBO,
      PRISMA,
      ESFERA
    };
		Figura(ofVec3f pos, int tamano);
		Figura(ofVec3f pos, int tamano, int profundidad);
    Figura(ofVec3f pos, int tamano, int profundidad, float smooth);
    Figura(ofVec3f pos, int tamano, int profundidad, float smooth, enumTipoFigura tipoFigura, int traslacionX, int traslacionY, int traslacionZ);
		
		void updateProfundidadMaxima(int prof);
    void updateTamano(int tamano);
    void updateFactorSmoothing(float factor);
    void updateFactorColorSmoothing(float factor);
    void updateTipoFigura(enumTipoFigura tipo);
    void updateTraslacionPosicion(int traslacionPosicionX, int traslacionPosicionY, int traslacionPosicionZ);
		void draw(float brightness);		
    void draw(std::map<std::string, float> factoresVinculaciones, bool tamanoPorBrillo = false, float tamanoPorBrilloMinimo=0.5, float tamanoPorBrilloMaximo=2);
    void draw(std::map<std::string, float> factoresVinculaciones, float R, float G, float B, float A, bool tamanoPorBrillo = false, float tamanoPorBrilloMinimo=0.5, float tamanoPorBrilloMaximo=2);
    void draw_sin_vinculacion(bool tamanoPorBrillo = false, float tamanoPorBrilloMinimo=0.5, float tamanoPorBrilloMaximo=2);
		
		ofVec3f posActual;
    ofVec3f posObjetivo;
		ofVec3f velocidad;

    int rangoProfundidad;
    int tamano;

    // Para el movimiento de las figuras
    int profActual;
    int profObjetivo;
    ofColor colorActual;
    ofColor colorObjetivo;
    void actualizarColorObjetivo(ofColor c);

    //  smoothing
    enumModoMovimiento modoMovimiento = SMOOTH;
    float factorSmoothing = 0.5;
    float factorColorSmoothing = 1;

    // Tipo de figura
    enumTipoFigura tipoFigura = CUBO;
    void dibujarFigura(ofVec3f pos, float size);

    int traslacionPosicionX = 0;
    int traslacionPosicionY = 0;
    int traslacionPosicionZ = 0;


  private:
    void actualizarProfObjetivo(float nuevaProfundidad);
    void actualizarProfActual();
    void actualizarColorActual();
};
