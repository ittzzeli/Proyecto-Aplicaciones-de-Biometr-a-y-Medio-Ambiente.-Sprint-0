<?php

// ------------------------------------------------------------
// Archivo: ServidorREST.php
//
// Descripción:
// Implementa el componente de comunicación REST del Sprint 0.
//
// Recibe peticiones HTTP ya separadas en método, ruta y cuerpo,
// transforma los datos recibidos y delega las operaciones en
// el componente business_logic.
//
// Este componente no accede directamente a la base de datos.
//
// Autor: Elia
// Fecha: 08/10/2026
//
// Aportación:
// Adaptación del servidor REST para mantener una separación
// completa entre communication y business_logic.
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
// ------------------------------------------------------------


class ServidorREST
{
    private object $logica;


    // ------------------------------------------------------------
    // logica: LogicaNegocio --> ServidorREST() -->
    // ------------------------------------------------------------
    //
    // Recibe una referencia al componente business_logic
    // que será utilizado para almacenar y recuperar mediciones.
    // ------------------------------------------------------------
    public function __construct(object $logica)
    {
        $this->logica = $logica;
    }


    // ------------------------------------------------------------
    // metodo: Text,
    // ruta: Text,
    // cuerpo: Text
    // --> manejarPeticion()
    // --> respuesta: RespuestaREST
    // ------------------------------------------------------------
    //
    // Determina qué operación REST debe ejecutarse según
    // el método HTTP y la ruta recibidos.
    //
    // Rutas disponibles:
    //
    // POST /medicion
    // GET  /medicion
    // ------------------------------------------------------------
    public function manejarPeticion(
        string $metodo,
        string $ruta,
        string $cuerpo = ""
    ): array {

        if (
            $metodo === "POST"
            &&
            $ruta === "/medicion"
        ) {

            return $this->postMedicion(
                $cuerpo
            );
        }


        if (
            $metodo === "GET"
            &&
            $ruta === "/medicion"
        ) {

            return $this->getMedicion();
        }


        $json = json_encode(
            [
                "error" => "Ruta no encontrada"
            ],
            JSON_UNESCAPED_UNICODE
        );


        return $this->crearRespuesta(
            404,
            $json
        );
    }


    // ------------------------------------------------------------
    // json: Text
    // --> postMedicion()
    // --> respuesta: RespuestaREST
    // ------------------------------------------------------------
    //
    // Convierte el JSON recibido en una Medicion.
    //
    // Comprueba que existan los campos necesarios y delega
    // el almacenamiento en business_logic.leerDatos().
    // ------------------------------------------------------------
    private function postMedicion(
        string $json
    ): array {

        $medicion =
            json_decode($json);


        // --------------------------------------------------------
        // Comprobar que el JSON recibido representa un objeto.
        // --------------------------------------------------------

        if (!is_object($medicion)) {

            $respuestaJson = json_encode(
                [
                    "error" => "JSON incorrecto"
                ],
                JSON_UNESCAPED_UNICODE
            );


            return $this->crearRespuesta(
                400,
                $respuestaJson
            );
        }


        // --------------------------------------------------------
        // Comprobar que existen todos los campos necesarios
        // para construir una Medicion.
        // --------------------------------------------------------

        if (
            !isset($medicion->uuid)
            ||
            !isset($medicion->fecha)
            ||
            !isset($medicion->major)
            ||
            !isset($medicion->minor)
            ||
            !isset($medicion->TxPower)
        ) {

            $respuestaJson = json_encode(
                [
                    "error" => "Medicion incorrecta"
                ],
                JSON_UNESCAPED_UNICODE
            );


            return $this->crearRespuesta(
                400,
                $respuestaJson
            );
        }


        // --------------------------------------------------------
        // communication no guarda directamente la medición.
        //
        // La operación se delega en business_logic.
        // --------------------------------------------------------

        $resultado =
            $this->logica->leerDatos(
                $medicion
            );


        if (!$resultado) {

            $respuestaJson = json_encode(
                [
                    "error" =>
                        "No se pudo guardar la Medicion"
                ],
                JSON_UNESCAPED_UNICODE
            );


            return $this->crearRespuesta(
                500,
                $respuestaJson
            );
        }


        $respuestaJson = json_encode(
            [
                "resultado" =>
                    "Medicion almacenada"
            ],
            JSON_UNESCAPED_UNICODE
        );


        return $this->crearRespuesta(
            201,
            $respuestaJson
        );
    }


    // ------------------------------------------------------------
    // getMedicion()
    // --> respuesta: RespuestaREST
    // ------------------------------------------------------------
    //
    // Solicita al componente business_logic la última
    // medición almacenada.
    //
    // Si existe, la convierte a JSON.
    // ------------------------------------------------------------
    private function getMedicion(): array
    {
        $medicion =
            $this->logica->mostrarDatos();


        if ($medicion === null) {

            $respuestaJson = json_encode(
                [
                    "error" =>
                        "No hay mediciones"
                ],
                JSON_UNESCAPED_UNICODE
            );


            return $this->crearRespuesta(
                404,
                $respuestaJson
            );
        }


        $respuestaJson = json_encode(
            $medicion,
            JSON_UNESCAPED_UNICODE
        );


        return $this->crearRespuesta(
            200,
            $respuestaJson
        );
    }


    // ------------------------------------------------------------
    // codigo: N,
    // json: Text
    // --> crearRespuesta()
    // --> respuesta: RespuestaREST
    // ------------------------------------------------------------
    //
    // Construye una respuesta REST formada por un código
    // HTTP y un cuerpo JSON.
    // ------------------------------------------------------------
    private function crearRespuesta(
        int $codigo,
        string $json
    ): array {

        return [
            "codigo" => $codigo,
            "json" => $json
        ];
    }
}