-- ------------------------------------------------------------
-- Archivo: database_test.sql
--
-- Descripción:
-- Pruebas sencillas de integración para comprobar
-- el funcionamiento de la tabla Medicion.
--
-- Autor: Elia
-- Fecha: 08/10/2026
--
-- Copyright:
-- Uso académico - Proyecto de Biometría y Medio Ambiente.
-- ------------------------------------------------------------


-- ------------------------------------------------------------
-- TEST 1
--
-- Insertar una primera medición.
--
-- id no se especifica porque debe generarse automáticamente.
-- ------------------------------------------------------------

INSERT INTO Medicion (
    uuid,
    fecha,
    major,
    minor,
    TxPower
)
VALUES (
    '45505347-2D47-5449-2D50-524F592D3341',
    '2026-10-08 16:00:00',
    2821,
    1234,
    -53
);


-- ------------------------------------------------------------
-- TEST 2
--
-- Recuperar la medición insertada y comprobar que
-- existe un id generado automáticamente.
-- ------------------------------------------------------------

SELECT
    id,
    uuid,
    fecha,
    major,
    minor,
    TxPower
FROM Medicion
ORDER BY id DESC
LIMIT 1;


-- ------------------------------------------------------------
-- TEST 3
--
-- Insertar una segunda medición para comprobar
-- que AUTO_INCREMENT continúa funcionando.
-- ------------------------------------------------------------

INSERT INTO Medicion (
    uuid,
    fecha,
    major,
    minor,
    TxPower
)
VALUES (
    '45505347-2D47-5449-2D50-524F592D3341',
    '2026-10-08 16:01:00',
    2822,
    1234,
    -53
);


-- ------------------------------------------------------------
-- TEST 4
--
-- Recuperar todas las mediciones almacenadas.
-- ------------------------------------------------------------

SELECT
    id,
    uuid,
    fecha,
    major,
    minor,
    TxPower
FROM Medicion
ORDER BY id ASC;