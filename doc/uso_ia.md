# Uso de herramientas de IA

Durante el Sprint 0 se ha utilizado inteligencia artificial como herramienta de apoyo para generar, revisar y corregir código a partir de los diseños realizados previamente.

La IA no se ha utilizado como sustitución del diseño del sistema.

El proceso seguido ha sido:

```text
Diseño previo
    |
    v
Redacción del prompt
    |
    v
Generación o modificación del código
    |
    v
Revisión del resultado
    |
    v
Pruebas
    |
    v
Correcciones
```

Los prompts mostrados a continuación corresponden a versiones consolidadas de las instrucciones utilizadas durante el Sprint.

---

# 1. Base de datos

## Diseño utilizado

```text
doc/database_design.md
```

## Objetivo

Generar la base de datos correspondiente al modelo de mediciones definido previamente.

## Prompt

```text
Implementa el componente database a partir de
doc/database_design.md.

Debes respetar exactamente el diseño proporcionado.

Utiliza SQL compatible con MySQL/MariaDB.

Durante el Sprint 0 debe existir únicamente la tabla MEDICIONES.

La tabla debe contener:

- valor INT NOT NULL
- fecha_hora DATETIME NOT NULL
- identificador_sensor VARCHAR(36) NOT NULL
- tipo INT NOT NULL

La clave primaria debe ser compuesta por:

(fecha_hora, identificador_sensor)

No añadas un campo id.

No almacenes Major, Minor, contador ni TxPower como columnas
independientes.

Genera también una prueba que permita comprobar la inserción
y recuperación de mediciones.

Mantén el código sencillo y ajustado al diseño.
```

## Revisión realizada

Después de generar la implementación se comprobó que:

```text
- la tabla se llamara MEDICIONES;
- existieran únicamente las cuatro columnas definidas;
- no existiera un campo id;
- la clave primaria fuera compuesta;
- el contador no se almacenara;
- Major, Minor y TxPower no se almacenaran;
- database_test.sql pudiera ejecutarse varias veces.
```

También se realizó una prueba manual desde phpMyAdmin utilizando:

```text
valor = 1234
tipo = 11
```

---

# 2. Lógica de negocio

## Diseño utilizado

```text
doc/business_logic_design.md
doc/database_design.md
```

## Objetivo

Implementar la lógica de negocio encargada de almacenar y recuperar mediciones.

## Prompt

```text
Genera la implementación del componente business_logic
siguiendo exactamente doc/business_logic_design.md y
doc/database_design.md.

Utiliza PHP 8 y PDO para MySQL/MariaDB.

Implementa la clase LogicaNegocio.

Debe disponer de:

conexion: ConexionBD --> LogicaNegocio() -->

medicion: Medicion --> leerDatos() --> resultado: B

mostrarDatos() --> medicion: Medicion

El tipo Medicion contiene únicamente:

- valor
- fecha_hora
- identificador_sensor
- tipo

leerDatos() debe almacenar la medición en la tabla MEDICIONES.

mostrarDatos() debe recuperar la última medición ordenando
por fecha_hora de forma descendente.

La lógica de negocio debe ser completamente independiente
de la capa de comunicación.

No debe conocer:

- HTTP
- REST
- JSON
- rutas
- códigos de estado
- peticiones o respuestas HTTP

No añadas dependencias hacia communication.

Genera también pruebas automatizadas para leerDatos()
y mostrarDatos().

Añade a cada fichero una cabecera con nombre, descripción,
copyright, fecha, autor y aportación.

Cada función o método debe incluir su diseño lógico y una
breve descripción dentro de comentarios delimitados por
líneas discontinuas.
```

## Revisión realizada

Se comprobó que:

```text
- LogicaNegocio no dependiera de communication;
- no existiera código HTTP dentro de business_logic;
- Medicion tuviera únicamente los cuatro campos definidos;
- leerDatos() realizara el INSERT en MEDICIONES;
- mostrarDatos() recuperara la última medición;
- las consultas coincidieran con database_design.md.
```

