// -*-c++-*-

// --------------------------------------------------------------
// Basado en HolaMundoIBeacon de Jordi Bataller i Mascarell
// Adaptado para SPEC ULPSM-O3 968-046.
//
// NUEVO EN ESTA VERSION:
// - ADC de 12 bits con escala del montaje 0..3.3 V.
// - Calibración automática del cero con 50 muestras en aire limpio.
// - Offset = promedio(Vgas) - promedio(Vref).
// - Sensibilidad M calculada desde Sensitivity Code y TIA Gain.
// - ppm = ((Vgas - Vref) - offset) / M.
// - Major = contador de prueba.
// - Minor = 0 (fijado temporalmente para la prueba).
// --------------------------------------------------------------

#include <bluefruit.h>

#undef min
#undef max

#include "LED.h"
#include "PuertoSerie.h"

namespace Globales {
  LED elLED(7);
  PuertoSerie elPuerto(115200);
};

#include "EmisoraBLE.h"
#include "Publicador.h"
#include "Medidor.h"

namespace Globales {
  Publicador elPublicador;
  Medidor elMedidor;
};

void inicializarPlaquita() {
}

// --------------------------------------------------------------
void setup() {
  using namespace Globales;

  //elPuerto.esperarDisponible();
  inicializarPlaquita();
  elMedidor.iniciarMedidor();

  elPuerto.escribir("\n========================================\n");
  elPuerto.escribir(" SENSOR O3 ULPSM-O3 968-046\n");
  elPuerto.escribir(" Vgas  -> A3\n");
  elPuerto.escribir(" Vref  -> A4\n");
  elPuerto.escribir(" Vtemp -> A5\n");
  elPuerto.escribir(" ADC   -> 12 bits, 3.3 V\n");
  elPuerto.escribir("========================================\n");

  // ------------------------------------------------------------
  // CALIBRACION. En este momento el sensor debe estar en aire limpio.
  // Se toman 50 muestras y se obtiene el offset del cero.
  // ------------------------------------------------------------
  elPuerto.escribir("\nCALIBRANDO CERO: mantener el sensor en aire limpio...\n");
  Medidor::CalibracionO3 calibracion = elMedidor.calibrarAireLimpio();

  elPuerto.escribir("ADC Vgas aire limpio = ");
  elPuerto.escribir(calibracion.adcVgasPromedio);
  elPuerto.escribir("\n");

  elPuerto.escribir("ADC Vref aire limpio = ");
  elPuerto.escribir(calibracion.adcVrefPromedio);
  elPuerto.escribir("\n");

  elPuerto.escribir("Vgas0 promedio = ");
  elPuerto.escribir(calibracion.vgasAireLimpio);
  elPuerto.escribir(" V\n");

  elPuerto.escribir("Vref0 promedio = ");
  elPuerto.escribir(calibracion.vrefAireLimpio);
  elPuerto.escribir(" V\n");

  elPuerto.escribir("OFFSET = Vgas0 - Vref0 = ");
  elPuerto.escribir(calibracion.offsetV);
  elPuerto.escribir(" V\n");

  elPuerto.escribir("M (sensibilidad electrica) = ");
  elPuerto.escribir(calibracion.sensibilidadVPorPPM);
  elPuerto.escribir(" V/ppm\n");

  elPuerto.escribir("CALIBRACION TERMINADA.\n\n");

  // Encendemos BLE después de obtener el cero.
  elPublicador.encenderEmisora();
}

// --------------------------------------------------------------
namespace Loop {
  uint16_t cont = 0;
};

// --------------------------------------------------------------
void loop() {
  using namespace Loop;
  using namespace Globales;

  cont++;

  Medidor::MedicionO3 medida = elMedidor.medirOzono();

  elPuerto.escribir("\n---- LECTURA O3 #");
  elPuerto.escribir(cont);
  elPuerto.escribir(" ----\n");

  elPuerto.escribir("Vgas = ");
  elPuerto.escribir(medida.vgas);
  elPuerto.escribir(" V   [ADC=");
  elPuerto.escribir(medida.adcVgas);
  elPuerto.escribir("]\n");

  elPuerto.escribir("Vref = ");
  elPuerto.escribir(medida.vref);
  elPuerto.escribir(" V   [ADC=");
  elPuerto.escribir(medida.adcVref);
  elPuerto.escribir("]\n");

  elPuerto.escribir("Vtemp = ");
  elPuerto.escribir(medida.vtemp);
  elPuerto.escribir(" V   [ADC=");
  elPuerto.escribir(medida.adcVtemp);
  elPuerto.escribir("]\n");

  elPuerto.escribir("Temperatura aprox = ");
  elPuerto.escribir(medida.temperaturaC);
  elPuerto.escribir(" C\n");

  elPuerto.escribir("Vgas - Vref = ");
  elPuerto.escribir(medida.diferenciaV);
  elPuerto.escribir(" V\n");

  elPuerto.escribir("Tras quitar offset = ");
  elPuerto.escribir(medida.diferenciaCorregidaV);
  elPuerto.escribir(" V\n");

  elPuerto.escribir("O3 = ");
  elPuerto.escribir(medida.ozonoPPM);
  elPuerto.escribir(" ppm\n");

  elPuerto.escribir("iBeacon Major = ");
  elPuerto.escribir(cont);
  elPuerto.escribir(" (contador de prueba)\n");

  elPuerto.escribir("iBeacon Minor = 0 (valor fijo de prueba)\n");

  // Se mantiene el anuncio 1 segundo.
  elPublicador.publicarOzono(
    medida.ozonoPPMx1000,
    cont,
    1000
  );

  // Nueva lectura aproximadamente cada 2 segundos.
  esperar(1000);
}
