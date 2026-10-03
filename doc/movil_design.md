# Diseño del Componente

El componente **movil** representa el cliente Android del Sprint 0. En el repositorio actual contiene dos responsabilidades principales: detectar y descomponer tramas iBeacon mediante BLE, y representar `leerDatos()` desde el cliente mediante una lógica fake que envía una `Medicion` a `POST /medicion`.

## Tipos de datos

```text
Medicion = (
    uuid: Text,
    fecha: Text,
    major: Z,
    minor: Z,
    tx_power: Z
)

TramaIBeacon = (
    prefijo: [ N ]_9,
    uuid: [ N ]_16,
    major: [ N ]_2,
    minor: [ N ]_2,
    tx_power: Z,
    adv_flags: [ N ]_3,
    adv_header: [ N ]_2,
    company_id: [ N ]_2,
    ibeacon_type: N,
    ibeacon_length: N
)

ResultadoBLE = resultado de una detección Bluetooth Low Energy
```

En el JSON enviado al servidor, el campo lógico `tx_power` se representa mediante la clave `TxPower`.

## Lector BLE

El flujo del lector BLE es:

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
TramaIBeacon
          |
          v
UUID / Major / Minor / TxPower
```

### Clase TramaIBeacon

```text
                 --------------- TramaIBeacon ----------------
                 |
                 | prefijo: [ N ]_9
                 | uuid: [ N ]_16
                 | major: [ N ]_2
                 | minor: [ N ]_2
                 | tx_power: Z
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
tx_power: Z      <-- getTxPower() <--
                 |
                 -----------------------------------------------
```

La clase separa de los bytes recibidos el prefijo iBeacon, UUID, Major, Minor y TxPower.

### Clase MainActivity (flujo BLE)

```text
                 --------------- MainActivity ----------------
                 |
                 | escaner_ble: EscanerBLE
                 |
                 | buscarTodosLosDispositivosBTLE() -->
                 |
resultado: ResultadoBLE --> mostrarInformacionDispositivoBTLE() -->
                 |
dispositivo: Text --> buscarEsteDispositivoBTLE() -->
                 |
                 | detenerBusquedaDispositivosBTLE() -->
                 |
                 | inicializarBlueTooth() -->
                 |
                 ------------------------------------------------
```

Los manejadores de botones llaman a las operaciones de iniciar búsqueda general, buscar el dispositivo configurado y detener el escaneo. `onCreate()` inicializa Bluetooth y solicita los permisos necesarios.

### Utilidades de conversión

La clase `Utilidades` proporciona operaciones estáticas para convertir entre texto, UUID, arrays de bytes y números:

```text
texto: Text --> stringToBytes() --x
bytes: [ N ] <--

uuid_texto: Text --> stringToUUID() --x

bytes: [ N ] --> bytesToString() --x
texto: Text <--

bytes: [ N ] --> bytesToInt() --x
valor: Z <--

bytes: [ N ] --> bytesToLong() --x
valor: Z <--

bytes: [ N ] --> bytesToHexString() --x
texto: Text <--
```

## Lógica fake del teléfono

```text
                 --------- LogicaFakeTelefono ---------
                 |
                 | servidor: Text
                 |
                 | medicion: Medicion --> crearJson() --> json: Text
                 |
                 | metodo: Text, ruta: Text, cuerpo: Text
                 | --> enviarPeticion() --> resultado: B
                 |
                 |
servidor: Text --> LogicaFakeTelefono() -->
                 |
                 |
medicion: Medicion --> leerDatos() -->
resultado: B      <--
                 |
                 ---------------------------------------
```

`leerDatos()` mantiene la intención de la operación de la lógica de negocio, pero desde el teléfono la implementa enviando la `Medicion` en JSON mediante `POST /medicion`.

## Cliente REST auxiliar heredado

El repositorio conserva `PeticionarioREST` como cliente REST genérico procedente del material base. Su responsabilidad es ejecutar una petición HTTP en segundo plano y devolver código y cuerpo mediante una respuesta asíncrona. No forma parte del flujo activo de `LogicaFakeTelefono`.

# Aclaraciones del Diseño

- El código BLE y la lógica fake del teléfono se encuentran actualmente en subcarpetas separadas dentro de `src/movil/`.
- La versión actual de `MainActivity` detecta y muestra por log los datos iBeacon, pero todavía no invoca `LogicaFakeTelefono.leerDatos()` desde el callback BLE; ambas piezas están presentes pero no integradas en un único flujo de aplicación dentro del repositorio.
- La búsqueda del dispositivo concreto en `MainActivity` utiliza actualmente el nombre `GTI3A-2025`.
- `AndroidManifest.xml` declara permisos BLE, ubicación e Internet, y permite tráfico HTTP sin cifrar para las pruebas locales del Sprint 0.
- `PeticionarioREST/` contiene código auxiliar heredado/de referencia y no es utilizado por `LogicaFakeTelefono`.
- `LogicaFakeTelefonoTest.java` comprueba que `leerDatos()` construye una petición `POST /medicion` con los campos esperados.

# Reglas Generales

- **Lenguaje de programación:** Java para Android; XML para manifest y layout; JUnit 4 para las pruebas Java.
- **Encabezados de funciones/métodos:** cada función o método propio del componente deberá incluir su diseño lógico dentro de un bloque de comentarios delimitado por líneas discontinuas (`--------------------`).
- **Legibilidad del código:** la lectura BLE, la representación de la medición y la comunicación REST deberán permanecer separadas en responsabilidades claras.
- **Pruebas automatizadas:** se deberán mantener pruebas para `LogicaFakeTelefono.leerDatos()` y añadir pruebas para las operaciones críticas de parseo BLE cuando puedan aislarse del framework/hardware.
