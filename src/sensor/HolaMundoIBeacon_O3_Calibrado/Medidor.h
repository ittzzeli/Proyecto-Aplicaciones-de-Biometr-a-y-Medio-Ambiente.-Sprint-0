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
// Funcionamiento:
//   1) El ADC trabaja a 12 bits (0..4095) y tomamos 3.3 V
//      como fondo de escala del montaje.
//
//   2) Al arrancar se toman 50 muestras EN AIRE LIMPIO.
//
//   3) Se calcula el offset real:
//        offset = Vgas_aire_limpio - Vref_aire_limpio
//
//   4) En cada medida:
//        deltaV = (Vgas - Vref) - offset
//        ppm    = deltaV / M
//
//   5) M se obtiene a partir del Sensitivity Code del sensor:
//        M [V/ppm] = Sensitivity[nA/ppm] * TIA[kV/A] * 1e-6
// ------------------------------------------------------

class Medidor {

public:

  // ------------------------------------------------------
  // Tipo lógico:
  //
  // CalibracionO3 =
  // (
  //   adc_vgas_promedio: N,
  //   adc_vref_promedio: N,
  //   vgas_aire_limpio: R,
  //   vref_aire_limpio: R,
  //   offset_v: R,
  //   sensibilidad_v_por_ppm: R
  // )
  // ------------------------------------------------------
  struct CalibracionO3 {
    uint16_t adcVgasPromedio;
    uint16_t adcVrefPromedio;

    float vgasAireLimpio;
    float vrefAireLimpio;

    float offsetV;
    float sensibilidadVPorPPM;
  };


  // ------------------------------------------------------
  // Tipo lógico:
  //
  // MedicionO3 =
  // (
  //   adc_vgas: N,
  //   adc_vref: N,
  //   adc_vtemp: N,
  //   vgas: R,
  //   vref: R,
  //   vtemp: R,
  //   temperatura_c: R,
  //   diferencia_v: R,
  //   diferencia_corregida_v: R,
  //   ozono_ppm: R,
  //   ozono_ppm_x1000: N
  // )
  // ------------------------------------------------------
  struct MedicionO3 {
    uint16_t adcVgas;
    uint16_t adcVref;
    uint16_t adcVtemp;

    float vgas;
    float vref;
    float vtemp;
    float temperaturaC;

    // Vgas - Vref antes de corregir el cero.
    float diferenciaV;

    // (Vgas - Vref) - offset.
    float diferenciaCorregidaV;

    float ozonoPPM;

    // Representación entera de ppm * 1000.
    // Se mantiene preparada para su posible publicación,
    // aunque actualmente Publicador fija Minor a 0.
    uint16_t ozonoPPMx1000;
  };


private:

  static const uint8_t PIN_VGAS  = A3;
  static const uint8_t PIN_VREF  = A4;
  static const uint8_t PIN_VTEMP = A5;


  // ADC: 12 bits => 0..4095.
  static constexpr uint16_t ADC_MAX = 4095;

  // El montaje trabaja a 3.3 V.
  // Con AR_VDD4 el rango del SAADC queda referenciado a VDD;
  // aquí suponemos VDD = 3.3 V.
  static constexpr float ADC_RANGO_V = 3.3f;


  // Promedio utilizado tanto para la calibración como
  // para estabilizar cada medida.
  static const uint8_t NUM_MUESTRAS_CALIBRACION = 50;
  static const uint8_t NUM_MUESTRAS_MEDIDA = 50;


  // ----------------------------------------------------
  // SENSIBILIDAD DEL SENSOR
  // ----------------------------------------------------

  // Para O3, el manual de SPEC indica TIA Gain = 499 kV/A.
  static constexpr float TIA_GAIN_KV_POR_A = 499.0f;

