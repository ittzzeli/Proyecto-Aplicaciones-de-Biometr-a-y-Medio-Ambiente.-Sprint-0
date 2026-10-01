// -*- mode: c++ -*-

// ----------------------------------------------------------
// Jordi Bataller i Mascarell
// 2019-07-07
// ----------------------------------------------------------

#ifndef PUERTO_SERIE_H_INCLUIDO
#define PUERTO_SERIE_H_INCLUIDO


// ----------------------------------------------------------
// PuertoSerie
//
// Responsabilidad:
// Gestionar la comunicación mediante el puerto serie,
// permitiendo inicializarlo, esperar hasta que esté
// disponible y escribir información por él.
// ----------------------------------------------------------
class PuertoSerie {

public:

  // ------------------------------------------------------------
  // baudios: Z --> PuertoSerie()
  // ------------------------------------------------------------
  //
  // Inicializa el puerto serie utilizando la velocidad
  // indicada en baudios.
  // ------------------------------------------------------------
  PuertoSerie(long baudios) {

    Serial.begin(baudios);

    // No esperamos aquí a que Serial esté disponible porque
    // el constructor puede ejecutarse antes de que el sistema
    // esté completamente inicializado.
  }


  // ------------------------------------------------------------
  // esperarDisponible() -->
  // ------------------------------------------------------------
  //
  // Bloquea la ejecución hasta que el puerto serie
  // se encuentre disponible.
  // ------------------------------------------------------------
  void esperarDisponible() {

    while (!Serial) {

      delay(10);
    }
  }


  // ------------------------------------------------------------
  // mensaje: T --> escribir() -->
  // ------------------------------------------------------------
  //
  // Escribe un dato por el puerto serie.
  //
  // T representa un tipo genérico admitido por Serial.print(),
  // por ejemplo Text, N, Z o R.
  // ------------------------------------------------------------
  template<typename T>
  void escribir(T mensaje) {

    Serial.print(mensaje);
  }

}; // class PuertoSerie


#endif