-- ------------------------------------------------------------
-- Archivo: database_test.sql
--
-- Descripción:
-- Pruebas sencillas de integración para comprobar
-- el funcionamiento de la tabla MEDICIONES.
--
-- Las filas utilizadas por las pruebas se eliminan antes
-- de comenzar para permitir ejecutar este archivo varias veces.
--
-- Autor: Élia Garcia Pons
-- Fecha: 14/10/2026
--
-- Copyright:
-- Uso académico - Proyecto de Biometría y Medio Ambiente.
--
-- Aportación:
-- Comprueba la inserción y recuperación de mediciones
-- en la base de datos del Sprint 0.
-- ------------------------------------------------------------


-- ------------------------------------------------------------
-- Preparación de las pruebas
--
-- Eliminar únicamente las filas utilizadas por este test
-- para que pueda ejecutarse varias veces sin producir
-- errores por clave primaria duplicada.
-- ------------------------------------------------------------

DELETE FROM MEDICIONES
WHERE
    identificador_sensor =
        '45505347-2D47-5449-2D50-524F592D3341'
    AND fecha_hora IN (
        '2026-10-13 17:00:00',
        '2026-10-13 17:00:05'
    );


-- ------------------------------------------------------------
-- TEST 1
--
-- Insertar una medición.
--
-- valor = Minor
-- identificador_sensor = UUID
-- tipo = parte de tipo extraída del Major
-- ------------------------------------------------------------

INSERT INTO MEDICIONES (
    valor,
    fecha_hora,
    identificador_sensor,
    tipo
)
VALUES (
    1234,
    '2026-10-13 17:00:00',
    '45505347-2D47-5449-2D50-524F592D3341',
    11
);


-- ------------------------------------------------------------
-- TEST 2
--
-- Recuperar la medición insertada.
-- ------------------------------------------------------------

SELECT
    valor,
    fecha_hora,
    identificador_sensor,
    tipo
FROM MEDICIONES
WHERE
    fecha_hora =
        '2026-10-13 17:00:00'
    AND identificador_sensor =
        '45505347-2D47-5449-2D50-524F592D3341';


-- ------------------------------------------------------------
-- TEST 3
--
-- Insertar una segunda medición del mismo sensor,
-- pero en una fecha/hora diferente.
-- ------------------------------------------------------------

INSERT INTO MEDICIONES (
    valor,
    fecha_hora,
    identificador_sensor,
    tipo
)
VALUES (
    1234,
    '2026-10-13 17:00:05',
    '45505347-2D47-5449-2D50-524F592D3341',
    11
);


-- ------------------------------------------------------------
-- TEST 4
--
-- Recuperar las dos mediciones utilizadas por este test
-- ordenadas por fecha/hora.
-- ------------------------------------------------------------

SELECT
    valor,
    fecha_hora,
    identificador_sensor,
    tipo
FROM MEDICIONES
WHERE
    identificador_sensor =
        '45505347-2D47-5449-2D50-524F592D3341'
    AND fecha_hora IN (
        '2026-10-13 17:00:00',
        '2026-10-13 17:00:05'
    )
ORDER BY fecha_hora ASC;