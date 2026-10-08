# Diseño del Componente

El componente **mobile_frontend_business_logic** representa en el cliente Android la parte de la lógica de negocio utilizada por la aplicación móvil.

Su responsabilidad es actuar como una lógica fake/proxy del componente `business_logic` del backend.

La interfaz gráfica móvil no realiza directamente peticiones HTTP ni conoce las rutas REST. En su lugar, utiliza las operaciones proporcionadas por `mobile_frontend_business_logic`.

Durante el Sprint 0, el cliente móvil necesita únicamente la operación:

```text
leerDatos()
```

Esta operación mantiene el mismo nombre, entrada y salida lógica que su equivalente en `business_logic`.

## Tipos de datos

```text
Medicion = (
    uuid: Text,
    fecha: Text,
    major: Z,
    minor: Z,
    TxPower: Z
)

RespuestaREST = (
    codigo: N,
    json: Text
)

PeticionarioREST = componente encargado de realizar
peticiones HTTP al backend
```

El tipo `Medicion` coincide con el definido en `business_logic_design.md`.

## Correspondencia con business_logic

El backend define:

```text
medicion: Medicion --> leerDatos() --> resultado: B
```

El frontend móvil mantiene la misma operación lógica:

```text
medicion: Medicion --> leerDatos() --> resultado: B
```

Por tanto:

```text
BACKEND

medicion: Medicion
      |
      v
business_logic.leerDatos()
      |
      v
resultado: B
```

```text
MÓVIL

medicion: Medicion
      |
      v
mobile_frontend_business_logic.leerDatos()
      |
      v
resultado: B
```

La diferencia se encuentra únicamente en la implementación.

En el backend, `leerDatos()` almacena directamente la medición mediante la capa de persistencia.

En el móvil, `leerDatos()` actúa como proxy y envía la medición al backend mediante la ruta REST correspondiente.

## Flujo del componente

```text
mobile_gui
    |
    | Medicion
    v
LogicaFakeTelefono.leerDatos()
    |
    | convierte Medicion a JSON
    v
PeticionarioREST
    |
    | POST /medicion
    v
Backend
```

La interfaz gráfica móvil solo conoce:

```text
LogicaFakeTelefono.leerDatos()
```

Los detalles relacionados con HTTP, JSON y la ruta `/medicion` permanecen encapsulados dentro de `mobile_frontend_business_logic`.

## Clase LogicaFakeTelefono

```text
                 -------- LogicaFakeTelefono --------
                 |
                 | servidor: Text
                 | peticionario: PeticionarioREST
                 |
                 |
servidor: Text --> LogicaFakeTelefono() -->
                 |
                 |
medicion: Medicion --> leerDatos() --> resultado: B
                 |
                 |
medicion: Medicion --> crearJson() --> json: Text
                 |
                 -------------------------------------
```

### LogicaFakeTelefono()

```text
servidor: Text --> LogicaFakeTelefono() -->
```

Recibe la dirección del servidor REST que será utilizada para realizar las peticiones al backend.

También prepara el componente encargado de ejecutar las peticiones HTTP.

### leerDatos()

```text
medicion: Medicion --> leerDatos() --> resultado: B
```

Recibe una `Medicion` procedente del cliente móvil.

La operación mantiene la misma interfaz lógica que:

```text
business_logic.leerDatos()
```

Internamente realiza el siguiente proceso:

```text
Medicion
    |
    v
crearJson()
    |
    v
JSON
    |
    v
PeticionarioREST
    |
    | POST /medicion
    v
Backend
```

El componente móvil no accede directamente a la base de datos.

### crearJson()

```text
medicion: Medicion --> crearJson() --> json: Text
```

Convierte una `Medicion` a la representación JSON utilizada por el componente `communication`.

El JSON contiene:

```text
uuid
fecha
major
minor
TxPower
```

Ejemplo conceptual:

