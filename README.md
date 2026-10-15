# Proyecto Aplicaciones de Biometría y Medio Ambiente - Sprint 0

Repositorio correspondiente al **Sprint 0** del proyecto de Aplicaciones de Biometría y Medio Ambiente.

El objetivo de este Sprint es comprender la arquitectura del sistema mediante ingeniería inversa, documentar sus componentes y comprobar el funcionamiento completo de una medición desde el sensor hasta una interfaz web.

Durante el Sprint también se han añadido y adaptado diseños, pruebas automatizadas y componentes necesarios para comunicar las distintas partes del sistema.

---

## Funcionamiento general

El sistema está formado por los siguientes componentes:

```text
Sensor
  |
  | BLE / iBeacon
  v
Aplicación Android
  |
  | API REST
  v
Servidor
  |
  v
Lógica de negocio
  |
  v
Base de datos
  |
  v
Página web
```

Para comprobar el funcionamiento completo del Sprint se utiliza una medición ficticia con valor:

```text
1234
```

El sensor publica esta medición mediante una trama iBeacon.

La aplicación Android recibe la trama y obtiene:

```text
valor = Minor
tipo = byte alto del Major
contador = byte bajo del Major
identificador_sensor = UUID
fecha_hora = fecha y hora de recepción
```

Después, Android envía la medición al servidor mediante:

```text
POST /medicion
```

El servidor almacena la medición en la base de datos.

Finalmente, la página web solicita la última medición mediante:

```text
GET /medicion
```

y la muestra en pantalla.

---

## Estructura del repositorio

```text
.
├── author.md
├── README.md
├── doc/
└── src/
```

### `doc/`

Contiene los diseños de los distintos componentes:

```text
business_logic_design.md
communication_design.md
database_design.md
mobile_frontend_business_logic_design.md
mobile_gui_design.md
sensor_design.md
web_frontend_business_logic_design.md
web_gui_design.md
```

### `src/`

Contiene las implementaciones correspondientes a los diseños:

```text
src/
├── business_logic/
├── communication/
├── database/
├── mobile_frontend_business_logic/
├── mobile_gui/
├── sensor/
├── web_frontend_business_logic/
└── web_gui/
```

---

## Componentes

### Sensor

El componente `sensor` está desarrollado en C++ para una placa compatible con Arduino.

Durante el Sprint 0 publica una medición ficticia mediante iBeacon.

Datos principales:

```text
Nombre BLE: Elia_GTI

UUID:
45505347-2D47-5449-2D50-524F592D3341

Tipo:
11

Valor ficticio:
1234
```

El campo `Major` contiene:

```text
byte alto = tipo
byte bajo = contador
```

El campo `Minor` contiene:

```text
valor de la medición
```

---

### Aplicación Android

La aplicación está desarrollada en Java.

Se encarga de:

```text
- buscar el beacon Elia_GTI;
- interpretar la trama iBeacon;
- evitar procesar repetidamente la misma medición;
- crear una Medicion;
- enviar la medición al servidor.
```

El modelo utilizado es:

```text
Medicion = (
    valor,
    fecha_hora,
    identificador_sensor,
    tipo
)
```

---

### API REST

El componente `communication` proporciona la comunicación entre los clientes y la lógica de negocio.

Rutas utilizadas:

```text
POST /medicion
GET /medicion
```

#### POST `/medicion`

Permite almacenar una medición.

Ejemplo:

```json
{
    "valor": 1234,
    "fecha_hora": "2026-10-14 09:05:00",
    "identificador_sensor": "45505347-2D47-5449-2D50-524F592D3341",
    "tipo": 11
}
```

#### GET `/medicion`

Devuelve la última medición almacenada.

---

## Base de datos

La tabla utilizada es:

```text
MEDICIONES
```

Su estructura es:

```text
valor                  INT
fecha_hora             DATETIME
identificador_sensor   VARCHAR(36)
tipo                   INT
```

La clave primaria está formada por:

```text
(fecha_hora, identificador_sensor)
```

El script de creación se encuentra en:

```text
src/database/database.sql
```

---

# Despliegue

## Base de datos

Crear una base de datos MySQL/MariaDB.

Después ejecutar:

```text
src/database/database.sql
```

Este script crea la tabla:

```text
MEDICIONES
```

---

## Servidor PHP

El backend necesita un servidor con:

```text
PHP
MySQL/MariaDB
Apache
```

Durante el desarrollo local se puede utilizar XAMPP.

Los componentes del backend son:

```text
src/business_logic/
src/communication/
```

En el servidor utilizado durante el Sprint se despliegan dentro de `httpdocs`.

La estructura utilizada es:

```text
httpdocs/
├── business_logic/
├── communication/
├── web_frontend_business_logic/
├── web_gui/
└── .htaccess
```

Las credenciales de la base de datos deben configurarse en la copia del servidor de:

```text
communication/index.php
```

Las credenciales privadas del servidor no deben almacenarse en el repositorio.

---

## Configuración de Apache

Para utilizar la ruta:

```text
/medicion
```

el servidor Apache utiliza un archivo `.htaccess` en `httpdocs`.

