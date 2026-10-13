-- ------------------------------------------------------------
-- Archivo: database.sql
--
-- Descripción:
-- Define la tabla MEDICIONES utilizada durante el Sprint 0.
--
-- La tabla almacena únicamente los datos útiles obtenidos
-- a partir de la trama iBeacon:
--
-- - valor: obtenido del Minor.
-- - fecha_hora: momento de la medición.
-- - identificador_sensor: obtenido del UUID.
-- - tipo: extraído del Major.
--
-- Autor: Élia Garcia Pons
-- Fecha: 13/10/2026
--
-- Copyright:
-- Uso académico - Proyecto de Biometría y Medio Ambiente.
-- ------------------------------------------------------------


CREATE TABLE MEDICIONES (

    valor INT NOT NULL,

    fecha_hora DATETIME NOT NULL,

    identificador_sensor VARCHAR(36) NOT NULL,

    tipo INT NOT NULL,

    PRIMARY KEY (
        fecha_hora,
        identificador_sensor
    )
);