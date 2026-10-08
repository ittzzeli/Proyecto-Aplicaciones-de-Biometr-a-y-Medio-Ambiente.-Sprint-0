<?php

// ------------------------------------------------------------
// LogicaNegocio
//
// Responsabilidad:
// Gestionar el almacenamiento y recuperación de mediciones.
//
// Este componente NO recibe ni devuelve peticiones HTTP.
// La comunicación REST se implementará en otro componente.
// ------------------------------------------------------------

class LogicaNegocio
{
    private PDO $conexion;


    // ------------------------------------------------------------
    // conexion: PDO --> LogicaNegocio()
    // ------------------------------------------------------------
    //
    // Recibe una conexión ya creada con la base de datos.
    // ------------------------------------------------------------
    public function __construct(PDO $conexion)
    {
        $this->conexion = $conexion;
    }


    // ------------------------------------------------------------
    // medicion: Medicion --> leerDatos() --> resultado: B
    // ------------------------------------------------------------
    //
    // Guarda en la base de datos los datos de una Medicion.
    //
    // La Medicion es un objeto JSON con:
    // uuid, fecha, major, minor y TxPower.
    // ------------------------------------------------------------
    public function leerDatos(object $medicion): bool
    {
        $sql = "
            INSERT INTO Medicion (
                uuid,
                fecha,
                major,
                minor,
                TxPower
            )
            VALUES (
                :uuid,
                :fecha,
                :major,
                :minor,
                :TxPower
            )
        ";

        $consulta = $this->conexion->prepare($sql);

        return $consulta->execute([
            ':uuid'    => $medicion->uuid,
            ':fecha'   => $medicion->fecha,
            ':major'   => $medicion->major,
            ':minor'   => $medicion->minor,
            ':TxPower' => $medicion->TxPower
        ]);
    }


    // ------------------------------------------------------------
    // mostrarDatos() --> medicion: Medicion
    // ------------------------------------------------------------
    //
    // Recupera la última Medicion almacenada en la base de datos.
    //
    // Si no existen mediciones devuelve null.
    // ------------------------------------------------------------
    public function mostrarDatos(): ?object
    {
        $sql = "
            SELECT
                id,
                uuid,
                fecha,
                major,
                minor,
                TxPower
            FROM Medicion
            ORDER BY id DESC
            LIMIT 1
        ";

        $consulta = $this->conexion->query($sql);

        $medicion = $consulta->fetch(PDO::FETCH_OBJ);

        if ($medicion === false) {
            return null;
        }

        return $medicion;
    }
}