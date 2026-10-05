// --------------------------------------------------------------
// Nombre: test_codificador_major.cpp
// Descripcion: Tests automaticos independientes del hardware para
//              comprobar la codificacion del Major del iBeacon.
// Copyright (c) 2026 Elia Garcia Pons
// Fecha: 2026-10-05
// Autor: Elia Garcia Pons
// Aportacion: Test reproducible de la logica tipo+contador.
// --------------------------------------------------------------

#include <cassert>
#include <iostream>
#include "../../src/sensor/SensorSprint0/CodificadorMajor.h"

int main() {
  // O3 = 11 = 0x0B, contador = 5 = 0x05
  uint16_t majorO3 = CodificadorMajor::construirMajor(11, 5);

  assert(majorO3 == 0x0B05);
  assert(majorO3 == 2821);
  assert(CodificadorMajor::obtenerTipo(majorO3) == 11);
  assert(CodificadorMajor::obtenerContador(majorO3) == 5);

  // TEMPERATURA = 12 = 0x0C, contador = 255 = 0xFF
  uint16_t majorTemperatura = CodificadorMajor::construirMajor(12, 255);

  assert(majorTemperatura == 0x0CFF);
  assert(CodificadorMajor::obtenerTipo(majorTemperatura) == 12);
  assert(CodificadorMajor::obtenerContador(majorTemperatura) == 255);

  // Comprobar el reinicio natural de un contador de 8 bits.
  uint8_t contador = 255;
  contador++;
  assert(contador == 0);

  std::cout << "OK - Codificacion y decodificacion de Major correctas" << std::endl;
  std::cout << "OK - O3(11) + contador(5) = 0x0B05 = 2821" << std::endl;
  std::cout << "OK - Reinicio de contador 255 -> 0 correcto" << std::endl;

  return 0;
}
