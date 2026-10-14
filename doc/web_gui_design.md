# Diseño del Componente

El componente **web_gui** representa la interfaz gráfica web del Sprint 0.

Su función es mostrar al usuario la última medición almacenada en el sistema.

La interfaz gráfica no accede directamente a la base de datos ni realiza peticiones HTTP.

Para obtener la información utiliza:

```text
LogicaFakeNavegador.mostrarDatos()
```

del componente:

```text
web_frontend_business_logic
```

## Datos mostrados

La página muestra la última `Medicion` almacenada.

```text
Medicion = (
    valor: Z,
    fecha_hora: Text,
    identificador_sensor: Text,
    tipo: Z
)
```

Los campos mostrados son:

```text
valor
fecha_hora
identificador_sensor
tipo
```

Ejemplo:

```text
Última medición

Valor:
1234

Fecha y hora:
2026-10-14 09:36:35

Identificador del sensor:
45505347-2D47-5449-2D50-524F592D3341

Tipo:
11

[ Actualizar ]
```

## Flujo de la interfaz

Al abrir la página:

```text
Usuario abre la página
        |
        v
web_gui
        |
        v
LogicaFakeNavegador.mostrarDatos()
        |
        v
Medicion
        |
        v
Mostrar datos en pantalla
```

Cuando el usuario pulsa el botón `Actualizar`:

```text
Usuario pulsa Actualizar
        |
        v
web_gui
        |
        v
LogicaFakeNavegador.mostrarDatos()
        |
        v
Medicion más reciente
        |
        v
Actualizar datos mostrados
```

La interfaz web no conoce la ruta REST utilizada internamente.

## Página principal

La página contiene:

```text
+--------------------------------------------------+
|                 Última medición                  |
+--------------------------------------------------+
|                                                  |
| Valor:                    1234                   |
|                                                  |
| Fecha y hora:             ...                    |
|                                                  |
| Identificador del sensor: ...                    |
|                                                  |
| Tipo:                     11                     |
|                                                  |
+--------------------------------------------------+

[ Actualizar ]
```

El diseño se mantiene sencillo, ya que el objetivo principal del Sprint 0 es comprobar el funcionamiento completo del sistema.

## app.js

`app.js` coordina la interfaz gráfica.

Su responsabilidad es:

```text
- obtener los elementos de la interfaz;
- crear LogicaFakeNavegador;
- solicitar la última medición al abrir la página;
- mostrar los datos recibidos;
- actualizar la información cuando el usuario pulsa Actualizar;
- mostrar un mensaje si ocurre un error.
```

`app.js` no realiza peticiones HTTP directamente.

## mostrarUltimaMedicion()

```text
logica: LogicaFakeNavegador,
elementos: ElementosGUI
-->
mostrarUltimaMedicion()
-->
```

Solicita:

```text
logica.mostrarDatos()
```

y muestra:

```text
medicion.valor
medicion.fecha_hora
medicion.identificador_sensor
medicion.tipo
```

Si ocurre un error durante la recuperación de los datos, muestra un mensaje en la interfaz.

La función no realiza `fetch()` ni conoce la ruta `/medicion`.

## iniciarInterfaz()

```text
iniciarInterfaz() -->
```

Inicializa la interfaz gráfica.

Obtiene los elementos HTML necesarios, crea una instancia de:

```text
LogicaFakeNavegador
```

y solicita una primera medición mediante:

```text
mostrarUltimaMedicion()
```

También configura el botón:

```text
Actualizar
```

Cuando el usuario pulsa el botón:

```text
Actualizar
    |
    v
mostrarUltimaMedicion()
```

Mientras se realiza la actualización, el botón puede quedar temporalmente desactivado para evitar varias solicitudes simultáneas.

## Botón Actualizar

El botón `Actualizar` permite solicitar manualmente la última medición disponible.

```text
pulsar Actualizar
        |
        v
mostrarUltimaMedicion()
        |
        v
LogicaFakeNavegador.mostrarDatos()
        |
        v
nueva Medicion
        |
        v
actualizar interfaz
```

La actualización es manual.

No se realizan consultas periódicas automáticas.

## Relación con web_frontend_business_logic

```text
web_gui
   |
   | mostrarDatos()
   v
LogicaFakeNavegador
   |
   v
Backend
```

La separación de responsabilidades debe mantenerse:

```text
web_gui
    NO conoce HTTP
    NO conoce rutas REST
    NO accede a la BBDD

web_frontend_business_logic
    encapsula la comunicación
```

# Aclaraciones del Diseño

- La página muestra únicamente la última medición almacenada.
- La interfaz es sencilla.
- Se muestran `valor`, `fecha_hora`, `identificador_sensor` y `tipo`.
- No existe un campo `id`.
- No se muestran Major, Minor ni TxPower.
- `valor` contiene el valor que originalmente procede del Minor.
- `tipo` contiene el tipo extraído originalmente del Major.
- `identificador_sensor` corresponde al UUID del sensor.
- La GUI utiliza `LogicaFakeNavegador.mostrarDatos()`.
- La GUI no realiza directamente peticiones HTTP.
- La GUI no accede directamente a la base de datos.
- La última medición se solicita al abrir la página.
- El usuario puede volver a solicitar la última medición mediante el botón `Actualizar`.
- La actualización es manual y no periódica.
- Mientras se realiza una actualización, el botón puede desactivarse temporalmente.
- Si ocurre un error al recuperar la medición, se muestra un mensaje al usuario.

# Reglas Generales

- **Lenguajes:** HTML, CSS y JavaScript.
- **Interfaz:** sencilla, clara y orientada a comprobar el funcionamiento del Sprint 0.
- **Encabezados de Funciones/Métodos:** cada función JavaScript deberá incluir su diseño lógico dentro de comentarios delimitados por líneas discontinuas (`--------------------`).
- **Legibilidad del Código:** HTML, CSS y JavaScript deberán mantenerse separados.
- **Separación de Responsabilidades:** `web_gui` no realizará peticiones HTTP directamente.
- **Comunicación:** la recuperación de datos se realizará mediante `web_frontend_business_logic`.
- **Actualización:** la última medición podrá solicitarse manualmente mediante el botón `Actualizar`.
- **Persistencia:** la interfaz no accede directamente a la base de datos.
- **Pruebas Automatizadas:** se deberán mantener pruebas que comprueben que los datos recibidos se muestran correctamente en la interfaz.