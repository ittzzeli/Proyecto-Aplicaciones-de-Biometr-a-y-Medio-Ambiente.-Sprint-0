<?php

// ------------------------------------------------------------
// Archivo: LogicaNegocio.php
//
// Descripción:
// Implementa la lógica de negocio encargada de almacenar
// y recuperar mediciones.
//
// Este componente es independiente de la capa de comunicación.
// No contiene rutas, peticiones HTTP, respuestas HTTP,
// códigos de estado ni formatos de transporte.
//
// Autor: Elia
// Fecha: 08/10/2026
//
// Aportación:
// Adaptación de la lógica de negocio del Sprint 0 para
// mantener una separación completa respecto a la capa
// de comunicación.
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
// ------------------------------------------------------------


class LogicaNegocio
{
    private PDO $conexion;


    // ------------------------------------------------------------
    // conexion: ConexionBD --> LogicaNegocio() -->
    // ------------------------------------------------------------
    //
    // Recibe una conexión ya creada con la base de datos
    // y la almacena para utilizarla en las operaciones
    // de persistencia.
    // ------------------------------------------------------------
    public function __construct(PDO $conexion)
    {
        $this->conexion = $conexion;
    }


    // ------------------------------------------------------------
    // medicion: Medicion --> leerDatos() --> resultado: B
    // ------------------------------------------------------------
    //
    // Guarda una medición en la tabla Medicion.
    //
    // La medición contiene:
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

        $consulta =
            $this->conexion->prepare($sql);

        return $consulta->execute([
            ':uuid' =>
                $medicion->uuid,

            ':fecha' =>
                $medicion->fecha,

            ':major' =>
                $medicion->major,

            ':minor' =>
                $medicion->minor,

            ':TxPower' =>
                $medicion->TxPower
        ]);
    }


    // ------------------------------------------------------------
    // mostrarDatos() --> medicion: MedicionAlmacenada
    // ------------------------------------------------------------
    //
    // Recupera la última medición almacenada.
    //
    // La medición recuperada incluye el id generado por
    // la base de datos.
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

        $consulta =
            $this->conexion->query($sql);

        $medicion =
            $consulta->fetch(PDO::FETCH_OBJ);


        if ($medicion === false) {
            return null;
        }


        return $medicion;
    }
}