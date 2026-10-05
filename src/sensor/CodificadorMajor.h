// -*- mode: c++ -*-

// --------------------------------------------------------------
// Nombre: CodificadorMajor.h
// Descripcion: Codifica y decodifica el campo Major del iBeacon.
// Copyright (c) 2026 Elia Garcia Pons
// Fecha: 2026-10-05
// Autor: Elia Garcia Pons
// Aportacion: Adaptacion para el Sprint 0 del proyecto PBIO.
// --------------------------------------------------------------

#ifndef CODIFICADOR_MAJOR_H_INCLUIDO
#define CODIFICADOR_MAJOR_H_INCLUIDO

#include <stdint.h>

// --------------------------------------------------------------
// CodificadorMajor
//
// Responsabilidad:
// Construir y separar los dos bytes del Major del iBeacon.
// Byte alto: tipo de medida.
// Byte bajo: contador de medida.
// --------------------------------------------------------------
class CodificadorMajor {

public:

  // ------------------------------------------------------------
  // tipo: N, contador: N --> construirMajor() --> major: N
  // ------------------------------------------------------------
  // Coloca el tipo en el byte alto y el contador en el byte bajo.
  // ------------------------------------------------------------
  static uint16_t construirMajor(uint8_t tipo, uint8_t contador) {
    return ((uint16_t) tipo << 8) | contador;
  }

  // ------------------------------------------------------------
  // major: N --> obtenerTipo() --> tipo: N
  // ------------------------------------------------------------
  // Recupera el byte alto del Major.
  // ------------------------------------------------------------
  static uint8_t obtenerTipo(uint16_t major) {
    return (uint8_t) ((major >> 8) & 0xFF);
  }

  // ------------------------------------------------------------
  // major: N --> obtenerContador() --> contador: N
  // ------------------------------------------------------------
  // Recupera el byte bajo del Major.
  // ------------------------------------------------------------
  static uint8_t obtenerContador(uint16_t major) {
    return (uint8_t) (major & 0xFF);
  }
};

#endif
