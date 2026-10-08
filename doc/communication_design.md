# Diseño del Componente

El componente **rest** actúa como intermediario entre los clientes y `logica_negocio`. Expone únicamente las rutas `POST /medicion` y `GET /medicion`, trabaja con JSON y no accede directamente a la tabla de la base de datos.

## Tipos de datos

```text
Medicion = (
    uuid: Text,
    fecha: Text,
    major: Z,
    minor: Z,
    tx_power: Z
)

RespuestaREST = (
    codigo: N,
    json: Text
)
```

En JSON, `tx_power` se representa mediante la clave `TxPower`.

## Rutas REST

```text
POST /medicion

Medicion en JSON
      |
      v
Servidor REST
      |
      v
LogicaNegocio.leerDatos(medicion)
      |
      v
Respuesta JSON
```

- Si la medición es válida y se almacena: `201`.
- Si el JSON o la medición son incorrectos: `400`.
- Si la lógica de negocio no puede almacenar la medición: `500`.

```text
GET /medicion

Servidor REST
      |
      v
LogicaNegocio.mostrarDatos()
      |
      v
Medicion en JSON
```

- Si existe una medición: `200`.
- Si no existen mediciones: `404`.

## Clase ServidorREST

```text
                 --------------- ServidorREST ----------------
                 |
                 | logica: LogicaNegocio
                 |
                 | json: Text --> postMedicion() --> respuesta: RespuestaREST
                 |
                 | getMedicion() --> respuesta: RespuestaREST
                 |
                 | codigo: N, datos: Object
                 | --> crearRespuesta() --> respuesta: RespuestaREST
                 |
                 |
logica: LogicaNegocio --> ServidorREST() -->
                 |
                 |
metodo: Text,
ruta: Text,
cuerpo: Text      --> manejarPeticion() -->
respuesta: RespuestaREST <--
                 |
                 ------------------------------------------------
```

El punto de entrada HTTP obtiene método, ruta y cuerpo de la petición, llama a `manejarPeticion()` y envía el código HTTP y el JSON devueltos por `ServidorREST`.

# Aclaraciones del Diseño

- Solo existen las rutas `POST /medicion` y `GET /medicion` como rutas funcionales del Sprint 0.
- `POST /medicion` convierte el JSON recibido en una `Medicion` y llama a `leerDatos()`.
- `GET /medicion` llama a `mostrarDatos()` y serializa el resultado a JSON.
- El componente REST no contiene sentencias `INSERT`, `SELECT` ni otras consultas SQL.
- La conexión a la base de datos se crea en el punto de entrada únicamente para construir `LogicaNegocio`; el acceso a los datos sigue delegado en dicha clase.
- `ServidorRESTTest.php` utiliza una lógica de negocio falsa para comprobar las rutas sin depender de MySQL.

# Reglas Generales

- **Lenguaje de programación:** PHP 8.x.
- **Encabezados de funciones/métodos:** cada función o método deberá incluir su diseño lógico dentro de un bloque de comentarios delimitado por líneas discontinuas (`--------------------`).
- **Legibilidad del código:** el servidor REST deberá limitarse a validar/transformar la petición, delegar en la lógica de negocio y construir la respuesta; no contendrá lógica de persistencia.
- **Pruebas automatizadas:** se deberán mantener pruebas para `POST /medicion` correcto e incorrecto, `GET /medicion`, uso de JSON y delegación en `leerDatos()`/`mostrarDatos()`.
