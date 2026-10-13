# Diseño del Componente

El componente **database** almacena las mediciones recibidas por el sistema.

Durante el Sprint 0 utiliza una única tabla denominada `MEDICIONES`.

La información almacenada corresponde a los datos útiles obtenidos de la trama iBeacon:

- `valor` se obtiene del campo Minor.
- `fecha_hora` corresponde al momento de la medición.
- `identificador_sensor` se obtiene del UUID.
- `tipo` se obtiene de la parte correspondiente al tipo dentro del Major.

El contador contenido en el Major se utiliza para controlar mediciones repetidas, pero no se almacena en la base de datos.

TxPower tampoco se almacena.

```text
====================================================================================
TABLE: MEDICIONES

DESCRIPTION: Almacena las mediciones recibidas por los sensores.

COLUMNS:

+ valor | INT | NOT NULL
+ fecha_hora | DATETIME | NOT NULL
+ identificador_sensor | VARCHAR(36) | NOT NULL
+ tipo | INT | NOT NULL

PRIMARY KEY: fecha_hora, identificador_sensor

FOREIGN KEYS:

+ Ninguna

CONSTRAINTS:

+ valor no puede ser NULL.
+ fecha_hora no puede ser NULL.
+ identificador_sensor no puede ser NULL.
+ tipo no puede ser NULL.
+ La combinación de fecha_hora e identificador_sensor identifica de forma única una medición.
====================================================================================
```

## Tipo Medicion

```text
Medicion = (
    valor: Z,
    fecha_hora: Text,
    identificador_sensor: Text,
    tipo: Z
)
```

## Relación con business_logic

El acceso a la tabla `MEDICIONES` se realiza desde el componente `business_logic`.

```text
business_logic
     |
     v
MEDICIONES
```

Para almacenar una medición se utiliza:

```text
INSERT
```

Para recuperar la última medición almacenada se utiliza:

```text
SELECT
```

El componente `database` no conoce HTTP, REST, JSON ni mecanismos de comunicación.

# Aclaraciones del Diseño

- Solo existe la tabla `MEDICIONES`.
- No existe un campo `id`.
- La clave primaria es compuesta por `fecha_hora` e `identificador_sensor`.
- `identificador_sensor` corresponde al UUID recibido mediante iBeacon.
- `valor` corresponde al valor obtenido del Minor.
- `tipo` corresponde únicamente al tipo extraído del Major.
- El contador incluido en el Major no se almacena.
- Major y Minor completos no se almacenan.
- TxPower no se almacena.
- No existen claves foráneas durante el Sprint 0.
- El componente `business_logic` realiza las operaciones de acceso a la tabla.

# Reglas Generales

- **Lenguaje de Programación:** SQL para MySQL/MariaDB.
- **Formato del Diseño:** las tablas deberán documentarse mediante `TABLE`, `DESCRIPTION`, `COLUMNS`, `PRIMARY KEY`, `FOREIGN KEYS` y `CONSTRAINTS`.
- **Encabezados de Funciones/Métodos:** si posteriormente se añadieran funciones o procedimientos SQL, deberán incluir su diseño lógico dentro de comentarios delimitados por líneas discontinuas (`--------------------`).
- **Legibilidad del Código:** los nombres de tabla y columnas utilizados en SQL deberán coincidir exactamente con los definidos en este diseño.
- **Separación de Responsabilidades:** la base de datos no contendrá mecanismos HTTP, REST ni lógica de comunicación.
- **Pruebas Automatizadas:** se deberán mantener pruebas de inserción y recuperación de mediciones.