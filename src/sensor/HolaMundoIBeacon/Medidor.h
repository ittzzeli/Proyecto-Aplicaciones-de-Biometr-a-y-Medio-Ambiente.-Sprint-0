// -*- mode: c++ -*-

#ifndef MEDIDOR_H_INCLUIDO
#define MEDIDOR_H_INCLUIDO

// ------------------------------------------------------
// Medidor para SPEC Sensors ULPSM-O3 968-046
//
// Conexiones usadas:
//   Sensor pin 1 (Vgas)  -> SparkFun A3
//   Sensor pin 2 (Vref)  -> SparkFun A4
//   Sensor pin 3 (Vtemp) -> SparkFun A5
//
// CAMBIO NUEVO:
//   1) El ADC trabaja a 12 bits (0..4095) y tomamos 3.3 V
//      como fondo de escala del montaje.
//   2) Al arrancar se toman 50 muestras EN AIRE LIMPIO.
//   3) Se calcula el offset real:
//        offset = Vgas_aire_limpio - Vref_aire_limpio
//   4) En cada medida:
//        deltaV = (Vgas - Vref) - offset
//        ppm    = deltaV / M
//   5) M se obtiene a partir del Sensitivity Code del sensor:
//        M [V/ppm] = Sensitivity[nA/ppm] * TIA[kV/A] * 1e-6
// ------------------------------------------------------
class Medidor {

public:
  struct CalibracionO3 {
    uint16_t adcVgasPromedio;
    uint16_t adcVrefPromedio;
    float vgasAireLimpio;
    float vrefAireLimpio;
    float offsetV;
    float sensibilidadVPorPPM;
  };

  struct MedicionO3 {
    uint16_t adcVgas;
    uint16_t adcVref;
    uint16_t adcVtemp;

    float vgas;
    float vref;
    float vtemp;
    float temperaturaC;

    float diferenciaV;          // Vgas - Vref antes de corregir el cero
    float diferenciaCorregidaV; // (Vgas - Vref) - offset
    float ozonoPPM;
    uint16_t ozonoPPMx1000;     // formato que enviamos en Minor
  };

private:
  static const uint8_t PIN_VGAS  = A3;
  static const uint8_t PIN_VREF  = A4;
  static const uint8_t PIN_VTEMP = A5;

  // ADC: 12 bits => 0..4095.
  static constexpr uint16_t ADC_MAX = 4095;
  // El montaje trabaja a 3.3 V. Con AR_VDD4 el rango del SAADC
  // queda referenciado a VDD; aquí suponemos VDD = 3.3 V.
  static constexpr float ADC_RANGO_V = 3.3f;

  // Promedio solicitado para estabilizar tanto el cero como la lectura.
  static const uint8_t NUM_MUESTRAS_CALIBRACION = 50;
  static const uint8_t NUM_MUESTRAS_MEDIDA = 50;

  // ----------------------------------------------------
  // SENSIBILIDAD DEL SENSOR
  // ----------------------------------------------------
  // Para O3, el manual de SPEC indica TIA Gain = 499 kV/A.
  static constexpr float TIA_GAIN_KV_POR_A = 499.0f;

  // IMPORTANTE:
  // Sustituye este valor por el "Sensitivity Code" que aparece
  // en la etiqueta de TU sensor, expresado en nA/ppm.
  //
  // Como valor provisional usamos -60.12 nA/ppm, que con un
  // TIA de 499 kV/A produce aproximadamente -0.030 V/ppm,
  // equivalente al span típico (-30 mV/ppm) del datasheet.
  static constexpr float SENSIBILIDAD_NA_POR_PPM = -59.20f;

  // El offset NO es fijo: se calcula al arrancar con aire limpio.
  float offsetV = 0.0f;
  bool calibrado = false;

  // .....................................................
  float adcAVoltios(uint16_t adc) const {
    return ((float) adc * ADC_RANGO_V) / (float) ADC_MAX;
  }

  // .....................................................
  // Factor M [V/ppm] calculado a partir del Sensitivity Code.
  //
  // nA/ppm * kV/A * (1e-9 A/nA) * (1e3 V/kV)
  // = nA/ppm * kV/A * 1e-6 = V/ppm
  // .....................................................
  float calcularSensibilidadVPorPPM() const {
    return SENSIBILIDAD_NA_POR_PPM * TIA_GAIN_KV_POR_A * 1.0e-6f;
  }

  // .....................................................
  // Lee Vgas, Vref y opcionalmente Vtemp de forma intercalada.
  // Hacer las lecturas intercaladas es mejor que tomar todas las
  // de un canal y luego todas las del otro, porque Vgas y Vref
  // representan prácticamente el mismo intervalo temporal.
  // .....................................................
  void leerPromediosADC(uint8_t numeroMuestras,
                        uint16_t &adcVgas,
                        uint16_t &adcVref,
                        uint16_t &adcVtemp,
                        bool leerTemperatura) {
    uint32_t sumaVgas = 0;
    uint32_t sumaVref = 0;
    uint32_t sumaVtemp = 0;

    // Lecturas iniciales descartadas para que el multiplexor del ADC
    // se estabilice al cambiar de canal.
    (void) analogRead(PIN_VGAS);
    (void) analogRead(PIN_VREF);
    if (leerTemperatura) {
      (void) analogRead(PIN_VTEMP);
    }

    for (uint8_t i = 0; i < numeroMuestras; i++) {
      sumaVgas += analogRead(PIN_VGAS);
      sumaVref += analogRead(PIN_VREF);

      if (leerTemperatura) {
        sumaVtemp += analogRead(PIN_VTEMP);
      }
    }

    adcVgas = (uint16_t) (sumaVgas / numeroMuestras);
    adcVref = (uint16_t) (sumaVref / numeroMuestras);
    adcVtemp = leerTemperatura
      ? (uint16_t) (sumaVtemp / numeroMuestras)
      : 0;
  }