Contenido:

```apache
RewriteEngine On

RewriteRule ^medicion/?$ communication/index.php [L,QSA]
```

De esta forma:

```text
/medicion
```

es redirigido internamente a:

```text
communication/index.php
```

---

## Página web

Los componentes web son:

```text
src/web_frontend_business_logic/
src/web_gui/
```

En el servidor se despliegan como:

```text
httpdocs/web_frontend_business_logic/
httpdocs/web_gui/
```

La interfaz muestra la última medición almacenada.

El botón:

```text
Actualizar
```

permite solicitar de nuevo la última medición sin recargar completamente la página.

Durante el Sprint 0 la página desplegada se encuentra en:

```text
https://egarpon.upv.edu.es/web_gui/
```

---

# Ejecución de pruebas

Las pruebas deben ejecutarse desde la raíz del repositorio.

---

## Prueba de la base de datos

Archivo:

```text
src/database/database_test.sql
```

Puede ejecutarse desde phpMyAdmin:

1. Iniciar Apache y MySQL.
2. Abrir phpMyAdmin.
3. Seleccionar la base de datos del proyecto.
4. Abrir la pestaña `SQL`.
5. Copiar el contenido de `database_test.sql`.
6. Ejecutarlo.

El test puede ejecutarse varias veces porque elimina previamente únicamente las filas utilizadas por las pruebas.

El resultado esperado incluye dos mediciones con:

```text
valor = 1234
tipo = 11
```

---

## Tests de lógica de negocio

Desde Git Bash:

```bash
/c/xampp/php/php.exe src/business_logic/LogicaNegocioTests.php
```

El test comprueba principalmente:

```text
- creación de Medicion;
- almacenamiento de una medición;
- recuperación de la última medición;
- valor;
- fecha y hora;
- identificador del sensor;
- tipo.
```

---

## Tests de comunicación REST

Desde Git Bash:

```bash
/c/xampp/php/php.exe src/communication/ServidorRESTTest.php
```

Comprueba el funcionamiento de:

```text
POST /medicion
GET /medicion
```

y diferentes casos de error.

---

## Tests de la lógica frontend web

Es necesario tener Node.js instalado.

Desde Git Bash:

```bash
node src/web_frontend_business_logic/LogicaFakeNavegadorTest.js
```

Comprueba:

```text
- GET /medicion;
- uso correcto de la URL;
- desactivación de caché;
- valor;
- fecha y hora;
- identificador del sensor;
- tipo;
- gestión de errores HTTP.
```

---

## Tests de la interfaz web

Desde Git Bash:

```bash
node src/web_gui/WebTest.js
```

Comprueba que la interfaz muestra correctamente:

```text
valor
fecha_hora
identificador_sensor
tipo
```

y que muestra un mensaje cuando ocurre un error.

---

## Tests de Android

Los tests Android se ejecutan desde Android Studio.

Tests principales:

```text
LogicaFakeTelefonoTest.java
TramaIBeaconTest.java
```

`LogicaFakeTelefonoTest` comprueba la creación y envío de una medición desde la lógica frontend móvil.

`TramaIBeaconTest` comprueba la interpretación de una trama iBeacon conocida:

```text
Major = 0x0B05

tipo = 11
contador = 5

Minor = 1234
```

También comprueba el UUID y TxPower de la trama.

Después de ejecutar los tests se debe comprobar también que el proyecto Android compila correctamente.

---

## Test del sensor

Archivo:

```text
src/sensor/test_codificador_major.cpp
```

Comprueba la codificación utilizada para construir el campo `Major`.

Para:

```text
tipo = 11
contador = 5
```

el resultado esperado es:

```text
Major = 0x0B05
```

Si se dispone de `g++`, puede compilarse desde la raíz del repositorio con:

```bash
g++ -std=c++17 src/sensor/test_codificador_major.cpp -o src/sensor/test_codificador_major
```

Y ejecutarse con:

```bash
./src/sensor/test_codificador_major
```

---

# Test de funcionamiento completo

El Sprint 0 debe comprobar también el funcionamiento completo del sistema.

El proceso utilizado es:

```text
1. El sensor genera el valor ficticio 1234.

2. El sensor publica una trama iBeacon.

3. La aplicación Android busca Elia_GTI.

4. Android recibe la trama.

5. Android obtiene:

   valor = 1234
   identificador_sensor = UUID
   tipo = 11
   contador = byte bajo del Major

6. Android genera fecha_hora.

7. Android envía:

   POST /medicion

8. El servidor recibe la petición.

9. La lógica de negocio almacena la medición
   en la tabla MEDICIONES.

10. La página web solicita:

    GET /medicion

11. La página muestra la última medición.

12. El usuario puede pulsar Actualizar para
    consultar de nuevo la última medición.
```

El resultado esperado es que el valor:

```text
1234
```

aparezca finalmente en la página web.

---

## Tecnologías utilizadas

```text
C / C++
Arduino
Bluetooth Low Energy
iBeacon
Java
Android
PHP
REST
MySQL / MariaDB
HTML
CSS
JavaScript
Node.js
Apache
Plesk
Git
GitHub
```