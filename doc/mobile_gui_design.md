# Diseño del Componente

El componente **mobile_gui** representa la parte de la aplicación Android encargada de interactuar con el usuario, gestionar el escaneo Bluetooth Low Energy y obtener las mediciones enviadas mediante iBeacon.

Su responsabilidad es:

- inicializar Bluetooth y solicitar los permisos necesarios;
- iniciar y detener el escaneo BLE;
- detectar el dispositivo `Elia_GTI`;
- interpretar la trama iBeacon recibida;
- obtener UUID, Major, Minor y TxPower;
- construir una `Medicion`;
- entregar la medición al componente `mobile_frontend_business_logic`.

El componente `mobile_gui` no realiza directamente peticiones HTTP, no conoce rutas REST y no accede a la base de datos.

La comunicación con el backend se delega completamente en:

```text
LogicaFakeTelefono.leerDatos()
```

perteneciente al componente `mobile_frontend_business_logic`.

## Tipos de datos

```text
Medicion = (
    uuid: Text,
    fecha: Text,
    major: Z,
    minor: Z,
    TxPower: Z
)

TramaIBeacon = (
    prefijo: [ N ]_9,
    uuid: [ N ]_16,
    major: [ N ]_2,
    minor: [ N ]_2,
    TxPower: Z,
    adv_flags: [ N ]_3,
    adv_header: [ N ]_2,
    company_id: [ N ]_2,
    ibeacon_type: N,
    ibeacon_length: N
)

ResultadoBLE = resultado de una detección Bluetooth Low Energy

LogicaFakeTelefono = componente mobile_frontend_business_logic
```

El tipo `Medicion` coincide con el utilizado por `mobile_frontend_business_logic` y `business_logic`.

## Flujo general del cliente móvil

```text
Usuario
   |
   v
MainActivity
   |
   | iniciar escaneo BLE
   v
Elia_GTI
   |
   | iBeacon
   v
ResultadoBLE
   |
   v
TramaIBeacon
   |
   v
UUID / Major / Minor / TxPower
   |
   v
Medicion
   |
   v
LogicaFakeTelefono.leerDatos()
```

A partir de la llamada a:

```text
LogicaFakeTelefono.leerDatos()
```

la responsabilidad deja de pertenecer a `mobile_gui`.

`mobile_gui` no conoce cómo se envía la medición al backend.

## Lector BLE

El flujo de lectura BLE es:

```text
Inicializar Bluetooth y permisos
          |
          v
Iniciar escaneo BLE
          |
          v
ResultadoBLE
          |
          v
Comprobar dispositivo
          |
          v
Elia_GTI
          |
          v
TramaIBeacon
          |
          v
UUID / Major / Minor / TxPower
```

El escaneo puede utilizarse para:

```text
Buscar todos los dispositivos BLE
```

o para:

```text
Buscar Elia_GTI
```

La búsqueda puede detenerse mediante la operación correspondiente de `MainActivity`.

## Clase TramaIBeacon

```text
                 --------------- TramaIBeacon ----------------
                 |
                 | prefijo: [ N ]_9
                 | uuid: [ N ]_16
                 | major: [ N ]_2
                 | minor: [ N ]_2
                 | TxPower: Z
                 | adv_flags: [ N ]_3
                 | adv_header: [ N ]_2
                 | company_id: [ N ]_2
                 | ibeacon_type: N
                 | ibeacon_length: N
                 |
                 |
bytes: [ N ] --> TramaIBeacon() -->
                 |
                 |
prefijo: [ N ]_9 <-- getPrefijo() <--
uuid: [ N ]_16   <-- getUUID() <--
major: [ N ]_2   <-- getMajor() <--
minor: [ N ]_2   <-- getMinor() <--
TxPower: Z       <-- getTxPower() <--
                 |
                 -----------------------------------------------
```

### TramaIBeacon()

```text
bytes: [ N ] --> TramaIBeacon() -->
```

Recibe los bytes de una trama BLE que contiene información iBeacon.

Separa los diferentes campos de la trama para permitir su consulta posterior.

### getPrefijo()

```text
getPrefijo() --> prefijo: [ N ]_9
```

Devuelve el prefijo de la trama iBeacon.

### getUUID()

```text
getUUID() --> uuid: [ N ]_16
```

Devuelve los 16 bytes correspondientes al UUID.

### getMajor()

```text
getMajor() --> major: [ N ]_2
```

Devuelve los dos bytes correspondientes al campo Major.

### getMinor()

```text
getMinor() --> minor: [ N ]_2
```

Devuelve los dos bytes correspondientes al campo Minor.

### getTxPower()

```text
getTxPower() --> TxPower: Z
```

Devuelve el valor TxPower incluido en la trama iBeacon.

## Clase MainActivity

`MainActivity` coordina la interacción con el usuario y el escaneo BLE.

```text
                 --------------- MainActivity ----------------
                 |
                 | escaner_ble: EscanerBLE
                 | logica: LogicaFakeTelefono
                 |
                 |
                 | inicializarBlueTooth() -->
                 |
                 |
                 | buscarTodosLosDispositivosBTLE() -->
                 |
                 |
dispositivo: Text --> buscarEsteDispositivoBTLE() -->
                 |
                 |
                 | detenerBusquedaDispositivosBTLE() -->
                 |
                 |
resultado: ResultadoBLE
-->
mostrarInformacionDispositivoBTLE()
-->
                 |
                 |
trama: TramaIBeacon
-->
crearMedicion()
-->
medicion: Medicion
                 |
                 |
medicion: Medicion
-->
enviarMedicion()
-->
resultado: B
                 |
                 ------------------------------------------------
```

### inicializarBlueTooth()

```text
inicializarBlueTooth() -->
```

