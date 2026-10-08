# Diseño del Componente

El componente **business_logic** gestiona el almacenamiento y la recuperación de mediciones.

Recibe y devuelve objetos lógicos relacionados con una medición y utiliza la base de datos para realizar la persistencia.

Este componente es completamente independiente de la capa de comunicación. No conoce rutas, peticiones HTTP, respuestas HTTP, códigos de estado ni formatos de transporte.

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

ConexionBD = recurso de acceso a la base de datos
```

## Clase LogicaNegocio

```text
                 --------------- LogicaNegocio ----------------
                 |
                 | conexion: ConexionBD
                 |
                 |
conexion: ConexionBD --> LogicaNegocio() -->
                 |
                 |
medicion: Medicion --> leerDatos() --> resultado: B
                 |
                 |
mostrarDatos() --> medicion: MedicionAlmacenada
                 |
                 ------------------------------------------------
```

### leerDatos()

```text
medicion: Medicion --> leerDatos() --> resultado: B
```

Recibe una medición formada por:

- `uuid`
- `fecha`
- `major`
- `minor`
- `TxPower`

y almacena sus datos en la tabla `Medicion`.

Devuelve un valor lógico que indica si el almacenamiento se ha realizado correctamente.

### mostrarDatos()

```text
mostrarDatos() --> medicion: MedicionAlmacenada
```

Obtiene la última medición almacenada en la tabla `Medicion`.

La medición devuelta contiene:

- `id`
- `uuid`
- `fecha`
- `major`
- `minor`
- `TxPower`

El campo `id` es generado automáticamente por la base de datos.

Si no existen mediciones almacenadas, el método puede no devolver ninguna medición.

## Relación con la base de datos

El componente utiliza la tabla `Medicion` definida en `database_design.md`.

```text
Medicion
|
+-- id
+-- uuid
+-- fecha
+-- major
+-- minor
+-- TxPower
```

El flujo lógico para almacenar una medición es:

```text
Medicion
   |
   v
leerDatos()
   |
   v
Tabla Medicion
```

El flujo lógico para recuperar la última medición es:

```text
Tabla Medicion
   |
   v
mostrarDatos()
   |
   v
MedicionAlmacenada
```

# Aclaraciones del Diseño

- El componente `business_logic` es completamente independiente del componente `communication`.
- El componente no conoce HTTP, rutas REST, códigos de estado, JSON ni otros mecanismos de transporte.
- La conexión con la base de datos se recibe desde el exterior.
- La persistencia se realiza utilizando la tabla `Medicion` definida en `database_design.md`.
- El campo `id` es generado automáticamente por la base de datos y no forma parte de la entrada de `leerDatos()`.
- El campo `id` sí forma parte de la medición recuperada mediante `mostrarDatos()`.
- La última medición almacenada se obtiene utilizando el valor de `id` más alto.
- Los campos utilizados por la lógica de negocio coinciden con los definidos en la tabla `Medicion`.

# Reglas Generales

- **Lenguaje de Programación:** PHP 8.x utilizando PDO para el acceso a MySQL/MariaDB.
- **Encabezados de Funciones/Métodos:** el encabezado de cada función o método deberá incluir su diseño lógico dentro de un bloque de comentarios delimitado por líneas discontinuas (`--------------------`).
- **Legibilidad del Código:** el código deberá ser claro y autoexplicativo, evitando comentarios adicionales innecesarios.
- **Separación de Responsabilidades:** este componente no deberá contener código relacionado con HTTP, REST, rutas, respuestas, códigos de estado, JSON ni otros mecanismos de transporte.
- **Base de Datos:** las operaciones de persistencia deberán utilizar la tabla y las columnas definidas en `database_design.md`.
- **Pruebas Automatizadas:** se deberán mantener pruebas automatizadas para los métodos clave `leerDatos()` y `mostrarDatos()`, comprobando el almacenamiento y la recuperación correcta de los campos de una medición.