```json
{
    "uuid": "45505347-2D47-5449-2D50-524F592D3341",
    "fecha": "2026-10-08 16:00:00",
    "major": 2821,
    "minor": 1234,
    "TxPower": -53
}
```

## Clase PeticionarioREST

`PeticionarioREST` encapsula los mecanismos de comunicación HTTP utilizados por `LogicaFakeTelefono`.

Su responsabilidad es recibir los datos necesarios para realizar una petición y comunicarse con el componente `communication` del backend.

```text
                 -------- PeticionarioREST --------
                 |
                 | servidor: Text
                 |
                 |
servidor: Text --> PeticionarioREST() -->
                 |
                 |
metodo: Text,
ruta: Text,
cuerpo: Text
-->
realizarPeticion()
-->
respuesta: RespuestaREST
                 |
                 -----------------------------------
```

### PeticionarioREST()

```text
servidor: Text --> PeticionarioREST() -->
```

Recibe la dirección base del servidor.

### realizarPeticion()

```text
metodo: Text,
ruta: Text,
cuerpo: Text
-->
realizarPeticion()
-->
respuesta: RespuestaREST
```

Realiza una petición HTTP al backend y obtiene su código de respuesta y su cuerpo.

Para `leerDatos()` se utilizará:

```text
metodo = POST
ruta = /medicion
cuerpo = Medicion en JSON
```

`PeticionarioREST` no contiene lógica relacionada con las mediciones.

Únicamente realiza la comunicación solicitada por `LogicaFakeTelefono`.

# Aclaraciones del Diseño

- `mobile_frontend_business_logic` está separado completamente de `mobile_gui`.
- La interfaz gráfica móvil no realiza directamente peticiones HTTP.
- La interfaz gráfica móvil no utiliza directamente `PeticionarioREST`.
- La interfaz gráfica móvil únicamente utiliza las operaciones públicas de `LogicaFakeTelefono`.
- `LogicaFakeTelefono` actúa como proxy/fake del componente `business_logic`.
- La operación `leerDatos()` mantiene el mismo nombre, entrada y salida lógica que `business_logic.leerDatos()`.
- Durante el Sprint 0 el teléfono solo necesita utilizar `leerDatos()`.
- `mostrarDatos()` no se implementa en el cliente móvil porque la interfaz Android no necesita recuperar la última medición.
- `LogicaFakeTelefono` convierte la `Medicion` a JSON.
- `PeticionarioREST` encapsula la comunicación HTTP.
- La ruta utilizada para almacenar una medición es `POST /medicion`.
- El cliente móvil no contiene consultas SQL ni accede directamente a la base de datos.
- El campo lógico se denomina `TxPower`, igual que en `business_logic` y `database`.
- La ejecución concreta de las peticiones de red deberá respetar las restricciones de Android y no bloquear el hilo de la interfaz gráfica.

# Reglas Generales

- **Lenguaje de Programación:** Java para Android.
- **Correspondencia con Backend:** las operaciones públicas que representan funciones de `business_logic` deberán mantener el mismo nombre, entradas y salidas lógicas que sus equivalentes del backend.
- **Encabezados de Funciones/Métodos:** cada función o método propio deberá incluir su diseño lógico dentro de un bloque de comentarios delimitado por líneas discontinuas (`--------------------`).
- **Legibilidad del Código:** las responsabilidades de representación de mediciones, lógica fake y comunicación HTTP deberán permanecer claramente separadas.
- **Separación de Responsabilidades:** `mobile_gui` no podrá contener código HTTP, construir peticiones REST ni utilizar directamente `PeticionarioREST`.
- **Comunicación:** toda comunicación con el backend utilizada por la interfaz móvil deberá estar encapsulada dentro de `mobile_frontend_business_logic`.
- **Persistencia:** este componente no accederá directamente a MySQL ni contendrá consultas SQL.
- **Pruebas Automatizadas:** se deberán mantener pruebas para `LogicaFakeTelefono.leerDatos()`, la transformación de `Medicion` a JSON y la correcta construcción de la petición `POST /medicion`.