# Diseño del Componente

El componente **database** almacena las mediciones recibidas por el sistema.

Durante el Sprint 0 utiliza una única tabla denominada `Medicion`.

Este componente no contiene lógica de negocio ni mecanismos de comunicación HTTP.

```text
====================================================================================
TABLE: Medicion

DESCRIPTION: Almacena las mediciones recibidas por el sistema.

COLUMNS:

+ id | INT | NOT NULL | AUTO_INCREMENT
+ uuid | VARCHAR(36) | NOT NULL
+ fecha | DATETIME | NOT NULL
+ major | INT | NOT NULL
+ minor | INT | NOT NULL
+ TxPower | INT | NOT NULL

PRIMARY KEY:

+ id

FOREIGN KEYS:

+ Ninguna

CONSTRAINTS:

+ id se genera automáticamente mediante AUTO_INCREMENT.
+ uuid no puede ser NULL.
+ fecha no puede ser NULL.
+ major no puede ser NULL.
+ minor no puede ser NULL.
+ TxPower no puede ser NULL.
+ Cada fila representa una única medición almacenada.
====================================================================================
```

## Relación con business_logic

El acceso a la tabla `Medicion` se realiza desde el componente `business_logic`.

El componente `database` no conoce HTTP, REST ni JSON.

El flujo de almacenamiento es:

```text
business_logic
     |
     v
Tabla Medicion
```

Las operaciones utilizadas por `business_logic` son:

```text
INSERT
```

para almacenar una nueva medición y:

```text
SELECT
```

para recuperar la última medición almacenada.

# Aclaraciones del Diseño

- Solo existe la tabla `Medicion`.
- `id` es una clave técnica interna de la base de datos.
- `id` se genera automáticamente y no forma parte de la medición enviada por los clientes.
- `uuid` se almacena como `VARCHAR(36)`.
- `fecha` se almacena como `DATETIME`.
- `major`, `minor` y `TxPower` se almacenan como `INT`.
- No existen claves foráneas durante el Sprint 0.
- No existen relaciones con otras tablas.
- El componente `business_logic` es responsable de realizar las operaciones de acceso a esta tabla.
- `database_test.sql` contiene pruebas sencillas de inserción y recuperación de mediciones.

# Reglas Generales

- **Lenguaje de Programación:** SQL para MySQL/MariaDB.
- **Formato del Diseño:** las tablas deberán documentarse mediante `TABLE`, `DESCRIPTION`, `COLUMNS`, `PRIMARY KEY`, `FOREIGN KEYS` y `CONSTRAINTS`.
- **Encabezados de Funciones/Métodos:** no aplica a las sentencias SQL de definición de tabla. Si posteriormente se añadieran funciones o procedimientos, deberán incluir su diseño lógico dentro de comentarios delimitados por líneas discontinuas (`--------------------`).
- **Legibilidad del Código:** las sentencias SQL deberán utilizar los mismos nombres de tabla y columnas definidos en este diseño.
- **Separación de Responsabilidades:** la base de datos no contendrá mecanismos HTTP, REST ni lógica de comunicación.
- **Pruebas Automatizadas:** se deberán mantener pruebas SQL de inserción y consulta que permitan comprobar que las mediciones se almacenan, que `id` se genera automáticamente y que los datos pueden recuperarse.