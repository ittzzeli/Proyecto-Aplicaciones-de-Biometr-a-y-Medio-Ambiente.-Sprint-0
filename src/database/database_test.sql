-- ------------------------------------------------------------
-- Pruebas sencillas de la base de datos
-- ------------------------------------------------------------


-- ------------------------------------------------------------
-- TEST 1
-- Comprobar que se puede insertar una medición.
-- El campo id no se indica porque se genera automáticamente.
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
    '2026-10-01 12:00:00',
    1,
    0,
    -53
);


-- ------------------------------------------------------------
-- TEST 2
-- Comprobar que el id se ha generado automáticamente.
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
-- Insertar una segunda medición para comprobar
-- que los identificadores continúan incrementándose.
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
    '2026-10-01 12:01:00',
    2,
    0,
    -53
);


-- ------------------------------------------------------------
-- TEST 4
-- Recuperar todos los datos almacenados.
-- ------------------------------------------------------------

SELECT *
FROM Medicion
ORDER BY id ASC;