Después se ejecutaron las pruebas automatizadas de:

```text
src/business_logic/LogicaNegocioTests.php
```

---

# 3. API REST

## Diseños utilizados

```text
doc/communication_design.md
doc/business_logic_design.md
```

## Objetivo

Implementar la capa de comunicación REST sin introducir lógica de negocio ni acceso directo a la base de datos.

## Prompt

```text
Genera el componente communication siguiendo
doc/communication_design.md.

Utiliza PHP 8.

El componente debe actuar únicamente como intermediario
entre HTTP y business_logic.

Implementa las rutas:

POST /medicion
GET /medicion

POST /medicion debe recibir un JSON con:

- valor
- fecha_hora
- identificador_sensor
- tipo

Debe convertir los datos recibidos en una Medicion y llamar a:

business_logic.leerDatos(medicion)

GET /medicion debe llamar a:

business_logic.mostrarDatos()

y serializar la Medicion recibida a JSON.

communication no debe acceder directamente a la base de datos.

Los códigos de respuesta deben corresponder con el diseño:

POST:
201 -> medición almacenada
400 -> petición incorrecta
500 -> error al almacenar

GET:
200 -> medición encontrada
404 -> no existe medición

Mantén completamente separadas communication y
business_logic.

Genera pruebas automatizadas utilizando una lógica de
negocio falsa para poder comprobar las rutas sin depender
de una base de datos real.
```

## Revisión realizada

Se comprobó que:

```text
- ServidorREST no ejecutara SQL;
- POST /medicion delegara en leerDatos();
- GET /medicion delegara en mostrarDatos();
- los campos JSON coincidieran con Medicion;
- no se utilizaran los antiguos campos id, Major, Minor o TxPower;
- las respuestas de error fueran gestionadas.
```

Después se ejecutaron:

```text
src/communication/ServidorRESTTest.php
```

---

# 4. Aplicación Android y lógica fake

## Diseños utilizados

```text
doc/mobile_frontend_business_logic_design.md
doc/mobile_gui_design.md
doc/sensor_design.md
```

## Objetivo

Recibir la trama iBeacon del sensor, transformarla al modelo lógico `Medicion` y enviarla al backend manteniendo separada la interfaz de la comunicación HTTP.

## Prompt

```text
Implementa la parte móvil respetando
mobile_frontend_business_logic_design.md y
mobile_gui_design.md.

La aplicación debe estar desarrollada en Java.

Debe buscar únicamente el beacon llamado:

Elia_GTI

Al recibir una trama iBeacon debe obtener:

UUID -> identificador_sensor

byte alto de Major -> tipo

byte bajo de Major -> contador

Minor -> valor

TxPower no debe formar parte de Medicion.

El contador se utiliza únicamente para evitar procesar varias
veces la misma medición y no debe enviarse al backend.

La aplicación debe crear:

Medicion(
    valor,
    fecha_hora,
    identificador_sensor,
    tipo
)

La GUI no debe realizar directamente peticiones HTTP.

Para enviar la medición debe utilizar:

LogicaFakeTelefono.leerDatos(medicion)

La lógica fake debe encapsular la petición al backend.

La ruta utilizada para almacenar la medición es:

POST /medicion

Mantén las responsabilidades separadas y no añadas lógica
innecesaria a MainActivity.

Genera pruebas automatizadas para la lógica fake y para la
interpretación de una trama iBeacon conocida.
```

## Revisión realizada

Se comprobó que:

```text
- Android detectara Elia_GTI;
- Minor se transformara en valor;
- el byte alto del Major produjera tipo = 11;
- el byte bajo se utilizara como contador;
- el contador evitara mediciones repetidas;
- TxPower no se enviara al backend;
- MainActivity no realizara directamente HTTP;
- LogicaFakeTelefono encapsulara la comunicación.
```

Se probó también físicamente el sensor con Android.

Para una trama con:

```text
Major = 0x0B05
Minor = 1234
```

