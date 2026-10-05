// -*- mode: c++ -*-

// --------------------------------------------------------------
// Nombre: Publicador.h
// Descripcion: Convierte una medicion en los campos Major/Minor
//              de un iBeacon y solicita su emision BLE.
// Copyright (c) 2026 Elia Garcia Pons
// Fecha: 2026-10-05
// Autor: Elia Garcia Pons
// Aportacion: Codificacion del tipo de medida y contador en Major.
// --------------------------------------------------------------

#ifndef PUBLICADOR_H_INCLUIDO
#define PUBLICADOR_H_INCLUIDO

#include <stdint.h>
#include "CodificadorMajor.h"
#include "EmisoraBLE.h"

class Publicador {

private:
  uint8_t beaconUUID[16] = {
    'E', 'P', 'S', 'G', '-', 'G', 'T', 'I',
    '-', 'P', 'R', 'O', 'Y', '-', '3', 'A'
  };

  EmisoraBLE laEmisora {
    "Elia_GTI",
    0x004C,
    4
  };

  const int8_t RSSI = -53;

public:

  enum MedicionesID : uint8_t {
    O3 = 11,
    TEMPERATURA = 12
  };

  // ------------------------------------------------------------
  // Publicador()
  // ------------------------------------------------------------
  // Construye el publicador. La emisora se enciende en setup().
  // ------------------------------------------------------------
  Publicador() {
  }

  // ------------------------------------------------------------
  // encenderEmisora() -->
  // ------------------------------------------------------------
  // Inicializa la emisora BLE.
  // ------------------------------------------------------------
  void encenderEmisora() {
    laEmisora.encenderEmisora();
  }

  // ------------------------------------------------------------
  // tipo: MedicionesID, valor: N, contador: N
  // --> publicarMedida() --> major: N
  // ------------------------------------------------------------
  // Codifica tipo y contador en Major, coloca la medida en Minor
  // y deja el iBeacon anunciandose hasta la siguiente medida.
  // ------------------------------------------------------------
  uint16_t publicarMedida(MedicionesID tipo,
                          uint16_t valor,
                          uint8_t contador) {
    uint16_t major = CodificadorMajor::construirMajor(
      (uint8_t) tipo,
      contador
    );

    laEmisora.emitirAnuncioIBeacon(
      beaconUUID,
      major,
      valor,
      RSSI
    );

    return major;
  }
};

#endif
