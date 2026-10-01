# Diseño del Componente

El componente **sensor** se encarga de inicializar el hardware de medida, calibrar el sensor de ozono ULPSM-O3 968-046, obtener medidas analógicas de Vgas, Vref y Vtemp, calcular la concentración de O3 y publicar un anuncio BLE en formato iBeacon.

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

## Funciones principales del programa

```text
inicializarPlaquita()

setup()

loop()

tiempo: Z --> esperar()
```

`setup()` inicializa el medidor, realiza la calibración de cero en aire limpio y enciende la emisora BLE.

`loop()` obtiene una medición de ozono, muestra los valores de diagnóstico por puerto serie y solicita su publicación mediante iBeacon.

## Clase Medidor

Responsabilidad: gestionar exclusivamente la adquisición y tratamiento de las señales analógicas del sensor ULPSM-O3 968-046.

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
medicion: MedicionO3     <-- medirOzono() -->
                 |
                 |
       offset: R         <-- getOffsetV() <--
                 |
                 |
 sensibilidad: R         <-- getSensibilidadVPorPPM() <--
                 |
                 ------------------------------------------
```

La calibración calcula el cero del sensor mediante:

```text
offset_v = vgas_aire_limpio - vref_aire_limpio
```

La sensibilidad eléctrica se obtiene mediante:

```text
sensibilidad_v_por_ppm = sensibilidad_na_por_ppm * tia_gain_kv_por_a * 10^-6
```

La concentración de ozono se calcula mediante:

```text
diferencia_v = vgas - vref

diferencia_corregida_v = diferencia_v - offset_v

ozono_ppm = diferencia_corregida_v / sensibilidad_v_por_ppm
```

Si el medidor todavía no se ha calibrado o la sensibilidad es prácticamente cero, la concentración devuelta será 0. Los valores negativos producidos por pequeñas variaciones o ruido se limitan igualmente a 0.

## Clase Publicador

Responsabilidad: transformar la información que debe publicarse en los campos utilizados por el iBeacon y solicitar a `EmisoraBLE` la emisión del anuncio.

```text
                 ---------------- Publicador ----------------
                 |
                 | beacon_uuid: [N]_16
                 | rssi: Z
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

En el estado actual del Sprint 0:

```text
major = contador
minor = 0
```

La concentración de O3 se calcula internamente, pero temporalmente no se introduce en `minor`.

## Clase EmisoraBLE

Responsabilidad: encapsular la comunicación BLE y la construcción/emisión del anuncio iBeacon.

```text
                 ---------------- EmisoraBLE ----------------
                 |
                 | nombre_emisora: Text
                 | fabricante_id: N
                 | tx_power: Z
                 |
                 |
nombre: Text,
fabricante_id: N,
tx_power: Z    --> EmisoraBLE() -->
                 |
                 |
                 encenderEmisora() -->
                 |
                 |
                 detenerAnuncio() -->
                 |
                 |
anunciando: B  <-- estaAnunciando() <--
                 |
                 |
beacon_uuid: [N]_16,
major: N,
minor: N,
rssi: Z        --> emitirAnuncioIBeacon() -->
                 |
                 --------------------------------------------
```

`emitirAnuncioIBeacon()` detiene cualquier anuncio anterior, limpia los datos BLE previos, configura UUID, Major, Minor, RSSI, fabricante y potencia, y comienza un nuevo anuncio.

## Clase LED

Responsabilidad: encapsular el control del LED de la placa.

```text
                 ---------------- LED ----------------
                 |
                 | numero_led: Z
                 | encendido: B
                 |
                 |
numero: Z      --> LED() -->
                 |
                 |
                 encender() -->
                 |
                 |
                 apagar() -->
                 |
                 |
                 alternar() -->
                 |
                 |
tiempo: Z      --> brillar() -->
                 |
                 -------------------------------------
```

## Clase PuertoSerie

Responsabilidad: encapsular la comunicación utilizada para mostrar información de diagnóstico por puerto serie.

```text
                 ------------- PuertoSerie -------------
                 |
                 |
baudios: Z      --> PuertoSerie() -->
                 |
                 |
                 esperarDisponible() -->
                 |
                 |
mensaje: Text   --> escribir() -->
                 |
                 ---------------------------------------
```

# Aclaraciones del Diseño

- El sensor utilizado es el **SPEC Sensors ULPSM-O3 968-046**.
- La implementación actual conecta `Vgas` a `A3`, `Vref` a `A4` y `Vtemp` a `A5`.
- El ADC se configura a 12 bits y se trabaja con un rango de 0 a 3.3 V.
- Tanto la calibración como la medida utilizan un promedio de 50 muestras.
- La calibración debe realizarse al arrancar mientras el sensor se encuentra en aire limpio.
- La sensibilidad configurada actualmente en el código es `-59.20 nA/ppm` y el TIA Gain es `499 kV/A`.
- La temperatura calculada a partir de `Vtemp` se utiliza como información de diagnóstico y no interviene actualmente en el cálculo de ppm.
- El UUID empleado por el publicador corresponde a `EPSG-GTI-PROY-3A`.
- Durante la prueba actual, `Major` se utiliza como contador de iteraciones y `Minor` se mantiene fijo a 0.
- `EmisoraBLE.h` y `ServicioEnEmisora.h` contienen además funcionalidad BLE genérica heredada del proyecto base. Las operaciones de servicios y características BLE no forman parte del flujo activo del Sprint 0 descrito en este diseño.

# Reglas Generales

- **Lenguaje de programación:** C++ para Arduino sobre la placa SparkFun Pro nRF52840 Mini, utilizando la librería Adafruit Bluefruit nRF52.
- **Encabezados de funciones/métodos:** cada función o método deberá incluir en su cabecera su diseño lógico dentro de un bloque de comentarios delimitado por líneas discontinuas (`--------------------`).
- **Legibilidad del código:** el código deberá ser claro y autoexplicativo, manteniendo separadas las responsabilidades de medición, publicación BLE y utilidades de hardware.
- **Pruebas automatizadas:** se deberán generar pruebas unitarias o de integración para las funciones y métodos críticos siempre que puedan aislarse del hardware. Las operaciones que dependan directamente del ADC, BLE o periféricos deberán verificarse mediante pruebas de integración sobre la placa o mediante abstracciones/mocks cuando sea posible.
