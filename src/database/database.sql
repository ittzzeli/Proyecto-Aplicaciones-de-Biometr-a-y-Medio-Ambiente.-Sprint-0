-- ------------------------------------------------------------
-- Base de datos del Sprint 0
-- Tabla para almacenar las mediciones recibidas.
-- ------------------------------------------------------------

CREATE TABLE Medicion (
    id INT AUTO_INCREMENT PRIMARY KEY,
    uuid VARCHAR(36) NOT NULL,
    fecha DATETIME NOT NULL,
    major INT NOT NULL,
    minor INT NOT NULL,
    TxPower INT NOT NULL
);