se comprobó:

```text
tipo = 11
contador = 5
valor = 1234
```

Las pruebas automatizadas utilizadas fueron:

```text
LogicaFakeTelefonoTest.java
TramaIBeaconTest.java
```

---

# 5. Página web y lógica fake

## Diseños utilizados

```text
doc/web_frontend_business_logic_design.md
doc/web_gui_design.md
```

## Objetivo

Mostrar la última medición almacenada sin introducir comunicación HTTP directamente en la interfaz gráfica.

## Prompt

```text
Implementa los componentes web_frontend_business_logic
y web_gui a partir de sus diseños.

Utiliza JavaScript para la lógica y HTML/CSS para la interfaz.

La GUI web no debe utilizar fetch() directamente.

Debe utilizar:

LogicaFakeNavegador.mostrarDatos()

La operación mostrarDatos() debe mantener la misma firma
lógica que business_logic.mostrarDatos().

Internamente LogicaFakeNavegador debe realizar:

GET /medicion

La Medicion devuelta contiene únicamente:

- valor
- fecha_hora
- identificador_sensor
- tipo

La página debe mostrar estos cuatro valores.

Debe existir un botón:

Actualizar

Al pulsarlo se debe solicitar de nuevo la última medición sin
recargar completamente la página.

Mientras se realiza la actualización puede desactivarse
temporalmente el botón para evitar peticiones simultáneas.

La petición GET debe evitar reutilizar respuestas almacenadas
en caché.

Genera pruebas automatizadas para:

- comprobar la ruta;
- comprobar el método GET;
- comprobar los campos recibidos;
- comprobar la gestión de errores;
- comprobar que no se utilice caché.
```

## Revisión realizada

Durante las pruebas en el navegador se detectó inicialmente un problema al guardar directamente `fetch` como función:

```text
TypeError: Failed to execute 'fetch' on 'Window': Illegal invocation
```

La implementación se corrigió encapsulando la llamada mediante una función.

Posteriormente se añadió:

```text
cache: "no-store"
```

para evitar reutilizar respuestas anteriores al pulsar `Actualizar`.

También se comprobó que:

```text
- web_gui no utilizara fetch directamente;
- la petición apareciera como GET /medicion;
- el servidor respondiera con código 200;
- el botón Actualizar recuperara nuevas mediciones;
- se mostraran valor, fecha_hora, identificador_sensor y tipo.
```

Las pruebas utilizadas fueron:

```text
src/web_frontend_business_logic/LogicaFakeNavegadorTest.js
src/web_gui/WebTest.js
```

---

# 6. Revisión del código generado

El código generado o modificado con ayuda de IA no se aceptó directamente sin revisión.

Durante el Sprint se realizaron varias fases de corrección.

Entre otras cosas se revisó:

```text
- correspondencia entre doc/ y src/;
- separación entre communication y business_logic;
- separación entre GUI y lógica fake;
- correspondencia entre Medicion y MEDICIONES;
- eliminación de campos antiguos;
- funcionamiento de las rutas REST;
- interpretación de Major y Minor;
- control de mediciones repetidas;
- comentarios y cabeceras;
- funcionamiento de los tests;
- funcionamiento completo sensor -> Android -> servidor -> BBDD -> web.
```

Cuando se detectó una diferencia entre diseño e implementación, se corrigió antes de continuar.

---

# 7. Uso responsable de IA

La IA se ha utilizado como herramienta de apoyo.

Las decisiones principales de arquitectura y diseño se han mantenido documentadas dentro de:

```text
doc/
```

Antes de modificar funcionalidades se ha comprobado su relación con los diseños existentes.

Las implementaciones generadas se han probado mediante:

```text
- pruebas automatizadas;
- pruebas de base de datos;
- compilación de la aplicación Android;
- pruebas físicas BLE;
- peticiones REST;
- pruebas en navegador;
- prueba completa de extremo a extremo.
```

Las credenciales privadas utilizadas durante el despliegue no se almacenan en el repositorio.