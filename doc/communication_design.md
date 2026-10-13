# Diseño del Componente

El componente **communication** actúa como intermediario entre los clientes externos y el componente `business_logic`.

Su responsabilidad es recibir peticiones HTTP, interpretar los datos recibidos, transformarlos al tipo lógico `Medicion`, invocar las operaciones de `business_logic` y construir las respuestas HTTP.

Este componente conoce REST, HTTP y JSON.

El componente `communication` no accede directamente a la base de datos.

## Tipos de datos

```text
Medicion = (
    valor: Z,
    fecha_hora: Text,
    identificador_sensor: Text,
    tipo: Z
)

RespuestaREST = (
    codigo: N,
    json: Text
)

LogicaNegocio = componente business_logic
```

El tipo `Medicion` coincide con el definido en `business_logic_design.md`.

## Rutas REST

Durante el Sprint 0 existen dos rutas funcionales:

```text
POST /medicion
GET /medicion
```

## POST /medicion

Recibe una medición codificada en JSON.

```text
JSON
 |
 v
communication
 |
 | convierte JSON a Medicion
 v
business_logic.leerDatos(medicion)
 |
 v
RespuestaREST
```

La petición debe contener:

```text
valor
fecha_hora
identificador_sensor
tipo
```

Ejemplo:

```json
{
    "valor": 1234,
    "fecha_hora": "2026-10-13 17:00:00",
    "identificador_sensor": "45505347-2D47-5449-2D50-524F592D3341",
    "tipo": 11
}
```

Resultados posibles:

```text
201 -> Medicion almacenada correctamente
400 -> JSON o Medicion incorrecta
500 -> No se pudo almacenar la Medicion
```

El almacenamiento se delega completamente en:

```text
business_logic.leerDatos(medicion)
```

## GET /medicion

Solicita la última medición almacenada.

```text
communication
 |
 v
business_logic.mostrarDatos()
 |
 v
Medicion
 |
 | serialización
 v
JSON
```

Resultados posibles:

```text
200 -> Existe una Medicion almacenada
404 -> No existen mediciones almacenadas
```

La recuperación se delega completamente en:

```text
business_logic.mostrarDatos()
```

## Clase ServidorREST

```text
                 --------------- ServidorREST ----------------
                 |
                 | logica: LogicaNegocio
                 |
                 |
json: Text       --> postMedicion() --> respuesta: RespuestaREST
                 |
                 |
                 | getMedicion() --> respuesta: RespuestaREST
                 |
                 |
codigo: N,
json: Text       --> crearRespuesta() --> respuesta: RespuestaREST
                 |
                 |
logica: LogicaNegocio --> ServidorREST() -->
                 |
                 |
metodo: Text,
ruta: Text,
cuerpo: Text
-->
manejarPeticion()
-->
respuesta: RespuestaREST
                 |
                 ------------------------------------------------
```

### ServidorREST()

```text
logica: LogicaNegocio --> ServidorREST() -->
```

Recibe una referencia al componente `business_logic`.

### manejarPeticion()

```text
metodo: Text,
ruta: Text,
cuerpo: Text
-->
manejarPeticion()
-->
respuesta: RespuestaREST
```

Determina qué operación REST debe ejecutarse.

Las rutas reconocidas son:

```text
POST /medicion
GET /medicion
```

Una combinación de método y ruta no reconocida devuelve `404`.

### postMedicion()

```text
json: Text --> postMedicion() --> respuesta: RespuestaREST
```

Convierte el JSON recibido en una `Medicion`.

Comprueba que existan los campos:

```text
valor
fecha_hora
identificador_sensor
tipo
```

Si la medición es válida, llama a:

```text
business_logic.leerDatos(medicion)
```

No realiza operaciones directas sobre la base de datos.

### getMedicion()

```text
getMedicion() --> respuesta: RespuestaREST
```

Solicita la última medición mediante:

```text
business_logic.mostrarDatos()
```

Si existe una medición, la convierte a JSON.

Si no existe ninguna medición, devuelve `404`.

### crearRespuesta()

```text
codigo: N,
json: Text
-->
crearRespuesta()
-->
respuesta: RespuestaREST
```

Construye una respuesta formada por:

```text
codigo
json
```

## Punto de entrada HTTP

El archivo `index.php` actúa como punto de entrada HTTP.

```text
Peticion HTTP
     |
     v
index.php
     |
     v
ServidorREST.manejarPeticion()
     |
     v
RespuestaREST
     |
     v
Cliente
```

`index.php` construye las dependencias necesarias:

```text
ConexionBD
    |
    v
LogicaNegocio
    |
    v
ServidorREST
```

Las operaciones sobre la tabla `MEDICIONES` continúan siendo responsabilidad de `business_logic`.

# Aclaraciones del Diseño

- El componente se denomina `communication`.
- Solo existen las rutas `POST /medicion` y `GET /medicion`.
- `communication` conoce HTTP, REST y JSON.
- `business_logic` no conoce HTTP, REST ni JSON.
- Una `Medicion` contiene `valor`, `fecha_hora`, `identificador_sensor` y `tipo`.
- `POST /medicion` convierte el JSON recibido en una `Medicion`.
- `GET /medicion` devuelve la última `Medicion` almacenada.
- `communication` no contiene sentencias SQL.
- `communication` no necesita conocer UUID, Major, Minor ni TxPower como conceptos de iBeacon.
- El origen de `valor`, `tipo` e `identificador_sensor` ya ha sido procesado antes de llegar al backend.
- `ServidorRESTTest.php` utiliza una lógica de negocio falsa para probar el componente sin depender de MySQL.

# Reglas Generales

- **Lenguaje de Programación:** PHP 8.x.
- **Encabezados de Funciones/Métodos:** cada función o método deberá incluir su diseño lógico dentro de un bloque de comentarios delimitado por líneas discontinuas (`--------------------`).
- **Legibilidad del Código:** el servidor REST deberá limitarse a recibir, validar y transformar peticiones, delegar en `business_logic` y construir respuestas.
- **Separación de Responsabilidades:** `communication` no contendrá lógica de persistencia.
- **Persistencia:** este componente no deberá contener consultas SQL.
- **Dependencias:** `communication` puede depender de `business_logic`, pero `business_logic` nunca deberá depender de `communication`.
- **Pruebas Automatizadas:** se deberán mantener pruebas para `POST /medicion`, `GET /medicion`, JSON incorrecto, mediciones incompletas, rutas incorrectas y delegación en `leerDatos()` y `mostrarDatos()`.