// -*- mode: c++ -*-

// --------------------------------------------------------------
// Nombre: Medidor.h
// Descripcion: Genera la medicion ficticia de O3 del Sprint 0.
// Copyright (c) 2026 Elia Garcia Pons
// Fecha: 2026-10-05
// Autor: Elia Garcia Pons
// Aportacion: Sustituye temporalmente el sensor real por un valor
//             fijo reproducible para la demostracion del Sprint 0.
// --------------------------------------------------------------

#ifndef MEDIDOR_H_INCLUIDO
#define MEDIDOR_H_INCLUIDO

#include <stdint.h>

class Medidor {

private:
  static const uint16_t VALOR_O3_SIMULADO = 1234;

public:

  // ------------------------------------------------------------
  // Medidor()
  // ------------------------------------------------------------
  // Construye el medidor simulado.
  // ------------------------------------------------------------
  Medidor() {
  }

  // ------------------------------------------------------------
  // iniciarMedidor() -->
  // ------------------------------------------------------------
  // Punto de inicializacion reservado para un sensor real futuro.
  // En el Sprint 0 no necesita realizar ninguna accion.
  // ------------------------------------------------------------
  void iniciarMedidor() {
  }

  // ------------------------------------------------------------
  // medirO3() --> valor: N
  // ------------------------------------------------------------
  // Devuelve la medicion ficticia de O3 utilizada en la demo.
  // ------------------------------------------------------------
  uint16_t medirO3() const {
    return VALOR_O3_SIMULADO;
  }
};

#endif
