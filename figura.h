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
    Figura(ofVec3f pos, int tamano, int profundidad, float smooth, enumTipoFigura tipoFigura);
		
		void updateProfundidadMaxima(int prof);
    void updateTamano(int tamano);
    void updateFactorSmoothing(float factor);
    void updateTipoFigura(enumTipoFigura tipo);
		void draw(float brightness);		
    void draw(float R, float G, float B);
		
		ofVec3f posActual;
    ofVec3f posObjetivo;
		ofVec3f velocidad;

    int rangoProfundidad;
    int tamano;

    // Para el movimiento de las figuras
    int profActual;
    int profObjetivo;

    //  smoothing
    enumModoMovimiento modoMovimiento = SMOOTH;
    float factorSmoothing = 0.5;

    // Tipo de figura
    enumTipoFigura tipoFigura = CUBO;
    void dibujarFigura(ofVec3f pos, float size);


  private:
    void actualizarProfObjetivo(float nuevaProfundidad);
    void actualizarProfActual();
};
