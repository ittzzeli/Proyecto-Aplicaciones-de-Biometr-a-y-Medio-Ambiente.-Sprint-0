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
# Diseño del Componente

El componente **web_gui** representa la interfaz gráfica web del Sprint 0.

Su responsabilidad es mostrar al usuario la última medición disponible.

La interfaz obtiene los datos exclusivamente mediante el componente `web_frontend_business_logic`.

`web_gui` no realiza directamente peticiones HTTP, no utiliza `fetch()` y no conoce las rutas REST del backend.

## Tipos de datos

```text
MedicionAlmacenada = (
    id: N,
    uuid: Text,
    fecha: Text,
    major: Z,
    minor: Z,
    TxPower: Z
)

ElementosGUI = (
    fecha,
    major,
    minor,
    txPower,
    error
)

LogicaFakeNavegador = componente web_frontend_business_logic
```

La interfaz recibe una `MedicionAlmacenada`, aunque durante el Sprint 0 únicamente muestra:

```text
fecha
major
minor
TxPower
```

Los campos:

```text
id
uuid
```

pueden estar presentes en la medición, pero no forman parte de la representación visual de esta pantalla.

## Flujo general

```text
Usuario
   |
   v
web_gui
   |
   | mostrarDatos()
   v
web_frontend_business_logic
   |
   v
MedicionAlmacenada
   |
   v
mostrarUltimaMedicion()
   |
   v
Pantalla
```

La GUI solo utiliza:

```text
LogicaFakeNavegador.mostrarDatos()
```

A partir de esa operación obtiene una `MedicionAlmacenada` y actualiza los elementos visibles de la página.

## Interfaz gráfica

La pantalla contiene un único bloque titulado:

```text
Última medición
```

con la siguiente información:

```text
+---------+-----------------------+
| Fecha   | valor de fecha        |
| Major   | valor de major        |
| Minor   | valor de minor        |
| TxPower | valor de TxPower      |
+---------+-----------------------+
```

También dispone de un elemento destinado a mostrar un mensaje de error cuando no es posible obtener la medición.

## Función mostrarUltimaMedicion()

```text
logica: LogicaFakeNavegador,
elementos: ElementosGUI
-->
mostrarUltimaMedicion()
-->
```

Solicita la última medición mediante:

```text
logica.mostrarDatos()
```

Si la operación finaliza correctamente:

```text
MedicionAlmacenada
        |
        v
fecha    --> elemento fecha
major    --> elemento major
minor    --> elemento minor
TxPower  --> elemento txPower
```

y oculta el mensaje de error.

Si ocurre un error:

```text
mostrarDatos()
      |
      X
      |
      v
mensaje de error visible
```

La función no realiza directamente ninguna petición HTTP.

## Inicialización de la interfaz

Cuando el documento HTML termina de cargarse:

```text
DOMContentLoaded
      |
      v
Crear LogicaFakeNavegador
      |
      v
Obtener elementos HTML
      |
      v
mostrarUltimaMedicion()
```

La instancia de `LogicaFakeNavegador` pertenece conceptualmente al componente `web_frontend_business_logic`.

La GUI únicamente utiliza su operación pública:

```text
mostrarDatos()
```

## Archivos del componente

El componente `web_gui` está formado por:

```text
src/web_gui/
├── index.html
├── styles.css
├── app.js
└── WebTest.js
```

### index.html

Define la estructura visual de la página y los elementos donde se muestran:

```text
Fecha
Major
Minor
TxPower
Error
```

### styles.css

Define exclusivamente la presentación visual de la página.

No contiene lógica de negocio ni mecanismos de comunicación.

### app.js

Contiene la lógica de presentación de la interfaz.

Su operación principal es:

```text
logica: LogicaFakeNavegador,
elementos: ElementosGUI
-->
mostrarUltimaMedicion()
-->
```

`app.js` no utiliza directamente `fetch()` ni conoce rutas REST.

### WebTest.js

Contiene las pruebas automatizadas de la interfaz gráfica.

Utiliza una lógica fake controlada para comprobar la representación de los datos sin depender de un backend real.

# Aclaraciones del Diseño

- El componente se denomina `web_gui`.
- `web_gui` está separado de `web_frontend_business_logic`.
- La GUI no contiene `fetch()`.
- La GUI no conoce la ruta `GET /medicion`.
- La comunicación con el backend queda encapsulada en `LogicaFakeNavegador`.
- La interfaz recibe una `MedicionAlmacenada`.
- La pantalla no muestra `id` ni `uuid`.
- La pantalla muestra `fecha`, `major`, `minor` y `TxPower`.
- Si `mostrarDatos()` falla, se muestra un mensaje de error.
- `WebTest.js` comprueba la visualización correcta de los datos y el comportamiento ante errores.
- La apariencia visual se define mediante `styles.css`.

# Reglas Generales

- **Lenguaje de Programación:** HTML5 para la estructura, CSS para la presentación y JavaScript para la lógica de interfaz y las pruebas.
- **Encabezados de Funciones/Métodos:** cada función o método propio deberá incluir su diseño lógico dentro de un bloque de comentarios delimitado por líneas discontinuas (`--------------------`).
- **Legibilidad del Código:** la lógica de presentación deberá ser clara y mantenerse separada de la comunicación con el backend.
- **Separación de Responsabilidades:** `web_gui` no podrá contener `fetch()`, rutas REST ni mecanismos directos de comunicación HTTP.
- **Dependencias:** `web_gui` podrá utilizar `web_frontend_business_logic`, pero no deberá acceder directamente a los mecanismos internos de comunicación.
- **Presentación:** este componente será responsable únicamente de mostrar los datos y mensajes correspondientes al usuario.
- **Pruebas Automatizadas:** se deberán mantener pruebas para la representación de la medición y el comportamiento de la interfaz ante errores.