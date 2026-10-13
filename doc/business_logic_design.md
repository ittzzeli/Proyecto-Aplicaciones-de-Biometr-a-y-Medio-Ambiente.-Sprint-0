# Diseño del Componente

El componente **business_logic** gestiona el almacenamiento y la recuperación de mediciones.

Recibe y devuelve objetos lógicos de tipo `Medicion` y utiliza el componente `database` para realizar la persistencia.

Este componente es completamente independiente de la capa de comunicación.

No conoce rutas REST, peticiones HTTP, respuestas HTTP, códigos de estado, JSON ni otros mecanismos de transporte.

## Tipos de datos

```text
Medicion = (
    valor: Z,
    fecha_hora: Text,
    identificador_sensor: Text,
    tipo: Z
)

ConexionBD = recurso de acceso a la base de datos
```

Los campos de `Medicion` se corresponden directamente con las columnas de la tabla `MEDICIONES` definida en `database_design.md`.

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
                 | mostrarDatos() --> medicion: Medicion
                 |
                 ------------------------------------------------
```

## LogicaNegocio()

```text
conexion: ConexionBD --> LogicaNegocio() -->
```

Recibe una conexión con la base de datos.

La conexión se utiliza para realizar las operaciones de persistencia definidas para la tabla `MEDICIONES`.

## leerDatos()

```text
medicion: Medicion --> leerDatos() --> resultado: B
```

Recibe una medición formada por:

```text
valor
fecha_hora
identificador_sensor
tipo
```

y almacena sus datos en la tabla:

```text
MEDICIONES
```

La correspondencia es directa:

```text
Medicion.valor
        ↓
MEDICIONES.valor

Medicion.fecha_hora
        ↓
MEDICIONES.fecha_hora

Medicion.identificador_sensor
        ↓
MEDICIONES.identificador_sensor

Medicion.tipo
        ↓
MEDICIONES.tipo
```

Devuelve:

```text
true
```

si la operación se realiza correctamente.

## mostrarDatos()

```text
mostrarDatos() --> medicion: Medicion
```

Recupera la última medición almacenada en la tabla `MEDICIONES`.

La última medición se determina utilizando:

```text
fecha_hora
```

de forma descendente.

Devuelve una `Medicion` formada por:

```text
valor
fecha_hora
identificador_sensor
tipo
```

Si no existe ninguna medición almacenada, puede devolver ausencia de medición.

## Relación con database

El componente `business_logic` utiliza la tabla:

```text
MEDICIONES
```

definida en `database_design.md`.

Su estructura es:

```text
MEDICIONES
├── valor
├── fecha_hora
├── identificador_sensor
└── tipo
```

con clave primaria compuesta:

```text
(fecha_hora, identificador_sensor)
```

Para almacenar una medición se utiliza:

```text
INSERT
```

Para recuperar la última medición se utiliza:

```text
SELECT
ORDER BY fecha_hora DESC
LIMIT 1
```

El flujo es:

```text
leerDatos()
    |
    v
INSERT INTO MEDICIONES
```

y:

```text
mostrarDatos()
    |
    v
SELECT FROM MEDICIONES
    |
    v
Medicion
```

# Aclaraciones del Diseño

- `business_logic` es completamente independiente de `communication`.
- Este componente no conoce HTTP, REST, JSON, rutas ni códigos de estado.
- El modelo lógico utilizado es `Medicion`.
- Una `Medicion` contiene únicamente `valor`, `fecha_hora`, `identificador_sensor` y `tipo`.
- `valor` procede originalmente del campo Minor de la trama iBeacon, pero `business_logic` no necesita conocer ese origen.
- `identificador_sensor` procede originalmente del UUID, pero `business_logic` no procesa tramas iBeacon.
- `tipo` procede originalmente del Major, pero `business_logic` recibe directamente el tipo ya extraído.
- El contador incluido en el Major no pertenece a la lógica de negocio y no se almacena.
- TxPower no forma parte de `Medicion`.
- No existe un campo `id`.
- Los campos utilizados por `business_logic` coinciden directamente con las columnas definidas en `database_design.md`.
- La conexión con la base de datos se recibe externamente mediante el constructor.

# Reglas Generales

- **Lenguaje de Programación:** PHP 8.x utilizando PDO para MySQL/MariaDB.
- **Encabezados de Funciones/Métodos:** cada función o método deberá incluir su diseño lógico dentro de un bloque de comentarios delimitado por líneas discontinuas (`--------------------`).
- **Legibilidad del Código:** el código deberá ser claro y autoexplicativo.
- **Separación de Responsabilidades:** `business_logic` no contendrá código HTTP, REST, JSON ni otros mecanismos de transporte.
- **Persistencia:** las operaciones de persistencia deberán utilizar exclusivamente la estructura definida en `database_design.md`.
- **Dependencias:** `business_logic` no podrá depender de `communication`.
- **Pruebas Automatizadas:** se deberán mantener pruebas para `leerDatos()` y `mostrarDatos()`.