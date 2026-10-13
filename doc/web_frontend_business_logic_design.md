# Diseño del Componente

El componente **web_frontend_business_logic** representa en el cliente web la parte de la lógica de negocio utilizada por la interfaz gráfica.

Su responsabilidad es actuar como una lógica fake/proxy del componente `business_logic` del backend.

La interfaz gráfica web no realiza directamente peticiones HTTP ni conoce las rutas REST. En su lugar, utiliza las operaciones proporcionadas por `web_frontend_business_logic`.

Durante el Sprint 0, el cliente web necesita únicamente la operación:

```text
mostrarDatos()
```

Esta operación mantiene el mismo nombre, entrada y salida lógica que su equivalente en `business_logic`.

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

RespuestaHTTP = respuesta obtenida del backend

FuncionHTTP = operación utilizada para realizar
peticiones HTTP
```

El tipo `MedicionAlmacenada` coincide con el definido en `business_logic_design.md`.

## Correspondencia con business_logic

El backend define:

```text
mostrarDatos() --> medicion: MedicionAlmacenada
```

El frontend web mantiene la misma operación lógica:

```text
mostrarDatos() --> medicion: MedicionAlmacenada
```

Por tanto:

```text
BACKEND

business_logic.mostrarDatos()
          |
          v
MedicionAlmacenada
```

```text
WEB

LogicaFakeNavegador.mostrarDatos()
          |
          v
MedicionAlmacenada
```

La diferencia se encuentra únicamente en la implementación.

En el backend, `mostrarDatos()` recupera la última medición mediante la capa de persistencia.

En el cliente web, `mostrarDatos()` actúa como proxy y obtiene la medición mediante una petición al backend.

## Flujo del componente

```text
web_gui
    |
    | mostrarDatos()
    v
LogicaFakeNavegador
    |
    | GET /medicion
    v
communication
    |
    v
business_logic
```

La interfaz gráfica web solo conoce:

```text
LogicaFakeNavegador.mostrarDatos()
```

Los detalles relacionados con HTTP, `fetch()` y la ruta `/medicion` permanecen encapsulados dentro de `web_frontend_business_logic`.

## Clase LogicaFakeNavegador

```text
                 ------ LogicaFakeNavegador ------
                 |
                 | servidor: Text
                 | fetch_impl: FuncionHTTP
                 |
                 |
servidor: Text,
fetch_impl: FuncionHTTP
-->
LogicaFakeNavegador()
-->
                 |
                 |
                 | mostrarDatos()
                 | --> medicion: MedicionAlmacenada
                 |
                 ----------------------------------
```

### LogicaFakeNavegador()

```text
servidor: Text,
fetch_impl: FuncionHTTP
-->
LogicaFakeNavegador()
-->
```

Recibe:

- la dirección base del servidor;
- la función utilizada para realizar peticiones HTTP.

La función HTTP puede sustituirse durante las pruebas automatizadas para evitar depender de un servidor real.

### mostrarDatos()

```text
mostrarDatos() --> medicion: MedicionAlmacenada
```

Mantiene la misma interfaz lógica que:

```text
business_logic.mostrarDatos()
```

Internamente realiza:

```text
GET /medicion
```

El flujo es:

```text
mostrarDatos()
      |
      v
fetch()
      |
      | GET /medicion
      v
Backend
      |
      v
JSON
      |
      v
MedicionAlmacenada
```

Si la respuesta HTTP es correcta, devuelve la medición recibida.

Si la respuesta HTTP indica un error, la operación genera un error para que la interfaz gráfica pueda informar al usuario.

## Relación con web_gui

`web_gui` depende de `web_frontend_business_logic`.

```text
web_gui
   |
   | mostrarDatos()
   v
web_frontend_business_logic
```

La interfaz gráfica puede realizar:

```text
logica.mostrarDatos()
```

pero no puede utilizar directamente:

```text
fetch()
```

ni conocer directamente:

```text
GET /medicion
```

La comunicación queda encapsulada dentro de `LogicaFakeNavegador`.

# Aclaraciones del Diseño

- El componente se denomina `web_frontend_business_logic`.
- `LogicaFakeNavegador` actúa como proxy/fake de `business_logic`.
- Durante el Sprint 0 el cliente web utiliza únicamente `mostrarDatos()`.
- `mostrarDatos()` mantiene el mismo nombre, entrada y salida lógica que `business_logic.mostrarDatos()`.
- El resultado de `mostrarDatos()` es una `MedicionAlmacenada`.
- La comunicación con el backend se realiza mediante `GET /medicion`.
- `fetch()` se encuentra exclusivamente dentro de `web_frontend_business_logic`.
- `web_gui` no realiza directamente peticiones HTTP.
- El parámetro `fetch_impl` permite sustituir la comunicación real durante las pruebas automatizadas.
- Este componente no contiene consultas SQL ni accede directamente a la base de datos.
- `LogicaFakeNavegadorTest.js` comprueba la ruta utilizada, el método HTTP, los datos recibidos y la gestión de respuestas incorrectas.

# Reglas Generales

- **Lenguaje de Programación:** JavaScript.
- **Correspondencia con Backend:** las operaciones públicas que representan funciones de `business_logic` deberán mantener el mismo nombre, entradas y salidas lógicas que sus equivalentes del backend.
- **Encabezados de Funciones/Métodos:** cada función o método propio deberá incluir su diseño lógico dentro de un bloque de comentarios delimitado por líneas discontinuas (`--------------------`).
- **Legibilidad del Código:** el código deberá ser claro y mantener separadas la interfaz gráfica y la comunicación HTTP.
- **Separación de Responsabilidades:** `web_gui` no podrá contener `fetch()`, rutas REST ni otros mecanismos directos de comunicación HTTP.
- **Comunicación:** toda comunicación con el backend utilizada por la interfaz web deberá estar encapsulada dentro de `web_frontend_business_logic`.
- **Persistencia:** este componente no accederá directamente a la base de datos.
- **Pruebas Automatizadas:** se deberán mantener pruebas para `mostrarDatos()`, la ruta `GET /medicion`, los datos recuperados y la gestión de errores HTTP.