  // IMPORTANTE:
  // Sustituir este valor por el "Sensitivity Code"
  // correspondiente al sensor utilizado, expresado en nA/ppm.
  //
  // En esta versión se utiliza -59.20 nA/ppm.
  static constexpr float SENSIBILIDAD_NA_POR_PPM = -59.20f;


  // El offset no es fijo:
  // se calcula al arrancar mediante la calibración en aire limpio.
  float offsetV = 0.0f;

  bool calibrado = false;


  // ------------------------------------------------------------
  // adc: N --> adcAVoltios() --> voltios: R
  // ------------------------------------------------------------
  float adcAVoltios(uint16_t adc) const {

    return ((float) adc * ADC_RANGO_V) / (float) ADC_MAX;
  }


  // ------------------------------------------------------------
  // calcularSensibilidadVPorPPM() --> sensibilidad: R
  // ------------------------------------------------------------
  //
  // Factor M [V/ppm] calculado a partir del Sensitivity Code.
  //
  // nA/ppm * kV/A * (1e-9 A/nA) * (1e3 V/kV)
  //
  // = nA/ppm * kV/A * 1e-6
  //
  // = V/ppm
  // ------------------------------------------------------------
  float calcularSensibilidadVPorPPM() const {

    return SENSIBILIDAD_NA_POR_PPM
           * TIA_GAIN_KV_POR_A
           * 1.0e-6f;
  }


  // ------------------------------------------------------------
  // numero_muestras: N, leer_temperatura: B
  // --> leerPromediosADC()
  // --> adc_vgas: N, adc_vref: N, adc_vtemp: N
  // ------------------------------------------------------------
  //
  // Lee Vgas, Vref y opcionalmente Vtemp de forma intercalada.
  //
  // Las lecturas se realizan intercaladas para que Vgas y Vref
  // representen aproximadamente el mismo intervalo temporal.
  //
  // En C++ adcVgas, adcVref y adcVtemp se implementan mediante
  // referencias, pero lógicamente son datos de salida.
  // ------------------------------------------------------------
  void leerPromediosADC(uint8_t numeroMuestras,
                        uint16_t &adcVgas,
                        uint16_t &adcVref,
                        uint16_t &adcVtemp,
                        bool leerTemperatura) {

    uint32_t sumaVgas = 0;
    uint32_t sumaVref = 0;
    uint32_t sumaVtemp = 0;


    // Lecturas iniciales descartadas para que el multiplexor
    // del ADC se estabilice al cambiar de canal.
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


  // ------------------------------------------------------------
  // vref: R, vtemp: R
  // --> calcularTemperatura()
  // --> temperatura_c: R
  // ------------------------------------------------------------
  //
  // Temperatura aproximada según el manual de SPEC:
  //
  // T = (87 / V+) * Vtemp - 18
  //
  // Como Vref ~= V+/2:
  //
  // V+ ~= 2 * Vref
  //
  // La temperatura se utiliza para depuración, pero no interviene
  // en el cálculo de ppm de esta versión.
  // ------------------------------------------------------------
  float calcularTemperatura(float vref, float vtemp) const {

    const float vAlimentacion = 2.0f * vref;

    if (vAlimentacion < 0.1f) {
      return -99.0f;
    }

    return (87.0f / vAlimentacion) * vtemp - 18.0f;
  }


public:

  // ------------------------------------------------------------
  // Medidor()
  // ------------------------------------------------------------
  Medidor() {
  }


  // ------------------------------------------------------------
  // iniciarMedidor() -->
  // ------------------------------------------------------------
  //
  // Configura las entradas analógicas y el ADC del nRF52840.
  // ------------------------------------------------------------
  void iniciarMedidor() {

    pinMode(PIN_VGAS, INPUT);
    pinMode(PIN_VREF, INPUT);
    pinMode(PIN_VTEMP, INPUT);


    // 12 bits => 4096 códigos: 0..4095.
    analogReadResolution(12);


    // En el core nRF52, AR_VDD4 configura el SAADC para medir
    // aproximadamente 0..VDD.
    //
    // En este montaje se considera VDD = 3.3 V.
    analogReference(AR_VDD4);


    // Vref y Vtemp son salidas de alta impedancia.
    // Un tiempo de adquisición largo y el oversampling
    // ayudan a reducir el ruido.
    analogSampleTime(40);
    analogOversampling(16);

    analogCalibrateOffset();

    delay(10);
  }


  // ------------------------------------------------------------
  // calibrarAireLimpio() --> calibracion: CalibracionO3
  // ------------------------------------------------------------
  //
  // CALIBRACIÓN DE CERO EN AIRE LIMPIO.
  //
  // Debe ejecutarse antes de introducir el sensor en ozono.
  //
  // La función modifica el estado interno:
  //   offsetV
  //   calibrado
  // ------------------------------------------------------------
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


    // Offset medido realmente cuando la concentración
    // de ozono considerada es 0 ppm.
    offsetV = c.vgasAireLimpio - c.vrefAireLimpio;

    calibrado = true;


    c.offsetV = offsetV;

    c.sensibilidadVPorPPM =
      calcularSensibilidadVPorPPM();


    return c;
  }


