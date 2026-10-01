// -*-c++-*-

// --------------------------------------------------------------
// Basado en HolaMundoIBeacon de Jordi Bataller i Mascarell
// Adaptado para SPEC ULPSM-O3 968-046.
//
// FUNCIONAMIENTO DE ESTA VERSIÓN:
//
// - ADC de 12 bits con escala del montaje 0..3.3 V.
// - Calibración automática del cero con 50 muestras en aire limpio.
// - Offset = promedio(Vgas) - promedio(Vref).
// - Sensibilidad M calculada desde Sensitivity Code y TIA Gain.
// - ppm = ((Vgas - Vref) - offset) / M.
// - Major = contador de prueba.
// - Minor = 0, fijado temporalmente para la prueba.
// --------------------------------------------------------------


#include <bluefruit.h>


// Evita conflictos entre macros min/max y otras definiciones.
#undef min
#undef max


#include "LED.h"
#include "PuertoSerie.h"


// --------------------------------------------------------------
// Recursos globales básicos de la placa.
// --------------------------------------------------------------
namespace Globales {

  LED elLED(7);

  PuertoSerie elPuerto(115200);
};


#include "EmisoraBLE.h"
#include "Publicador.h"
#include "Medidor.h"


// --------------------------------------------------------------
// Componentes principales del sistema.
// --------------------------------------------------------------
namespace Globales {

  Publicador elPublicador;

  Medidor elMedidor;
};


// ------------------------------------------------------------
// inicializarPlaquita() -->
// ------------------------------------------------------------
//
// Punto reservado para inicializaciones generales de la placa.
//
// Actualmente no necesita realizar ninguna acción.
// ------------------------------------------------------------
void inicializarPlaquita() {
}


// ------------------------------------------------------------
// setup() -->
// ------------------------------------------------------------
//
// Inicializa el sistema:
//
// 1. Inicializa la placa.
// 2. Configura el medidor.
// 3. Realiza la calibración en aire limpio.
// 4. Muestra los datos de calibración por puerto serie.
// 5. Enciende la emisora BLE.
//
// Arduino ejecuta esta función una única vez al arrancar.
// ------------------------------------------------------------
void setup() {

  using namespace Globales;


  inicializarPlaquita();


  // ----------------------------------------------------------
  // Inicialización del sistema de medición.
  // ----------------------------------------------------------
  elMedidor.iniciarMedidor();


  // ----------------------------------------------------------
  // Información inicial por puerto serie.
  // ----------------------------------------------------------
  elPuerto.escribir(
    "\n========================================\n"
  );

  elPuerto.escribir(
    " SENSOR O3 ULPSM-O3 968-046\n"
  );

  elPuerto.escribir(
    " Vgas  -> A3\n"
  );

  elPuerto.escribir(
    " Vref  -> A4\n"
  );

  elPuerto.escribir(
    " Vtemp -> A5\n"
  );

  elPuerto.escribir(
    " ADC   -> 12 bits, 3.3 V\n"
  );

  elPuerto.escribir(
    "========================================\n"
  );


  // ----------------------------------------------------------
  // CALIBRACIÓN DE CERO
  //
  // En este momento el sensor debe encontrarse en aire limpio.
  //
  // Se toman 50 muestras de Vgas y Vref y se obtiene:
  //
  // offset = Vgas0 - Vref0
  // ----------------------------------------------------------

  elPuerto.escribir(
    "\nCALIBRANDO CERO: mantener el sensor en aire limpio...\n"
  );


  Medidor::CalibracionO3 calibracion =
    elMedidor.calibrarAireLimpio();


  // ----------------------------------------------------------
  // Mostrar resultados de calibración.
  // ----------------------------------------------------------

  elPuerto.escribir(
    "ADC Vgas aire limpio = "
  );

  elPuerto.escribir(
    calibracion.adcVgasPromedio
  );

  elPuerto.escribir(
    "\n"
  );


  elPuerto.escribir(
    "ADC Vref aire limpio = "
  );

  elPuerto.escribir(
    calibracion.adcVrefPromedio
  );

  elPuerto.escribir(
    "\n"
  );


  elPuerto.escribir(
    "Vgas0 promedio = "
  );

  elPuerto.escribir(
    calibracion.vgasAireLimpio
  );

  elPuerto.escribir(
    " V\n"
  );


  elPuerto.escribir(
    "Vref0 promedio = "
  );

  elPuerto.escribir(
    calibracion.vrefAireLimpio
  );

  elPuerto.escribir(
    " V\n"
  );


  elPuerto.escribir(
    "OFFSET = Vgas0 - Vref0 = "
  );

  elPuerto.escribir(
    calibracion.offsetV
  );

  elPuerto.escribir(
    " V\n"
  );


  elPuerto.escribir(
    "M (sensibilidad electrica) = "
  );

  elPuerto.escribir(
    calibracion.sensibilidadVPorPPM
  );

  elPuerto.escribir(
    " V/ppm\n"
  );


  elPuerto.escribir(
    "CALIBRACION TERMINADA.\n\n"
  );


  // ----------------------------------------------------------
  // La comunicación BLE se inicia después de haber
  // realizado la calibración del sensor.
  // ----------------------------------------------------------
  elPublicador.encenderEmisora();
}


