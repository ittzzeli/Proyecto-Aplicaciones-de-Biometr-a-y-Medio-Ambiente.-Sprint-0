<?php

// ------------------------------------------------------------
// ServidorREST
//
// Responsabilidad:
// Recibir las peticiones REST y delegar el trabajo
// en la lógica de negocio.
//
// Este componente NO accede directamente a la base de datos.
// ------------------------------------------------------------

class ServidorREST
{
    private object $logica;


    // ------------------------------------------------------------
    // logica: LogicaNegocio --> ServidorREST()
    // ------------------------------------------------------------
    public function __construct(object $logica)
    {
        $this->logica = $logica;
    }


    // ------------------------------------------------------------
    // metodo: Text, ruta: Text, cuerpo: Text
    // --> manejarPeticion()
    // --> respuesta
    // ------------------------------------------------------------
    //
    // Gestiona únicamente:
    //
    // POST /medicion
    // GET  /medicion
    // ------------------------------------------------------------
    public function manejarPeticion(
        string $metodo,
        string $ruta,
        string $cuerpo = ""
    ): array {

        if ($metodo === "POST" && $ruta === "/medicion") {
            return $this->postMedicion($cuerpo);
        }

        if ($metodo === "GET" && $ruta === "/medicion") {
            return $this->getMedicion();
        }

        return $this->crearRespuesta(
            404,
            [
                "error" => "Ruta no encontrada"
            ]
        );
    }


    // ------------------------------------------------------------
    // json: Text --> postMedicion() --> respuesta
    // ------------------------------------------------------------
    //
    // Convierte el JSON recibido en una Medicion
    // y delega su almacenamiento en leerDatos().
    // ------------------------------------------------------------
    private function postMedicion(string $json): array
    {
        $medicion = json_decode($json);


        // Comprobar que el JSON representa un objeto.
        if (!is_object($medicion)) {

            return $this->crearRespuesta(
                400,
                [
                    "error" => "JSON incorrecto"
                ]
            );
        }


        // Comprobar que existen exactamente los datos
        // necesarios para una Medicion.
        if (
            !isset($medicion->uuid) ||
            !isset($medicion->fecha) ||
            !isset($medicion->major) ||
            !isset($medicion->minor) ||
            !isset($medicion->TxPower)
        ) {

            return $this->crearRespuesta(
                400,
                [
                    "error" => "Medicion incorrecta"
                ]
            );
        }


        // El servidor REST no guarda los datos.
        // Delega esa responsabilidad en la lógica de negocio.
        $resultado =
            $this->logica->leerDatos($medicion);


        if (!$resultado) {

            return $this->crearRespuesta(
                500,
                [
                    "error" => "No se pudo guardar la Medicion"
                ]
            );
        }


        return $this->crearRespuesta(
            201,
            [
                "resultado" => "Medicion almacenada"
            ]
        );
    }


    // ------------------------------------------------------------
    // getMedicion() --> respuesta
    // ------------------------------------------------------------
    //
    // Solicita a la lógica de negocio la última Medicion
    // almacenada y la devuelve en formato JSON.
    // ------------------------------------------------------------
    private function getMedicion(): array
    {
        $medicion =
            $this->logica->mostrarDatos();


        if ($medicion === null) {

            return $this->crearRespuesta(
                404,
                [
                    "error" => "No hay mediciones"
                ]
            );
        }


        return $this->crearRespuesta(
            200,
            $medicion
        );
    }


    // ------------------------------------------------------------
    // codigo: N, datos: Object
    // --> crearRespuesta()
    // --> respuesta
    // ------------------------------------------------------------
    //
    // Crea una respuesta preparada para ser enviada como JSON.
    // ------------------------------------------------------------
    private function crearRespuesta(
        int $codigo,
        mixed $datos
    ): array {

        return [
            "codigo" => $codigo,
            "json" => json_encode(
                $datos,
                JSON_UNESCAPED_UNICODE
            )
        ];
    }
}