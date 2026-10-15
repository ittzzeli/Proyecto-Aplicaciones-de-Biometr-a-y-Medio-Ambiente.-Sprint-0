<?php

// ------------------------------------------------------------
// Archivo: LogicaNegocio.php
//
// Descripción:
// Implementa la lógica de negocio encargada de almacenar
// y recuperar mediciones.
//
// Trabaja con el modelo lógico:
// - valor
// - fecha_hora
// - identificador_sensor
// - tipo
//
// Este componente es independiente de la capa de comunicación.
// No contiene rutas, peticiones HTTP, respuestas HTTP,
// códigos de estado ni formatos de transporte.
//
// Autor: Élia Garcia Pons
// Fecha: 13/10/2026
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
//
// Aportación:
// Implementa las operaciones de almacenamiento y recuperación de
// mediciones manteniendo la lógica de negocio independiente de HTTP.
// ------------------------------------------------------------


class LogicaNegocio
{
    private PDO $conexion;


    // ------------------------------------------------------------
    // conexion: ConexionBD --> LogicaNegocio() -->
    // ------------------------------------------------------------
    //
    // Recibe una conexión ya creada con la base de datos.
    // ------------------------------------------------------------
    public function __construct(
        PDO $conexion
    ) {

        $this->conexion =
            $conexion;
    }


    // ------------------------------------------------------------
    // medicion: Medicion
    // --> leerDatos()
    // --> resultado: B
    // ------------------------------------------------------------
    //
    // Guarda una Medicion en la tabla MEDICIONES.
    //
    // Campos:
    // - valor
    // - fecha_hora
    // - identificador_sensor
    // - tipo
    // ------------------------------------------------------------
    public function leerDatos(
        object $medicion
    ): bool {

        $sql = "
            INSERT INTO MEDICIONES (
                valor,
                fecha_hora,
                identificador_sensor,
                tipo
            )
            VALUES (
                :valor,
                :fecha_hora,
                :identificador_sensor,
                :tipo
            )
        ";


        $consulta =
            $this->conexion->prepare(
                $sql
            );


        return $consulta->execute([
            ':valor' =>
                $medicion->valor,

            ':fecha_hora' =>
                $medicion->fecha_hora,

            ':identificador_sensor' =>
                $medicion->identificador_sensor,

            ':tipo' =>
                $medicion->tipo
        ]);
    }


    // ------------------------------------------------------------
    // mostrarDatos()
    // --> medicion: Medicion
    // ------------------------------------------------------------
    //
    // Recupera la última Medicion almacenada.
    //
    // La última medición se determina mediante fecha_hora.
    //
    // Si no existen mediciones devuelve null.
    // ------------------------------------------------------------
    public function mostrarDatos(): ?object
    {

        $sql = "
            SELECT
                valor,
                fecha_hora,
                identificador_sensor,
                tipo
            FROM MEDICIONES
            ORDER BY fecha_hora DESC
            LIMIT 1
        ";


        $consulta =
            $this->conexion->query(
                $sql
            );


        $medicion =
            $consulta->fetch(
                PDO::FETCH_OBJ
            );


        if ($medicion === false) {

            return null;
        }


        return $medicion;
    }
}