  // .....................................................
  // Temperatura aproximada según el manual de SPEC:
  // T = (87 / V+) * Vtemp - 18
  // Como Vref ~= V+/2, V+ ~= 2*Vref.
  // La temperatura se muestra para depuración, pero NO se utiliza
  // en el cálculo de ppm de esta versión.
  // .....................................................
  float calcularTemperatura(float vref, float vtemp) const {
    const float vAlimentacion = 2.0f * vref;

    if (vAlimentacion < 0.1f) {
      return -99.0f;
    }

    return (87.0f / vAlimentacion) * vtemp - 18.0f;
  }

public:
  Medidor() {
  }

  // .....................................................
  void iniciarMedidor() {
    pinMode(PIN_VGAS, INPUT);
    pinMode(PIN_VREF, INPUT);
    pinMode(PIN_VTEMP, INPUT);

    // 12 bits => 4096 códigos: 0..4095.
    analogReadResolution(12);

    // En el core nRF52, AR_VDD4 configura el SAADC para medir
    // aproximadamente 0..VDD. En nuestro montaje VDD = 3.3 V.
    analogReference(AR_VDD4);

    // Vref y Vtemp son salidas de alta impedancia. Un tiempo de
    // adquisición largo y oversampling ayudan a reducir ruido.
    analogSampleTime(40);
    analogOversampling(16);
    analogCalibrateOffset();

    delay(10);
  }

  // .....................................................
  // CALIBRACIÓN DE CERO EN AIRE LIMPIO.
  // Debe ejecutarse antes de introducir el sensor en ozono.
  // .....................................................
  CalibracionO3 calibrarAireLimpio() {
    CalibracionO3 c;
    uint16_t dummyTemp = 0;

    leerPromediosADC(
      NUM_MUESTRAS_CALIBRACION,
      c.adcVgasPromedio,
      c.adcVrefPromedio,
      dummyTemp,
      false
    );

    c.vgasAireLimpio = adcAVoltios(c.adcVgasPromedio);
    c.vrefAireLimpio = adcAVoltios(c.adcVrefPromedio);

    // Este es el Voffset medido realmente con 0 ppm.
    offsetV = c.vgasAireLimpio - c.vrefAireLimpio;
    calibrado = true;

    c.offsetV = offsetV;
    c.sensibilidadVPorPPM = calcularSensibilidadVPorPPM();

    return c;
  }

  // .....................................................
  MedicionO3 medirOzono() {
    MedicionO3 m;

    leerPromediosADC(
      NUM_MUESTRAS_MEDIDA,
      m.adcVgas,
      m.adcVref,
      m.adcVtemp,
      true
    );

    m.vgas = adcAVoltios(m.adcVgas);
    m.vref = adcAVoltios(m.adcVref);
    m.vtemp = adcAVoltios(m.adcVtemp);
    m.temperaturaC = calcularTemperatura(m.vref, m.vtemp);

    // Diferencia actual respecto a la referencia eléctrica.
    m.diferenciaV = m.vgas - m.vref;

    // Quitamos el offset que medimos en aire limpio.
    m.diferenciaCorregidaV = m.diferenciaV - offsetV;

    const float M = calcularSensibilidadVPorPPM();

    if (!calibrado || M > -0.000001f && M < 0.000001f) {
      m.ozonoPPM = 0.0f;
    } else {
      // Fórmula de SPEC:
      // Cx = (Vgas - Vgas0) / M
      // y Vgas0 = Vref + offset
      // => Cx = ((Vgas - Vref) - offset) / M
      m.ozonoPPM = m.diferenciaCorregidaV / M;
    }

    // Un poco de ruido puede dar una concentración ligeramente negativa.
    if (m.ozonoPPM < 0.0f) {
      m.ozonoPPM = 0.0f;
    }

    // --------------------------------------------------
    // Minor es entero de 16 bits. Para no perder decimales:
    //   Minor = ppm * 1000
    // Ejemplos:
    //   0.235 ppm -> Minor = 235
    //   1.500 ppm -> Minor = 1500
    // En el receptor: ppm = Minor / 1000.0
    // --------------------------------------------------
    float ppmEscalados = m.ozonoPPM * 1000.0f;

    if (ppmEscalados > 65535.0f) {
      ppmEscalados = 65535.0f;
    }

    m.ozonoPPMx1000 = (uint16_t) (ppmEscalados + 0.5f);

    return m;
  }

  float getOffsetV() const {
    return offsetV;
  }

  float getSensibilidadVPorPPM() const {
    return calcularSensibilidadVPorPPM();
  }

}; // class Medidor

#endif
