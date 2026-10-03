# Diseño del Componente

El componente **sensor** inicializa el hardware de medida, calibra el sensor de ozono ULPSM-O3 968-046, obtiene las señales analógicas Vgas, Vref y Vtemp, calcula una concentración de O3 y publica un anuncio BLE en formato iBeacon.

## Tipos de datos

```text
CalibracionO3 = (
    adc_vgas_promedio: N,
    adc_vref_promedio: N,
    vgas_aire_limpio: R,
    vref_aire_limpio: R,
    offset_v: R,
    sensibilidad_v_por_ppm: R
)

MedicionO3 = (
    adc_vgas: N,
    adc_vref: N,
    adc_vtemp: N,
    vgas: R,
    vref: R,
    vtemp: R,
    temperatura_c: R,
    diferencia_v: R,
    diferencia_corregida_v: R,
    ozono_ppm: R,
    ozono_ppm_x1000: N
)

MedicionesID = { OZONO }
```

## Flujo principal

```text
setup()
   |
   +--> iniciarMedidor()
   +--> calibrarAireLimpio()
   +--> encenderEmisora()

loop()
   |
   +--> medirOzono()
   +--> escribir diagnóstico por puerto serie
   +--> publicarOzono()
   +--> esperar()
```

```text
inicializarPlaquita()
setup()
loop()
tiempo: Z --> esperar()
```

## Clase Medidor

Responsabilidad: adquirir y tratar las señales analógicas del ULPSM-O3 968-046.

```text
                 ---------------- Medidor -----------------
                 |
                 | offset_v: R
                 | calibrado: B
                 |
                 | adc: N --> adcAVoltios() --> voltios: R
                 |
                 | calcularSensibilidadVPorPPM() --> sensibilidad: R
                 |
                 | numero_muestras: N, leer_temperatura: B
                 | --> leerPromediosADC()
                 | --> adc_vgas: N, adc_vref: N, adc_vtemp: N
                 |
                 | vref: R, vtemp: R
                 | --> calcularTemperatura() --> temperatura: R
                 |
                 |
                 Medidor() -->
                 |
                 |
                 iniciarMedidor() -->
                 |
                 |
calibracion: CalibracionO3 <-- calibrarAireLimpio() -->
                 |
                 |
medicion: MedicionO3 <-- medirOzono() <--
                 |
                 |
offset: R        <-- getOffsetV() <--
                 |
sensibilidad: R  <-- getSensibilidadVPorPPM() <--
                 |
                 ------------------------------------------
```

La calibración y el cálculo principal son:

```text
offset_v = vgas_aire_limpio - vref_aire_limpio
sensibilidad_v_por_ppm = sensibilidad_na_por_ppm * tia_gain_kv_por_a * 10^-6
diferencia_v = vgas - vref
diferencia_corregida_v = diferencia_v - offset_v
ozono_ppm = diferencia_corregida_v / sensibilidad_v_por_ppm
```

## Clase Publicador

Responsabilidad: decidir los campos lógicos del iBeacon de O3 y delegar la emisión en `EmisoraBLE`.

```text
                 ---------------- Publicador ----------------
                 |
                 | beacon_uuid: [ N ]_16
                 | emisora: EmisoraBLE
                 | rssi: Z
                 | major_como_contador: B
                 |
                 |
                 Publicador() -->
                 |
                 |
                 encenderEmisora() -->
                 |
                 |
valor_ppm_x1000: N,
contador: N,
tiempo_espera: Z --> publicarOzono() -->
                 |
                 --------------------------------------------
```

En el estado actual:

```text
major = contador
minor = 0
```

## Clase EmisoraBLE

Responsabilidad: encapsular la configuración y emisión BLE/iBeacon.

