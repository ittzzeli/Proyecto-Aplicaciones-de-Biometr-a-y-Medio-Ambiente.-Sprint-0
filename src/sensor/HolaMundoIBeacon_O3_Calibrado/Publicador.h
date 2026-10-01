// -*- mode: c++ -*-

// --------------------------------------------------------------
// Jordi Bataller i Mascarell
// Adaptación: publicación de concentración de O3 por iBeacon
// --------------------------------------------------------------

#ifndef PUBLICADOR_H_INCLUIDO
#define PUBLICADOR_H_INCLUIDO

class Publicador {

private:

  uint8_t beaconUUID[16] = {
    'E', 'P', 'S', 'G', '-', 'G', 'T', 'I',
    '-', 'P', 'R', 'O', 'Y', '-', '3', 'A'
  };

public:

  EmisoraBLE laEmisora {
    "gatotico",
    0x004c,
    4
  };

  const int8_t RSSI = -53;

  // ID que podremos utilizar cuando dejemos de usar el Major
  // como contador de prueba.
  enum MedicionesID {
    OZONO = 14
  };

  // ------------------------------------------------------------
  // AHORA: true  -> Major = contador de loop (prueba solicitada)
  // DESPUÉS: false -> Major = 14, identificador fijo de O3
  // ------------------------------------------------------------
  static const bool MAJOR_COMO_CONTADOR = true;


  // ------------------------------------------------------------
  // --> Publicador() -->
  // ------------------------------------------------------------
  Publicador() {
  }


  // ------------------------------------------------------------
  // --> encenderEmisora() -->
  // ------------------------------------------------------------
  void encenderEmisora() {
    (*this).laEmisora.encenderEmisora();
  }


  // ------------------------------------------------------------
  // iBeacon enviado:
  //
  //   Major = contador (modo prueba actual)
  //           o OZONO = 14 cuando MAJOR_COMO_CONTADOR = false
  //
  //   Minor = 0 (fijado temporalmente para evitar variaciones)
  // ------------------------------------------------------------

  // ------------------------------------------------------------
  // valorPPMx1000: N, contador: N, tiempoEspera: Z
  // --> publicarOzono() -->
  // ------------------------------------------------------------
  void publicarOzono(uint16_t valorPPMx1000,
                     uint16_t contador,
                     long tiempoEspera) {

    const uint16_t major = MAJOR_COMO_CONTADOR
      ? contador
      : (uint16_t) MedicionesID::OZONO;

    // La medida de O3 se sigue calculando, pero NO se envía en Minor.
    // Por petición para la prueba actual, Minor permanece siempre a 0.
    (void) valorPPMx1000;

    const uint16_t minor = 0;

    (*this).laEmisora.emitirAnuncioIBeacon(
      (*this).beaconUUID,
      major,
      minor,
      (*this).RSSI
    );

    esperar(tiempoEspera);

    (*this).laEmisora.detenerAnuncio();
  }

}; // class

#endif