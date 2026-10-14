# Diseño del Componente

El componente **mobile_gui** representa la interfaz y el funcionamiento principal de la aplicación Android.

Durante el Sprint 0 sus responsabilidades son:

- iniciar y detener la búsqueda de dispositivos BLE;
- detectar el iBeacon del sensor;
- interpretar la trama iBeacon recibida;
- extraer los datos necesarios;
- evitar procesar dos veces la misma medición utilizando el contador;
- crear una `Medicion`;
- entregar la medición a `mobile_frontend_business_logic`.

La interfaz gráfica no realiza peticiones HTTP directamente.

## Datos recibidos mediante iBeacon

La trama iBeacon contiene:

```text
UUID
Major
Minor
TxPower
```

En este proyecto se interpretan de la siguiente forma:

```text
UUID
 |
 v
identificador_sensor


Major
 |
 +-------------------+
 |                   |
 v                   v
byte superior     byte inferior
    |                 |
    v                 v
   tipo            contador


Minor
 |
 v
valor


TxPower
 |
 v
no se utiliza
```

## Conversión de la trama

La conversión utilizada por la aplicación es:

```text
identificador_sensor = UUID

tipo = parte superior del Major

contador = parte inferior del Major

valor = Minor

fecha_hora = fecha y hora actual del teléfono
```

Por ejemplo, si:

```text
UUID  = 45505347-2D47-5449-2D50-524F592D3341

Major = 0x0B05

Minor = 1234
```

entonces:

```text
identificador_sensor =
45505347-2D47-5449-2D50-524F592D3341

tipo = 11

contador = 5

valor = 1234
```

El contador no forma parte de `Medicion`.

Únicamente se utiliza en el teléfono para comprobar si una medición ya ha sido procesada.

TxPower tampoco forma parte de `Medicion`.

## Tipo Medicion

Después de interpretar la trama BLE se crea:

```text
Medicion = (
    valor: Z,
    fecha_hora: Text,
    identificador_sensor: Text,
    tipo: Z
)
```

Esta estructura coincide con la definida en:

```text
business_logic_design.md
mobile_frontend_business_logic_design.md
database_design.md
```

## Flujo general

```text
Sensor
   |
   | iBeacon
   v
MainActivity
   |
   v
TramaIBeacon
   |
   | UUID
   | Major
   | Minor
   | TxPower
   v
Interpretación
   |
   +--> UUID ----------------> identificador_sensor
   |
   +--> Major byte superior -> tipo
   |
   +--> Major byte inferior -> contador
   |
   +--> Minor ---------------> valor
   |
   +--> TxPower --------------> descartado
   |
   v
Comprobar contador
   |
   v
Crear Medicion
   |
   v
LogicaFakeTelefono.leerDatos()
```

## Control de mediciones repetidas

El byte inferior del Major contiene un contador.

```text
Major = (tipo << 8) | contador
```

El teléfono conserva el último contador procesado.

Cuando recibe una trama:

```text
contador nuevo == ultimo contador
        |
        v
No enviar nuevamente la medición
```

Si:

```text
contador nuevo != ultimo contador
        |
        v
Crear y enviar una nueva Medicion
```

El contador no se almacena en la base de datos ni se envía al backend.

## Clase MainActivity

`MainActivity` coordina la interfaz y el escaneo BLE.

Sus responsabilidades son:

```text
- solicitar los permisos BLE necesarios;
- iniciar el escaneo;
- detener el escaneo;
- recibir los resultados BLE;
- localizar el iBeacon utilizado en el proyecto;
- interpretar la trama;
- controlar mediciones repetidas;
- crear Medicion;
- llamar a LogicaFakeTelefono.leerDatos().
```

`MainActivity` no realiza peticiones HTTP directamente.

## Clase TramaIBeacon

`TramaIBeacon` representa una trama iBeacon recibida.

Permite obtener:

```text
UUID
Major
Minor
TxPower
```

La clase no conoce la base de datos ni el servidor REST.

## Clase Utilidades

`Utilidades` contiene operaciones auxiliares necesarias para interpretar los bytes recibidos mediante BLE.

No contiene lógica HTTP ni acceso a la base de datos.

## Comunicación con mobile_frontend_business_logic

Una vez creada una medición:

```text
Medicion medicion
        |
        v
LogicaFakeTelefono.leerDatos(medicion)
```

La GUI no conoce:

```text
POST /medicion
JSON
HTTP
ServidorREST
PDO
MEDICIONES
```

Todos los detalles de comunicación se encuentran encapsulados en:

```text
mobile_frontend_business_logic
```

# Aclaraciones del Diseño

- La aplicación está desarrollada en Java.
- `mobile_gui` se encarga del escaneo BLE y de interpretar la trama iBeacon.
- El UUID se transforma en `identificador_sensor`.
- El Minor se transforma directamente en `valor`.
- Del Major se extraen `tipo` y `contador`.
- `tipo` corresponde al byte superior del Major.
- `contador` corresponde al byte inferior del Major.
- El contador únicamente se utiliza para evitar mediciones repetidas.
- El contador no se almacena ni se envía al backend.
- TxPower no se almacena ni se envía al backend.
- La fecha y hora se genera en el teléfono cuando se procesa una nueva medición.
- La GUI crea una `Medicion` con `valor`, `fecha_hora`, `identificador_sensor` y `tipo`.
- La GUI utiliza `LogicaFakeTelefono.leerDatos()` para entregar la medición.
- La GUI no realiza directamente peticiones HTTP.
- Durante el Sprint 0 el valor enviado por el sensor es el valor ficticio `1234`.

# Reglas Generales

- **Lenguaje de Programación:** Java para Android.
- **Interfaz Gráfica:** sencilla y orientada a comprobar el funcionamiento del Sprint 0.
- **Encabezados de Funciones/Métodos:** cada función o método deberá incluir su diseño lógico dentro de comentarios delimitados por líneas discontinuas (`--------------------`).
- **Legibilidad del Código:** el código deberá mantenerse sencillo y comprensible.
- **Separación de Responsabilidades:** la GUI no realizará comunicación HTTP directa ni acceso a la base de datos.
- **BLE:** la interpretación de la trama deberá mantener separados UUID, Major, Minor y TxPower.
- **Conversión:** antes de crear `Medicion` se deberán obtener `valor`, `identificador_sensor` y `tipo`.
- **Duplicados:** el contador del Major se utilizará únicamente para detectar nuevas mediciones.
- **Pruebas Automatizadas:** se mantendrán pruebas sobre la interpretación de la trama iBeacon.