Inicializa los elementos necesarios para utilizar Bluetooth Low Energy.

También permite comprobar que Bluetooth está disponible antes de iniciar el escaneo.

### buscarTodosLosDispositivosBTLE()

```text
buscarTodosLosDispositivosBTLE() -->
```

Inicia un escaneo BLE para detectar los dispositivos disponibles.

Los resultados obtenidos se procesan mediante el callback de escaneo.

### buscarEsteDispositivoBTLE()

```text
dispositivo: Text --> buscarEsteDispositivoBTLE() -->
```

Inicia la búsqueda de un dispositivo BLE concreto.

Durante el Sprint 0 el dispositivo buscado es:

```text
Elia_GTI
```

### detenerBusquedaDispositivosBTLE()

```text
detenerBusquedaDispositivosBTLE() -->
```

Detiene el escaneo BLE activo.

### mostrarInformacionDispositivoBTLE()

```text
resultado: ResultadoBLE
-->
mostrarInformacionDispositivoBTLE()
-->
```

Recibe un resultado de escaneo BLE.

Cuando el resultado corresponde a un iBeacon válido, obtiene los bytes anunciados y construye una `TramaIBeacon`.

A partir de la trama se pueden recuperar:

```text
UUID
Major
Minor
TxPower
```

### crearMedicion()

```text
trama: TramaIBeacon
-->
crearMedicion()
-->
medicion: Medicion
```

Construye una `Medicion` a partir de los datos obtenidos de la trama iBeacon.

La medición contiene:

```text
uuid
fecha
major
minor
TxPower
```

La fecha corresponde al momento en el que el cliente móvil procesa la medición.

### enviarMedicion()

```text
medicion: Medicion
-->
enviarMedicion()
-->
resultado: B
```

Entrega la medición al componente `mobile_frontend_business_logic` mediante:

```text
LogicaFakeTelefono.leerDatos(medicion)
```

`enviarMedicion()` no contiene código HTTP y no conoce la ruta REST utilizada por el backend.

## Utilidades de conversión

La clase `Utilidades` proporciona operaciones auxiliares para convertir entre texto, UUID, arrays de bytes y valores numéricos.

```text
texto: Text
-->
stringToBytes()
-->
bytes: [ N ]


uuid_texto: Text
-->
stringToUUID()
-->
uuid


bytes: [ N ]
-->
bytesToString()
-->
texto: Text


bytes: [ N ]
-->
bytesToInt()
-->
valor: Z


bytes: [ N ]
-->
bytesToLong()
-->
valor: Z


bytes: [ N ]
-->
bytesToHexString()
-->
texto: Text
```

Estas operaciones no realizan comunicaciones HTTP ni acceden a la base de datos.

## Relación con mobile_frontend_business_logic

`mobile_gui` depende de `mobile_frontend_business_logic`.

```text
mobile_gui
     |
     | Medicion
     v
mobile_frontend_business_logic
```

La única operación de lógica de negocio utilizada por la interfaz móvil durante el Sprint 0 es:

```text
medicion: Medicion --> leerDatos() --> resultado: B
```

Por tanto, `MainActivity` puede realizar:

```text
logica.leerDatos(medicion)
```

pero no puede realizar directamente:

```text
POST /medicion
```

ni utilizar directamente:

```text
PeticionarioREST
```

# Aclaraciones del Diseño

- El componente se denomina `mobile_gui`.
- El código correspondiente se encuentra separado de `mobile_frontend_business_logic`.
- `MainActivity` gestiona la interacción con el usuario y el escaneo BLE.
- El dispositivo utilizado durante el Sprint 0 es `Elia_GTI`.
- `TramaIBeacon` interpreta los bytes de la trama iBeacon.
- `Utilidades` contiene operaciones auxiliares de conversión.
- El cliente móvil obtiene UUID, Major, Minor y TxPower de la trama recibida.
- A partir de esos valores se construye una `Medicion`.
- `mobile_gui` no realiza peticiones HTTP directamente.
- `mobile_gui` no construye JSON para enviarlo al backend.
- `mobile_gui` no conoce la ruta `POST /medicion`.
- `mobile_gui` no utiliza directamente `PeticionarioREST`.
- La comunicación con el backend se delega a `LogicaFakeTelefono.leerDatos()`.
- `AndroidManifest.xml` contiene los permisos necesarios para utilizar Bluetooth Low Energy, ubicación cuando sea necesaria e Internet.
- La interfaz gráfica visual se implementa mediante recursos XML de Android.
- Los tres controles principales permiten iniciar la búsqueda BLE, detenerla y buscar específicamente `Elia_GTI`.

# Reglas Generales

- **Lenguaje de Programación:** Java para Android y XML para manifest y layouts.
- **Encabezados de Funciones/Métodos:** cada función o método propio deberá incluir su diseño lógico dentro de un bloque de comentarios delimitado por líneas discontinuas (`--------------------`).
- **Legibilidad del Código:** el código relacionado con interfaz, lectura BLE, interpretación de tramas y envío de mediciones deberá mantener responsabilidades claramente diferenciadas.
- **Separación de Responsabilidades:** `mobile_gui` no contendrá implementación HTTP, rutas REST ni acceso directo a la base de datos.
- **Dependencias:** `mobile_gui` podrá utilizar `mobile_frontend_business_logic`, pero no deberá utilizar directamente los componentes internos encargados de HTTP.
- **Comunicación:** la única vía para enviar una medición al backend será mediante `LogicaFakeTelefono.leerDatos()`.
- **Pruebas Automatizadas:** se deberán añadir pruebas para las operaciones de interpretación de tramas iBeacon que puedan ejecutarse sin depender del hardware Bluetooth.