```text
                 ---------------- EmisoraBLE ----------------
                 |
                 | nombre_emisora: Text
                 | fabricante_id: N
                 | tx_power: Z
                 |
                 | detenerAnuncio() -->
                 | estaAnunciando() --> anunciando: B
                 | carga: Text, tamanyo_carga: N
                 | --> emitirAnuncioIBeaconLibre() -->
                 | servicio: ServicioEnEmisora
                 | --> anyadirServicio() --> resultado: B
                 |
                 |
nombre: Text,
fabricante_id: N,
tx_power: Z      --> EmisoraBLE() -->
                 |
                 |
                 encenderEmisora() -->
                 |
                 |
beacon_uuid: [ N ]_16,
major: N,
minor: N,
rssi: Z          --> emitirAnuncioIBeacon() -->
                 |
                 --------------------------------------------
```

La implementación conserva además operaciones auxiliares para callbacks, servicios y características BLE procedentes del código base.

## Clase LED

```text
                 ---------------- LED ----------------
                 |
                 | numero_led: Z
                 | encendido: B
                 |
                 |
numero: Z      --> LED() -->
                 |
                 | encender() -->
                 | apagar() -->
                 | alternar() -->
                 |
tiempo: Z      --> brillar() -->
                 |
                 -------------------------------------
```

## Clase PuertoSerie

```text
                 ------------- PuertoSerie -------------
                 |
                 |
baudios: Z      --> PuertoSerie() -->
                 |
                 | esperarDisponible() -->
                 |
mensaje: T      --> escribir() -->
                 |
                 ---------------------------------------
```

`T` representa un tipo de dato imprimible por el puerto serie.

## ServicioEnEmisora y características BLE

`ServicioEnEmisora` y su clase interna `Caracteristica` encapsulan la funcionalidad GATT heredada del proyecto base: creación de UUID, configuración de propiedades/permisos, escritura, notificación, asociación de características y activación de servicios. Esta funcionalidad no participa en el flujo iBeacon activo del Sprint 0, pero forma parte del código existente del componente sensor.

# Aclaraciones del Diseño

- El sensor utilizado es el **SPEC Sensors ULPSM-O3 968-046**.
- La implementación actual conecta `Vgas` a `A3`, `Vref` a `A4` y `Vtemp` a `A5`.
- El ADC se configura a 12 bits y se trabaja con un rango considerado de 0 a 3.3 V.
- Tanto la calibración como la medida utilizan un promedio de 50 muestras.
- La calibración se realiza al arrancar suponiendo que el sensor se encuentra en aire limpio.
- La sensibilidad configurada actualmente es `-59.20 nA/ppm` y el TIA Gain es `499 kV/A`.
- La temperatura calculada se utiliza como diagnóstico y no interviene en el cálculo actual de ppm.
- El UUID del publicador corresponde a `EPSG-GTI-PROY-3A`.
- El nombre BLE configurado actualmente en `Publicador` es `gatotico`.
- Durante la prueba actual `Major` se utiliza como contador y `Minor` se mantiene fijo a `0`.
- `EmisoraBLE.h` y `ServicioEnEmisora.h` conservan funcionalidad BLE/GATT genérica heredada que no forma parte del flujo activo de publicación de O3.

# Reglas Generales

- **Lenguaje de programación:** C++ para Arduino sobre SparkFun Pro nRF52840 Mini, utilizando la librería Adafruit Bluefruit nRF52.
- **Encabezados de funciones/métodos:** cada función o método deberá incluir su diseño lógico dentro de un bloque de comentarios delimitado por líneas discontinuas (`--------------------`).
- **Legibilidad del código:** el código deberá mantener separadas las responsabilidades de medición, publicación iBeacon, comunicación BLE, puerto serie y utilidades de hardware.
- **Pruebas automatizadas:** se deberán generar pruebas unitarias o de integración para las funciones y métodos críticos. Las operaciones dependientes directamente de ADC, BLE o periféricos deberán aislarse mediante mocks/abstracciones cuando sea viable y complementarse con pruebas de integración sobre la placa.
