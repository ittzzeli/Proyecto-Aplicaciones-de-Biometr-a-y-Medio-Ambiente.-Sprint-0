# Diseño del Componente

El componente `sensor` representa el nodo físico del Sprint 0. Su responsabilidad es generar una medición ficticia de O3 y publicarla mediante Bluetooth Low Energy en formato iBeacon.

El diseño toma como referencia el ejemplo `HolaMundoIBeacon` proporcionado por el profesorado, pero se limita a la funcionalidad necesaria para el Sprint 0.

## Tipos de datos

```text
MedicionesID = { O3, TEMPERATURA }
```

Valores asignados en esta versión:

```text
O3          = 11
TEMPERATURA = 12
```

El campo `Major` del iBeacon se divide en dos bytes:

```text
Major = [ tipo ][ contador ]
          8 bit    8 bit
```

Por tanto:

```text
major = (tipo << 8) | contador
```

El campo `Minor` contiene directamente el valor ficticio de la medición. En este Sprint 0:

```text
minor = 1234
```

## Funciones principales

```text
setup()

loop()
```

`setup()` inicializa el puerto serie, el medidor simulado y la emisora BLE.

`loop()` incrementa el contador, obtiene una medición ficticia de O3, publica un nuevo iBeacon y espera hasta la siguiente medición.

## Clase CodificadorMajor

```text
------------- CodificadorMajor -------------
|
|
tipo: N, contador: N --> construirMajor() --x
major: N <--
|
|
major: N --> obtenerTipo() --x
tipo: N <--
|
|
major: N --> obtenerContador() --x
contador: N <--
|
--------------------------------------------
```

Responsabilidad: encapsular la regla de codificación del campo `Major` sin depender de Arduino ni del hardware BLE.

## Clase Medidor

```text
---------------- Medidor ----------------
| valor_o3_simulado: N                  |
|                                       |
|                                       |
|               Medidor() -->           |
|                                       |
|                                       |
|        iniciarMedidor() -->           |
|                                       |
|                                       |
valor: N <-- medirO3() <--
|                                       |
-----------------------------------------
```

Responsabilidad: proporcionar la medición ficticia que se utilizará en la demostración. En esta versión devuelve siempre `1234`.

## Clase EmisoraBLE

```text
-------------------- EmisoraBLE ---------------------
| nombre_emisora: Text                              |
| fabricante_id: N                                  |
| tx_power: Z                                       |
|                                                    |
|                                                    |
nombre: Text, fabricante: N, potencia: Z --> EmisoraBLE() -->
|                                                    |
|                                                    |
|                         encenderEmisora() -->      |
|                                                    |
|                                                    |
anunciando: B <-- estaAnunciando() <--
|                                                    |
|                                                    |
|                         detenerAnuncio() -->       |
|                                                    |
|                                                    |
uuid: [N]_16, major: N, minor: N, rssi: Z --> emitirAnuncioIBeacon() -->
|                                                    |
------------------------------------------------------
```

Responsabilidad: encapsular únicamente la comunicación BLE necesaria para emitir el iBeacon.

## Clase Publicador

```text
------------------------ Publicador -------------------------
| beacon_uuid: [N]_16                                      |
| la_emisora: EmisoraBLE                                   |
| rssi: Z                                                  |
|                                                          |
|                                                          |
|                         Publicador() -->                  |
|                                                          |
|                                                          |
|                   encenderEmisora() -->                  |
|                                                          |
|                                                          |
tipo: MedicionesID, valor: N, contador: N --> publicarMedida() -->
major: N <--                                                |
|                                                          |
------------------------------------------------------------
```

Responsabilidad: transformar la medición lógica en los campos del iBeacon. `Publicador` no realiza mediciones y `EmisoraBLE` no decide cómo se codifican los datos.

## Flujo del componente

```text
loop()
  |
  +--> Medidor.medirO3()
  |        |
  |        +--> 1234
  |
  +--> incrementar contador
  |
  +--> Publicador.publicarMedida(O3, 1234, contador)
           |
           +--> CodificadorMajor.construirMajor(11, contador)
           |
           +--> EmisoraBLE.emitirAnuncioIBeacon(...)
```

# Aclaraciones del Diseño

- La placa objetivo es la SparkFun Pro nRF52840 Mini.
- La librería BLE utilizada es Adafruit Bluefruit nRF52.
- El nombre anunciado por la placa es `Elia_GTI`.
- El UUID del iBeacon es el mismo del recurso de referencia del profesor: `EPSG-GTI-PROY-3A`, representado mediante 16 bytes ASCII.
- El fabricante iBeacon utilizado es `0x004C`.
- `O3` utiliza el identificador `11`.
- `TEMPERATURA` queda reservado con el identificador `12`, aunque no se publica en este Sprint 0.
- El byte alto de `Major` contiene el tipo de medida y el byte bajo contiene el contador.
- El contador es de 8 bits y, después de `255`, vuelve naturalmente a `0`.
- La medición ficticia de O3 es `1234` y se publica en `Minor`.
- Se genera una nueva medición aproximadamente cada `5000 ms`.
- Entre dos mediciones, el controlador BLE anuncia repetidamente el mismo iBeacon. El cliente Android será responsable de ignorar anuncios repetidos con el mismo contador.
- No se realiza calibración ni lectura de un sensor físico en el Sprint 0.
- No se incluyen servicios GATT, características BLE ni callbacks de conexión porque no forman parte del flujo requerido.

## Criterios de aceptación del componente

- Al arrancar, la placa anuncia un iBeacon con nombre `Elia_GTI`.
- El UUID emitido corresponde a `EPSG-GTI-PROY-3A`.
- Para O3 y contador `5`, el `Major` debe ser `0x0B05` (`2821`).
- El `Minor` debe contener `1234`.
- El contador debe cambiar una sola vez por cada nueva medición.
- La codificación y decodificación de `Major` debe superar el test automático independiente del hardware.

# Reglas Generales

- **Lenguaje de programación:** C++ para Arduino.
- **Plataforma:** SparkFun Pro nRF52840 Mini con Adafruit Bluefruit nRF52.
- **Cabecera de cada fichero:** debe indicar nombre, descripción, copyright, fecha, autor y aportación.
- **Encabezados de funciones y métodos:** cada función o método debe incluir su diseño lógico dentro de un bloque delimitado por líneas discontinuas (`--------------------`) y una breve descripción.
- **Notación:** los diseños utilizan tipos abstractos `N`, `Z`, `R`, `B`, `Text` y las reglas de la especificación de diseño lógico v3.
- **Legibilidad:** cada clase debe mantener una única responsabilidad clara.
- **Separación de responsabilidades:** `Medidor` genera la medida, `CodificadorMajor` codifica el Major, `Publicador` decide qué datos enviar y `EmisoraBLE` realiza la comunicación BLE.
- **Pruebas automatizadas:** los tests del componente se encuentran separados del sketch Arduino y deben poder ejecutarse sin conectar la placa cuando prueben lógica independiente del hardware.