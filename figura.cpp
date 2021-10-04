#include "figura.h"
#include <iostream>

Figura::Figura(ofVec3f pos, int tamano) {
  this->posActual = pos;  
  this->tamano = tamano;
  this->rangoProfundidad = this->profActual = this->profObjetivo = 0;
}

Figura::Figura(ofVec3f pos, int tamano, int rangoProfundidad) {
  this->posActual = pos;  
  this->tamano = tamano;
  this->rangoProfundidad = rangoProfundidad;
  this->profActual = this->profObjetivo = 0;
}

Figura::Figura(ofVec3f pos, int tamano, int rangoProfundidad, float factorSmoothing) {
  this->posActual = pos;  
  this->tamano = tamano;
  this->rangoProfundidad = rangoProfundidad;
  this->profActual = this->profObjetivo = 0;
  this->factorSmoothing = factorSmoothing;
}

Figura::Figura(ofVec3f pos, int tamano, int rangoProfundidad, float factorSmoothing, enumTipoFigura tipoFigura) {
  this->posActual = pos;  
  this->tamano = tamano;
  this->rangoProfundidad = rangoProfundidad;
  this->profActual = this->profObjetivo = 0;
  this->factorSmoothing = factorSmoothing;
  this->tipoFigura = tipoFigura;
  this->colorObjetivo = ofColor(0,0,0,0);
}

void Figura::draw(float brightness) {
  ofPushMatrix();
    ofSetColor(brightness);
    float profBloque = ofMap(brightness, 0, 255, -this->rangoProfundidad, this->rangoProfundidad);
    float factorTamano = ofMap(profBloque, -this->rangoProfundidad, this->rangoProfundidad, 1, 3);
    // std::cout << posActual.x << " " << posActual.y << " " << profBloque << "\n";
    ofTranslate(this->posActual.x, this->posActual.y, profBloque);
    ofDrawBox(this->tamano);
  ofPopMatrix();
}

void Figura::draw(float R, float G, float B, bool tamanoPorBrillo, float tamanoPorBrilloMinimo, float tamanoPorBrilloMaximo) {
  this->actualizarProfActual();
  this->actualizarColorActual();
  float brightness = 0.2126 * R + 0.7152 * G + 0.0722 * B;
  // ofSetColor(R,G,B);
  this->colorObjetivo = ofColor(R,G,B);
  ofSetColor(this->colorActual);

  float nuevaProfObjetivo = ofMap(brightness, 0, 255, -this->rangoProfundidad, this->rangoProfundidad);
  this->profObjetivo = nuevaProfObjetivo;
  
  float tamanoActual = ofMap(brightness, 0, 255, 1, tamano);
  float profBloque = this->profActual;
  float factorTamanoBrillo = ofMap(brightness, 0, 255, tamanoPorBrilloMinimo, tamanoPorBrilloMaximo);
  float factorTamano = ofMap(profBloque, -this->rangoProfundidad, this->rangoProfundidad, 1, 3);
  ofVec3f pos = ofVec3f(this->posActual.x, this->posActual.y, profBloque);
  if (tamanoPorBrillo) {
    this->dibujarFigura(pos, this->tamano*factorTamanoBrillo);
  } else {
    this->dibujarFigura(pos, this->tamano);
  }
}

void Figura::dibujarFigura(ofVec3f pos, float size) {
  ofPushMatrix();
    if (this->tipoFigura == CUBO) {
      ofTranslate(pos);
      ofDrawBox(size);
    }
    else if (this->tipoFigura == PRISMA) {
      ofTranslate(pos.x, pos.y, -pos.z);
      ofDrawBox(size, size, profActual*2);
    }
    else if (this->tipoFigura == ESFERA) {
      ofTranslate(pos);
      ofDrawSphere(size);
    }
  ofPopMatrix();
}

void Figura::actualizarProfActual() {
  if (modoMovimiento == DIRECTO) {
    this->profActual = this->profObjetivo;
  } else if (modoMovimiento == SMOOTH)
  {
    this->profActual = this->profActual + (this->profObjetivo - this->profActual)*factorSmoothing;
    this->colorActual = this->colorActual.lerp(this->colorObjetivo, this->factorSmoothing);
  }
  
}

void Figura::actualizarColorActual() {
  this->colorActual = this->colorActual.lerp(this->colorObjetivo, this->factorColorSmoothing);
}

void Figura::updateFactorColorSmoothing(float factor) {
  this->factorColorSmoothing = factor;
}

void Figura::updateFactorSmoothing(float factor) {
  this->factorSmoothing = factor;
}

void Figura::updateTipoFigura(enumTipoFigura tipo) {
  this->tipoFigura = tipo;
}

void Figura::updateProfundidadMaxima(int prof) {
  this->rangoProfundidad = prof;
}
