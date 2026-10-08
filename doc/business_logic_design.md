# Diseño del Componente

El componente **logica_negocio** gestiona el almacenamiento y la recuperación de mediciones. Recibe y devuelve objetos lógicos de tipo `Medicion`, delegando la persistencia en la base de datos. No recibe peticiones HTTP ni genera respuestas HTTP.

## Tipos de datos

```text
Medicion = (
    uuid: Text,
    fecha: Text,
    major: Z,
    minor: Z,
    tx_power: Z
)

ConexionBD = recurso de acceso a la base de datos
```

En el intercambio JSON, el campo lógico `tx_power` se representa con la clave `TxPower` para conservar el formato utilizado por el resto del proyecto.

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
medicion: Medicion --> leerDatos() -->
resultado: B      <--
                 |
                 |
medicion: Medicion <-- mostrarDatos() <--
                 |
                 ------------------------------------------------
```

`leerDatos()` recibe una `Medicion` y guarda `uuid`, `fecha`, `major`, `minor` y `TxPower` utilizando la base de datos.

`mostrarDatos()` obtiene la última medición almacenada y la devuelve al componente que la solicita. Si no existen mediciones, puede no devolver una `Medicion`.

## Flujo lógico

```text
Medicion
   |
   v
leerDatos()
   |
   v
Base de datos

Base de datos
   |
   v
mostrarDatos()
   |
   v
Medicion
```

# Aclaraciones del Diseño

- La lógica de negocio está separada del servidor REST.
- La conexión con la base de datos se recibe desde el exterior; la clase no crea rutas HTTP.
- La `Medicion` utilizada por los clientes contiene `uuid`, `fecha`, `major`, `minor` y `TxPower`.
- `id` pertenece a la persistencia de la base de datos y no forma parte del tipo lógico de entrada de una medición.
- La implementación actual recupera la última fila ordenando por `id` de forma descendente.
- `LogicaNegocioTests.php` comprueba la conversión desde JSON, el almacenamiento, la recuperación y la conservación de los campos principales.

# Reglas Generales

- **Lenguaje de programación:** PHP 8.x utilizando PDO para el acceso a MySQL/MariaDB.
- **Encabezados de funciones/métodos:** cada función o método deberá incluir su diseño lógico dentro de un bloque de comentarios delimitado por líneas discontinuas (`--------------------`).
- **Legibilidad del código:** la clase deberá mantener separadas las responsabilidades de negocio, persistencia y comunicación HTTP; no se añadirá lógica REST dentro de este componente.
- **Pruebas automatizadas:** se deberán mantener pruebas unitarias o de integración para `leerDatos()` y `mostrarDatos()`, incluyendo almacenamiento y recuperación de los campos de `Medicion`.
