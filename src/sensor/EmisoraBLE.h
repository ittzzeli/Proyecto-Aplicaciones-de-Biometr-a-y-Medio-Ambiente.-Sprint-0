// -*- mode: c++ -*-

// --------------------------------------------------------------
// Nombre: EmisoraBLE.h
// Descripcion: Encapsula la emision BLE en formato iBeacon.
// Copyright (c) 2026 Elia Garcia Pons
// Fecha: 2026-10-05
// Autor: Elia Garcia Pons
// Aportacion: Version minima para el Sprint 0 basada en el recurso
//             HolaMundoIBeacon proporcionado por el profesorado.
// --------------------------------------------------------------

#ifndef EMISORA_BLE_H_INCLUIDO
#define EMISORA_BLE_H_INCLUIDO

#include <bluefruit.h>

class EmisoraBLE {

private:
  const char * nombreEmisora;
  const uint16_t fabricanteID;
  const int8_t txPower;

public:

  // ------------------------------------------------------------
  // nombre: Text, fabricante: N, potencia: Z --> EmisoraBLE()
  // ------------------------------------------------------------
  // Guarda la configuracion de la emisora BLE.
  // ------------------------------------------------------------
  EmisoraBLE(const char * nombre,
             uint16_t fabricante,
             int8_t potencia)
    : nombreEmisora(nombre),
      fabricanteID(fabricante),
      txPower(potencia) {
  }

  // ------------------------------------------------------------
  // encenderEmisora() -->
  // ------------------------------------------------------------
  // Inicializa Bluefruit y configura nombre y potencia.
  // ------------------------------------------------------------
  void encenderEmisora() {
    Bluefruit.begin();
    Bluefruit.setTxPower(txPower);
    Bluefruit.setName(nombreEmisora);
    detenerAnuncio();
  }

  // ------------------------------------------------------------
  // estaAnunciando() --> anunciando: B
  // ------------------------------------------------------------
  // Indica si existe un anuncio BLE activo.
  // ------------------------------------------------------------
  bool estaAnunciando() const {
    return Bluefruit.Advertising.isRunning();
  }

  // ------------------------------------------------------------
  // detenerAnuncio() -->
  // ------------------------------------------------------------
  // Detiene el anuncio BLE si esta activo.
  // ------------------------------------------------------------
  void detenerAnuncio() {
    if (estaAnunciando()) {
      Bluefruit.Advertising.stop();
    }
  }

  // ------------------------------------------------------------
  // uuid: [N]_16, major: N, minor: N, rssi: Z
  // --> emitirAnuncioIBeacon() -->
  // ------------------------------------------------------------
  // Construye un iBeacon y comienza a anunciarlo indefinidamente.
  // Se mantendra activo hasta que una nueva medida lo sustituya.
  // ------------------------------------------------------------
  void emitirAnuncioIBeacon(uint8_t * uuid,
                            uint16_t major,
                            uint16_t minor,
                            int8_t rssi) {
    detenerAnuncio();

    Bluefruit.Advertising.clearData();
    Bluefruit.ScanResponse.clearData();

    BLEBeacon beacon(uuid, major, minor, rssi);
    beacon.setManufacturer(fabricanteID);

    Bluefruit.setTxPower(txPower);
    Bluefruit.setName(nombreEmisora);
    Bluefruit.ScanResponse.addName();

    Bluefruit.Advertising.setBeacon(beacon);
    Bluefruit.Advertising.restartOnDisconnect(true);

    // 160 unidades * 0.625 ms = 100 ms entre anuncios.
    Bluefruit.Advertising.setInterval(160, 160);

    // 0 = anunciar indefinidamente hasta detenerlo manualmente.
    Bluefruit.Advertising.start(0);
  }
};

#endif