  // ------------------------------------------------------------
  // medirOzono() --> medicion: MedicionO3
  // ------------------------------------------------------------
  //
  // Obtiene las señales analógicas, elimina el offset calculado
  // durante la calibración y calcula la concentración de O3.
  // ------------------------------------------------------------
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


    m.temperaturaC =
      calcularTemperatura(m.vref, m.vtemp);


    // Diferencia actual respecto a la referencia eléctrica.
    m.diferenciaV = m.vgas - m.vref;


    // Eliminamos el offset medido durante
    // la calibración en aire limpio.
    m.diferenciaCorregidaV =
      m.diferenciaV - offsetV;


    const float M =
      calcularSensibilidadVPorPPM();


    // No calculamos ppm si todavía no existe una calibración
    // válida o si la sensibilidad fuese prácticamente cero.
    if (!calibrado ||
        (M > -0.000001f && M < 0.000001f)) {

      m.ozonoPPM = 0.0f;

    } else {

      // Fórmula de SPEC:
      //
      // Cx = (Vgas - Vgas0) / M
      //
      // siendo:
      //
      // Vgas0 = Vref + offset
      //
      // por tanto:
      //
      // Cx = ((Vgas - Vref) - offset) / M
      m.ozonoPPM =
        m.diferenciaCorregidaV / M;
    }


    // El ruido puede producir una concentración
    // ligeramente negativa.
    //
    // Para esta aplicación se limita a 0 ppm.
    if (m.ozonoPPM < 0.0f) {
      m.ozonoPPM = 0.0f;
    }


    // --------------------------------------------------
    // Representación entera de la concentración:
    //
    // valor = ppm * 1000
    //
    // Ejemplos:
    //
    //   0.235 ppm -> 235
    //   1.500 ppm -> 1500
    //
    // Permite mantener tres cifras decimales usando
    // un entero de 16 bits.
    //
    // Actualmente Publicador conserva este valor,
    // pero el Minor del iBeacon permanece fijado a 0.
    // --------------------------------------------------

    float ppmEscalados =
      m.ozonoPPM * 1000.0f;


    if (ppmEscalados > 65535.0f) {
      ppmEscalados = 65535.0f;
    }


    m.ozonoPPMx1000 =
      (uint16_t) (ppmEscalados + 0.5f);


    return m;
  }


  // ------------------------------------------------------------
  // getOffsetV() --> offset: R
  // ------------------------------------------------------------
  float getOffsetV() const {

    return offsetV;
  }


  // ------------------------------------------------------------
  // getSensibilidadVPorPPM() --> sensibilidad: R
  // ------------------------------------------------------------
  float getSensibilidadVPorPPM() const {

    return calcularSensibilidadVPorPPM();
  }

}; // class Medidor

#endif