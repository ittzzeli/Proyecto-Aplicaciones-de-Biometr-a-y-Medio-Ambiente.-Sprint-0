# Diseño del Componente

El componente **mobile_frontend_business_logic** representa en el cliente Android la parte de la lógica de negocio utilizada por la aplicación móvil.

Actúa como una lógica fake/proxy del componente `business_logic` del backend.

La interfaz gráfica móvil no realiza directamente peticiones HTTP ni conoce las rutas REST. Utiliza las operaciones proporcionadas por este componente.

Durante el Sprint 0, el cliente móvil utiliza únicamente:

```text
leerDatos()
```

Esta operación mantiene el mismo nombre, entrada y salida lógica que su equivalente en `business_logic`.

## Tipos de datos

```text
Medicion = (
    valor: Z,
    fecha_hora: Text,
    identificador_sensor: Text,
    tipo: Z
)

RespuestaREST = (
    codigo: N,
    json: Text
)

PeticionarioREST = componente encargado de realizar
peticiones HTTP al backend
```

El tipo `Medicion` coincide con el definido en `business_logic_design.md`.

## Correspondencia con business_logic

El backend define:

```text
medicion: Medicion --> leerDatos() --> resultado: B
```

El cliente móvil mantiene exactamente:

```text
medicion: Medicion --> leerDatos() --> resultado: B
```

Por tanto:

```text
MÓVIL

medicion: Medicion
      |
      v
LogicaFakeTelefono.leerDatos()
      |
      v
resultado: B
```

La diferencia está únicamente en la implementación.

En el backend, `leerDatos()` almacena la medición.

En el móvil, `leerDatos()` actúa como proxy y la envía al backend.

## Flujo del componente

```text
mobile_gui
    |
    | Medicion
    v
LogicaFakeTelefono.leerDatos()
    |
    | Medicion a JSON
    v
PeticionarioREST
    |
    | POST /medicion
    v
Backend
```

La interfaz móvil no conoce cómo se realiza la petición HTTP.

## Clase LogicaFakeTelefono

```text
                 -------- LogicaFakeTelefono --------
                 |
                 | peticionario: PeticionarioREST
                 |
                 |
servidor: Text --> LogicaFakeTelefono() -->
                 |
                 |
peticionario: PeticionarioREST
--> LogicaFakeTelefono() -->
                 |
                 |
medicion: Medicion
--> leerDatos()
--> resultado: B
                 |
                 |
medicion: Medicion
--> crearJson()
--> json: Text
                 |
                 -------------------------------------
```

### LogicaFakeTelefono()

```text
servidor: Text --> LogicaFakeTelefono() -->
```

Crea la lógica fake utilizando la dirección del servidor.

También puede construirse mediante:

```text
peticionario: PeticionarioREST
--> LogicaFakeTelefono() -->
```

Esta segunda forma permite sustituir la comunicación real durante las pruebas automatizadas.

### leerDatos()

```text
medicion: Medicion --> leerDatos() --> resultado: B
```

Mantiene exactamente la misma interfaz lógica que:

```text
business_logic.leerDatos()
```

Internamente realiza:

```text
Medicion
    |
    v
crearJson()
    |
    v
JSON
    |
    v
PeticionarioREST
    |
    | POST /medicion
    v
Backend
```

Devuelve `true` cuando el backend responde correctamente y `false` cuando se produce un error.

### crearJson()

```text
medicion: Medicion --> crearJson() --> json: Text
```

Convierte una `Medicion` al JSON esperado por el backend.

Contiene:

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
    "fecha_hora": "2026-10-13 17:00:00",
    "identificador_sensor": "45505347-2D47-5449-2D50-524F592D3341",
    "tipo": 11
}
```

## Clase PeticionarioREST

`PeticionarioREST` encapsula la comunicación HTTP.

```text
                 -------- PeticionarioREST --------
                 |
                 | servidor: Text
                 |
                 |
servidor: Text --> PeticionarioREST() -->
                 |
                 |
metodo: Text,
ruta: Text,
cuerpo: Text
-->
realizarPeticion()
-->
respuesta: RespuestaREST
                 |
                 -----------------------------------
```

Para almacenar una medición se utiliza:

```text
metodo = POST
ruta = /medicion
cuerpo = Medicion en JSON
```

`PeticionarioREST` no conoce el significado de los datos de la medición.

# Aclaraciones del Diseño

- `mobile_frontend_business_logic` está separado de `mobile_gui`.
- La GUI no realiza peticiones HTTP directamente.
- `LogicaFakeTelefono` actúa como proxy/fake de `business_logic`.
- `leerDatos()` mantiene exactamente la misma firma lógica que `business_logic.leerDatos()`.
- Una `Medicion` contiene únicamente `valor`, `fecha_hora`, `identificador_sensor` y `tipo`.
- `valor` procede originalmente del Minor, pero este componente recibe ya el valor procesado.
- `identificador_sensor` procede originalmente del UUID.
- `tipo` procede originalmente del Major.
- El contador del Major no se envía al backend.
- TxPower no se envía al backend.
- `LogicaFakeTelefono` convierte la medición a JSON.
- `PeticionarioREST` encapsula HTTP.
- La ruta utilizada es `POST /medicion`.
- Este componente no contiene SQL ni accede directamente a la base de datos.

# Reglas Generales

- **Lenguaje de Programación:** Java para Android.
- **Correspondencia con Backend:** las operaciones públicas que representan funciones de `business_logic` deberán mantener el mismo nombre, entradas y salidas lógicas.
- **Encabezados de Funciones/Métodos:** cada función o método deberá incluir su diseño lógico dentro de un bloque de comentarios delimitado por líneas discontinuas (`--------------------`).
- **Legibilidad del Código:** las responsabilidades de medición, lógica fake y comunicación HTTP deberán permanecer separadas.
- **Separación de Responsabilidades:** `mobile_gui` no contendrá comunicación HTTP directa.
- **Comunicación:** toda comunicación con el backend se encapsulará en `mobile_frontend_business_logic`.
- **Persistencia:** este componente no accede directamente a la base de datos.
- **Pruebas Automatizadas:** se deberán mantener pruebas para `leerDatos()`, la generación del JSON y la petición `POST /medicion`.