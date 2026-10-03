# Diseño del Componente

El componente **web** muestra en el navegador la última medición disponible. La interfaz obtiene los datos exclusivamente mediante `LogicaFakeNavegador`; no accede directamente al servidor REST ni a la base de datos desde la capa de presentación.

## Tipo de datos

```text
Medicion = (
    uuid: Text,
    fecha: Text,
    major: Z,
    minor: Z,
    tx_power: Z
)
```

En JSON, `tx_power` se representa con la clave `TxPower`.

## Lógica fake del navegador

```text
                 -------- LogicaFakeNavegador --------
                 |
                 | servidor: Text
                 |
                 |
servidor: Text --> LogicaFakeNavegador() -->
                 |
                 |
medicion: Medicion <-- mostrarDatos() <--
                 |
                 ---------------------------------------
```

`mostrarDatos()` representa la operación homónima de la lógica de negocio desde el cliente. Para obtener la medición realiza `GET /medicion` y devuelve el JSON convertido en una `Medicion`.

## Interfaz gráfica

La pantalla contiene un único bloque titulado **“Última medición”** con los siguientes valores:

```text
Última medición
+---------+-----------------------+
| Fecha   | valor de fecha        |
| Major   | valor de major        |
| Minor   | valor de minor        |
| TxPower | valor de TxPower      |
+---------+-----------------------+
```

La función que actualiza la interfaz se diseña como:

```text
logica: LogicaFakeNavegador,
interfaz: InterfazMedicion --> mostrarUltimaMedicion() -->
```

La interfaz se inicializa al cargar el documento y solicita la última medición a la lógica fake.

# Aclaraciones del Diseño

- La interfaz no muestra `id` ni `uuid`; esos datos pueden existir en la medición pero no forman parte del bloque visual solicitado.
- `app.js` no realiza llamadas `fetch()` directas al REST; utiliza `LogicaFakeNavegador.mostrarDatos()`.
- `LogicaFakeNavegador` utiliza `GET /medicion` para representar `mostrarDatos()` desde el cliente.
- Si no se puede obtener una medición, la interfaz muestra un mensaje de error.
- `WebTest.js` prueba la actualización de `Fecha`, `Major`, `Minor` y `TxPower` mediante una lógica fake controlada.
- `LogicaFakeNavegadorTest.js` comprueba que la lógica fake utiliza `GET /medicion` y conserva los datos recibidos.

# Reglas Generales

- **Lenguaje de programación:** HTML5 para la estructura, CSS para la presentación y JavaScript para la lógica del navegador y las pruebas.
- **Encabezados de funciones/métodos:** cada función o método JavaScript deberá incluir su diseño lógico dentro de un bloque de comentarios delimitado por líneas discontinuas (`--------------------`).
- **Legibilidad del código:** la vista deberá permanecer separada de la comunicación REST; la obtención de datos se realizará a través de `LogicaFakeNavegador`.
- **Pruebas automatizadas:** se deberán mantener pruebas para la lógica fake del navegador y para la representación de los datos en la interfaz.
