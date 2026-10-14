# Diseño del Componente

El componente **web_frontend_business_logic** representa en el navegador la parte de la lógica de negocio utilizada por la interfaz web.

Actúa como una lógica fake/proxy del componente `business_logic` del backend.

La interfaz gráfica web no realiza directamente peticiones HTTP ni conoce las rutas REST.

Durante el Sprint 0 utiliza únicamente la operación:

```text
mostrarDatos()
```

Esta operación mantiene el mismo nombre y salida lógica que su equivalente en `business_logic`.

## Tipos de datos

```text
Medicion = (
    valor: Z,
    fecha_hora: Text,
    identificador_sensor: Text,
    tipo: Z
)
```

El tipo `Medicion` coincide con el definido en:

```text
business_logic_design.md
database_design.md
```

## Correspondencia con business_logic

El backend define:

```text
mostrarDatos() --> medicion: Medicion
```

El cliente web mantiene:

```text
mostrarDatos() --> medicion: Medicion
```

La diferencia se encuentra únicamente en la implementación.

En el backend:

```text
mostrarDatos()
    |
    v
Base de datos
```

En el navegador:

```text
mostrarDatos()
    |
    v
GET /medicion
    |
    v
Backend
```

## Flujo del componente

```text
web_gui
    |
    v
LogicaFakeNavegador.mostrarDatos()
    |
    v
GET /medicion
    |
    v
ServidorREST
    |
    v
Medicion
    |
    v
web_gui
```

## Clase LogicaFakeNavegador

```text
              -------- LogicaFakeNavegador --------
              |
              | servidor: Text
              |
              |
servidor: Text
--> LogicaFakeNavegador() -->
              |
              |
              | mostrarDatos()
              | --> medicion: Medicion
              |
              --------------------------------------
```

## LogicaFakeNavegador()

```text
servidor: Text --> LogicaFakeNavegador() -->
```

Recibe la dirección base del servidor.

Esta dirección se utiliza para realizar las peticiones necesarias al backend.

Durante las pruebas automatizadas también puede utilizarse una implementación falsa de `fetch`.

## mostrarDatos()

```text
mostrarDatos() --> medicion: Medicion
```

Mantiene la misma operación lógica que:

```text
business_logic.mostrarDatos()
```

Internamente realiza:

```text
GET /medicion
```

Si la petición se realiza correctamente, devuelve una `Medicion` formada por:

```text
valor
fecha_hora
identificador_sensor
tipo
```

Ejemplo:

```json
{
    "valor": 1234,
    "fecha_hora": "2026-10-14 09:06:41",
    "identificador_sensor": "45505347-2D47-5449-2D50-524F592D3341",
    "tipo": 11
}
```

Si el servidor devuelve un error, la operación informa del fallo para que la interfaz pueda mostrarlo.

# Aclaraciones del Diseño

- `web_frontend_business_logic` está separado de `web_gui`.
- `LogicaFakeNavegador` actúa como proxy de `business_logic`.
- `mostrarDatos()` mantiene la misma salida lógica que `business_logic.mostrarDatos()`.
- Una `Medicion` contiene únicamente `valor`, `fecha_hora`, `identificador_sensor` y `tipo`.
- No existe un campo `id`.
- No se reciben Major, Minor ni TxPower.
- `valor` corresponde al valor que originalmente fue obtenido del Minor.
- `tipo` corresponde al tipo que originalmente fue extraído del Major.
- `identificador_sensor` corresponde al UUID del sensor.
- Este componente conoce la comunicación HTTP necesaria para acceder al backend.
- La interfaz gráfica web no realiza directamente peticiones HTTP.
- La ruta utilizada para recuperar la última medición es `GET /medicion`.
- Este componente no accede directamente a la base de datos.

# Reglas Generales

- **Lenguaje de Programación:** JavaScript.
- **Correspondencia con Backend:** las operaciones que representan funciones de `business_logic` deberán mantener el mismo nombre y salida lógica.
- **Encabezados de Funciones/Métodos:** cada función o método deberá incluir su diseño lógico dentro de comentarios delimitados por líneas discontinuas (`--------------------`).
- **Legibilidad del Código:** el código deberá ser sencillo y mantener separadas la interfaz gráfica y la comunicación.
- **Separación de Responsabilidades:** `web_gui` no realizará peticiones HTTP directamente.
- **Comunicación:** las peticiones al backend se encapsularán en `web_frontend_business_logic`.
- **Persistencia:** este componente no accede directamente a la base de datos.
- **Pruebas Automatizadas:** se deberán mantener pruebas para `mostrarDatos()`, la ruta utilizada y la gestión de respuestas incorrectas.