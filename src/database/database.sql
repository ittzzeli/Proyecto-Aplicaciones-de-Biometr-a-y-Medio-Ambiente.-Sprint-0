-- ------------------------------------------------------------
-- Archivo: database.sql
--
-- Descripción:
-- Define la tabla utilizada por el Sprint 0 para almacenar
-- las mediciones recibidas por el sistema.
--
-- Autor: Elia
-- Fecha: 08/10/2026
--
-- Copyright:
-- Uso académico - Proyecto de Biometría y Medio Ambiente.
-- ------------------------------------------------------------


CREATE TABLE Medicion (
    id INT AUTO_INCREMENT PRIMARY KEY,
    uuid VARCHAR(36) NOT NULL,
    fecha DATETIME NOT NULL,
    major INT NOT NULL,
    minor INT NOT NULL,
    TxPower INT NOT NULL
);