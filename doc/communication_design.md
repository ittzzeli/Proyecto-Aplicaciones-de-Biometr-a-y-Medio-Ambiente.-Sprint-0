# Diseño del Componente

El componente **communication** actúa como intermediario entre los clientes externos y el componente `business_logic`.

Su responsabilidad es recibir peticiones HTTP, interpretar los datos recibidos, transformarlos a los tipos utilizados por la lógica de negocio, invocar los métodos correspondientes y construir las respuestas HTTP.

Este componente conoce los mecanismos de comunicación REST y JSON.

El componente `communication` no accede directamente a la base de datos.

## Tipos de datos

```text
Medicion = (
    uuid: Text,
    fecha: Text,
    major: Z,
    minor: Z,
    TxPower: Z
)

MedicionAlmacenada = (
    id: N,
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

LogicaNegocio = componente business_logic
```

`Medicion` y `MedicionAlmacenada` coinciden con los tipos definidos en `business_logic_design.md`.

## Rutas REST

El Sprint 0 dispone de dos rutas funcionales:

```text
POST /medicion
GET /medicion
```

### POST /medicion

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

La petición recibida debe contener:

```text
uuid
fecha
major
minor
TxPower
```

Resultados posibles:

```text
201 -> Medicion almacenada correctamente
400 -> JSON o Medicion incorrecta
500 -> No se pudo almacenar la Medicion
```

El componente `communication` valida la representación recibida y crea una `Medicion`.

El almacenamiento de la medición se delega completamente en:

```text
business_logic.leerDatos(medicion)
```

### GET /medicion

Solicita la última medición almacenada.

```text
communication
 |
 v
business_logic.mostrarDatos()
 |
 v
MedicionAlmacenada
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

La recuperación de datos se delega completamente en:

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
cuerpo: Text     --> manejarPeticion() -->
respuesta: RespuestaREST <--
                 |
                 ------------------------------------------------
```

### ServidorREST()

```text
logica: LogicaNegocio --> ServidorREST() -->
```

Recibe una referencia al componente `business_logic`.

El servidor utilizará esta referencia para delegar las operaciones de almacenamiento y recuperación de mediciones.

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

Recibe el método HTTP, la ruta solicitada y el cuerpo de la petición.

Determina qué operación REST debe ejecutarse.

Las rutas reconocidas son:

```text
POST /medicion
GET /medicion
```

Si la combinación de método y ruta no existe, devuelve una respuesta con código `404`.

### postMedicion()

```text
json: Text --> postMedicion() --> respuesta: RespuestaREST
```

Convierte el JSON recibido en una `Medicion`.

Comprueba que existan los campos:

```text
uuid
fecha
major
minor
TxPower
```

Si la medición es válida, llama a:

```text
business_logic.leerDatos(medicion)
```

El método no realiza ninguna operación directa sobre la base de datos.

### getMedicion()

```text
getMedicion() --> respuesta: RespuestaREST
```

Solicita al componente `business_logic` la última medición mediante:

```text
business_logic.mostrarDatos()
```

Si existe una medición, la serializa a JSON.

Si no existe ninguna medición, devuelve una respuesta `404`.

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

Esta respuesta será posteriormente utilizada por el punto de entrada HTTP para enviar el código de estado y el cuerpo JSON al cliente.

## Punto de entrada HTTP

El archivo `index.php` actúa como punto de entrada de la comunicación HTTP.

Su flujo es:

```text
Peticion HTTP
     |
     v
index.php
     |
     | obtiene metodo, ruta y cuerpo
     v
ServidorREST.manejarPeticion()
     |
     v
RespuestaREST
     |
     | codigo HTTP + JSON
     v
Cliente
```

`index.php` también crea las dependencias necesarias para construir el sistema:

```text
ConexionBD
    |
    v
LogicaNegocio
    |
    v
ServidorREST
```

La conexión con la base de datos se utiliza únicamente para construir `LogicaNegocio`.

Las operaciones de persistencia continúan siendo responsabilidad de `business_logic`.

# Aclaraciones del Diseño

- El componente se denomina `communication`.
- Solo existen las rutas funcionales `POST /medicion` y `GET /medicion` durante el Sprint 0.
- `communication` conoce HTTP, REST y JSON.
- `business_logic` no conoce HTTP, REST ni JSON.
- `POST /medicion` convierte el JSON recibido en una `Medicion` y delega el almacenamiento en `business_logic.leerDatos()`.
- `GET /medicion` delega la recuperación en `business_logic.mostrarDatos()` y serializa el resultado a JSON.
- `communication` no contiene sentencias `INSERT`, `SELECT` ni otras consultas SQL.
- La conexión con la base de datos creada en `index.php` se utiliza únicamente para construir el objeto `LogicaNegocio`.
- El acceso efectivo a la tabla `Medicion` permanece dentro del componente `business_logic`.
- Una ruta o combinación de método/ruta no reconocida devuelve una respuesta `404`.
- `ServidorRESTTest.php` utiliza una implementación falsa de la lógica de negocio para probar `communication` sin depender de una base de datos real.

# Reglas Generales

- **Lenguaje de Programación:** PHP 8.x.
- **Encabezados de Funciones/Métodos:** cada función o método deberá incluir su diseño lógico dentro de un bloque de comentarios delimitado por líneas discontinuas (`--------------------`).
- **Legibilidad del Código:** el código deberá ser claro y autoexplicativo, evitando comentarios adicionales innecesarios.
- **Separación de Responsabilidades:** `communication` se limitará a recibir, validar y transformar peticiones, delegar operaciones en `business_logic` y construir respuestas.
- **Persistencia:** este componente no deberá contener consultas SQL ni acceder directamente a la tabla `Medicion`.
- **Dependencias:** `communication` puede depender de `business_logic`, pero `business_logic` nunca deberá depender de `communication`.
- **Pruebas Automatizadas:** se deberán mantener pruebas automatizadas para `POST /medicion`, `GET /medicion`, rutas incorrectas, JSON incorrecto y delegación en `leerDatos()` y `mostrarDatos()`.