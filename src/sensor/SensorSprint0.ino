// -*- mode: c++ -*-

// --------------------------------------------------------------
// Nombre: SensorSprint0.ino
// Descripcion: Programa principal del nodo sensor del Sprint 0.
//              Genera una medida ficticia de O3 y la publica
//              mediante un iBeacon BLE.
// Copyright (c) 2026 Elia Garcia Pons
// Fecha: 2026-10-05
// Autor: Elia Garcia Pons
// Aportacion: Integracion de medicion simulada, contador y BLE.
// --------------------------------------------------------------

#include "Medidor.h"
#include "Publicador.h"

// Tiempo entre dos mediciones nuevas.
// Durante estos 5 segundos el iBeacon sigue anunciando rapidamente
// la misma medida y el mismo contador.
const unsigned long INTERVALO_MEDICION_MS = 5000;

namespace Globales {
  Medidor elMedidor;
  Publicador elPublicador;
}

namespace Estado {
  uint8_t contador = 0;
}

// ------------------------------------------------------------
// setup() -->
// ------------------------------------------------------------
// Inicializa el puerto serie, el medidor y la emisora BLE.
// Se ejecuta una unica vez al arrancar la placa.
// ------------------------------------------------------------
void setup() {
  using namespace Globales;

  Serial.begin(115200);

  // No esperamos indefinidamente a Serial: la placa debe funcionar
  // aunque no haya un monitor serie conectado.
  delay(1000);

  elMedidor.iniciarMedidor();
  elPublicador.encenderEmisora();

  Serial.println("===== INICIANDO SENSOR SPRINT 0 =====");
  Serial.println("Beacon: Elia_GTI");
  Serial.println("UUID: EPSG-GTI-PROY-3A");
  Serial.println("O3 = tipo 11");
  Serial.println("TEMPERATURA = tipo 12 (reservado)");
  Serial.println("Valor O3 simulado = 1234");
}

// ------------------------------------------------------------
// loop() -->
// ------------------------------------------------------------
// Genera una nueva medida, incrementa el contador y publica
// el iBeacon. El Major contiene tipo+contador y Minor la medida.
// ------------------------------------------------------------
void loop() {
  using namespace Globales;
  using namespace Estado;

  contador++;

  uint16_t valorO3 = elMedidor.medirO3();

  uint16_t major = elPublicador.publicarMedida(
    Publicador::O3,
    valorO3,
    contador
  );

  Serial.println();
  Serial.print("Nueva medicion - Loop ");
  Serial.println(contador);

  Serial.print("Tipo O3 = ");
  Serial.println((uint8_t) Publicador::O3);

  Serial.print("Valor simulado O3 = ");
  Serial.println(valorO3);

  Serial.print("Major decimal = ");
  Serial.println(major);

  Serial.print("Major hexadecimal = 0x");
  Serial.println(major, HEX);

  Serial.print("Byte tipo = ");
  Serial.println(CodificadorMajor::obtenerTipo(major));

  Serial.print("Byte contador = ");
  Serial.println(CodificadorMajor::obtenerContador(major));

  Serial.print("Minor = ");
  Serial.println(valorO3);

  delay(INTERVALO_MEDICION_MS);
}
