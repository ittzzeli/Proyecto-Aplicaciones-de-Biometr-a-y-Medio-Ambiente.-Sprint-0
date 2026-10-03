# Diseño del Componente

El componente **database** almacena las mediciones recibidas por el sistema. En el Sprint 0 utiliza una única tabla y no contiene lógica de negocio ni comunicación HTTP.

```text
====================================================================================
TABLE: Medicion

DESCRIPTION: Almacena las mediciones recibidas por el sistema.

COLUMNS:

+ id | INT | NOT NULL | Auto-Increment
+ uuid | VARCHAR(36) | NOT NULL
+ fecha | DATETIME | NOT NULL
+ major | INT | NOT NULL
+ minor | INT | NOT NULL
+ TxPower | INT | NOT NULL

PRIMARY KEY: id

FOREIGN KEYS:

+ Ninguna

CONSTRAINTS:

+ id se genera automáticamente mediante AUTO_INCREMENT.
+ Cada fila representa una única medición.
====================================================================================
```

La base de datos ofrece las operaciones necesarias para que la lógica de negocio pueda insertar una medición y recuperar las mediciones almacenadas. El acceso concreto a la tabla se realiza desde el componente `logica_negocio`.

# Aclaraciones del Diseño

- Solo existe la tabla `Medicion`.
- `id` es una clave técnica interna de la base de datos y se genera automáticamente; no forma parte de la medición enviada por los clientes.
- `uuid` se almacena como `VARCHAR(36)`.
- `fecha` se almacena como `DATETIME`.
- `major`, `minor` y `TxPower` se almacenan como `INT`.
- No existen claves foráneas ni relaciones con otras tablas en el Sprint 0.
- `database_test.sql` contiene inserciones y consultas sencillas para comprobar el autoincremento y la recuperación de datos.

# Reglas Generales

- **Lenguaje de programación:** SQL para MySQL/MariaDB.
- **Encabezados de funciones/métodos:** no aplica a las sentencias SQL de definición de tabla; cualquier función o procedimiento que se añadiera posteriormente deberá incluir su diseño lógico dentro de un bloque de comentarios delimitado por líneas discontinuas (`--------------------`).
- **Legibilidad del código:** las sentencias SQL deberán ser sencillas, explícitas y utilizar los mismos nombres de tabla y columnas definidos en este diseño.
- **Pruebas automatizadas:** se deberán mantener pruebas SQL de inserción y consulta que comprueben que las mediciones se almacenan, que `id` se genera automáticamente y que los datos pueden recuperarse.