// --------------------------------------------------------------
// Estado utilizado por el bucle principal.
// --------------------------------------------------------------
namespace Loop {

  uint16_t cont = 0;
};


// ------------------------------------------------------------
// loop() -->
// ------------------------------------------------------------
//
// Ciclo principal del sistema:
//
// 1. Incrementa el contador de iteraciones.
// 2. Obtiene una medición de O3.
// 3. Muestra la medición por puerto serie.
// 4. Publica un anuncio iBeacon.
// 5. Espera antes de realizar la siguiente lectura.
//
// Arduino ejecuta esta función continuamente.
// ------------------------------------------------------------
void loop() {

  using namespace Loop;

  using namespace Globales;


  // ----------------------------------------------------------
  // Número de lectura actual.
  // ----------------------------------------------------------
  cont++;


  // ----------------------------------------------------------
  // Obtener una nueva medición del sensor.
  // ----------------------------------------------------------
  Medidor::MedicionO3 medida =
    elMedidor.medirOzono();


  // ----------------------------------------------------------
  // Mostrar información por puerto serie.
  // ----------------------------------------------------------

  elPuerto.escribir(
    "\n---- LECTURA O3 #"
  );

  elPuerto.escribir(
    cont
  );

  elPuerto.escribir(
    " ----\n"
  );


  elPuerto.escribir(
    "Vgas = "
  );

  elPuerto.escribir(
    medida.vgas
  );

  elPuerto.escribir(
    " V   [ADC="
  );

  elPuerto.escribir(
    medida.adcVgas
  );

  elPuerto.escribir(
    "]\n"
  );


  elPuerto.escribir(
    "Vref = "
  );

  elPuerto.escribir(
    medida.vref
  );

  elPuerto.escribir(
    " V   [ADC="
  );

  elPuerto.escribir(
    medida.adcVref
  );

  elPuerto.escribir(
    "]\n"
  );


  elPuerto.escribir(
    "Vtemp = "
  );

  elPuerto.escribir(
    medida.vtemp
  );

  elPuerto.escribir(
    " V   [ADC="
  );

  elPuerto.escribir(
    medida.adcVtemp
  );

  elPuerto.escribir(
    "]\n"
  );


  elPuerto.escribir(
    "Temperatura aprox = "
  );

  elPuerto.escribir(
    medida.temperaturaC
  );

  elPuerto.escribir(
    " C\n"
  );


  elPuerto.escribir(
    "Vgas - Vref = "
  );

  elPuerto.escribir(
    medida.diferenciaV
  );

  elPuerto.escribir(
    " V\n"
  );


  elPuerto.escribir(
    "Tras quitar offset = "
  );

  elPuerto.escribir(
    medida.diferenciaCorregidaV
  );

  elPuerto.escribir(
    " V\n"
  );


  elPuerto.escribir(
    "O3 = "
  );

  elPuerto.escribir(
    medida.ozonoPPM
  );

  elPuerto.escribir(
    " ppm\n"
  );


  // ----------------------------------------------------------
  // Información correspondiente al iBeacon actual.
  //
  // En esta versión:
  //
  // Major = contador de prueba
  // Minor = 0
  // ----------------------------------------------------------

  elPuerto.escribir(
    "iBeacon Major = "
  );

  elPuerto.escribir(
    cont
  );

  elPuerto.escribir(
    " (contador de prueba)\n"
  );


  elPuerto.escribir(
    "iBeacon Minor = 0 (valor fijo de prueba)\n"
  );


  // ----------------------------------------------------------
  // Publicación BLE.
  //
  // medirOzono() calcula ozonoPPMx1000 aunque actualmente
  // Publicador no lo introduce en Minor.
  //
  // El anuncio permanece activo durante 1000 ms.
  // ----------------------------------------------------------
  elPublicador.publicarOzono(
    medida.ozonoPPMx1000,
    cont,
    1000
  );


  // ----------------------------------------------------------
  // Espera adicional antes de comenzar otra iteración.
  //
  // Aproximadamente:
  //
  // 1 segundo anunciando
  // +
  // 1 segundo de espera
  //
  // = nueva lectura aproximadamente cada 2 segundos.
  // ----------------------------------------------------------
  esperar(1000);
}