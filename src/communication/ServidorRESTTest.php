<?php

// ------------------------------------------------------------
// Archivo: ServidorRESTTest.php
//
// Descripción:
// Pruebas automatizadas del componente communication.
//
// Comprueba las rutas REST del Sprint 0 utilizando una
// lógica de negocio falsa para evitar depender de MySQL.
//
// Autor: Elia
// Fecha: 08/10/2026
//
// Aportación:
// Pruebas de integración de ServidorREST con una
// implementación falsa de business_logic.
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
// ------------------------------------------------------------


require_once __DIR__ . "/ServidorREST.php";


// ------------------------------------------------------------
// LogicaNegocioFake
//
// Responsabilidad:
// Simular el componente business_logic durante los tests.
//
// Permite comprobar que communication delega correctamente
// en leerDatos() y mostrarDatos() sin utilizar una base de
// datos real.
// ------------------------------------------------------------
class LogicaNegocioFake
{
    public int $vecesLeerDatos = 0;

    public int $vecesMostrarDatos = 0;

    public ?object $ultimaMedicion = null;

    public bool $resultadoLeerDatos = true;


    // ------------------------------------------------------------
    // medicion: Medicion --> leerDatos() --> resultado: B
    // ------------------------------------------------------------
    //
    // Simula el almacenamiento de una medición.
    // ------------------------------------------------------------
    public function leerDatos(
        object $medicion
    ): bool {

        $this->vecesLeerDatos++;

        $this->ultimaMedicion =
            $medicion;

        return $this->resultadoLeerDatos;
    }


    // ------------------------------------------------------------
    // mostrarDatos() --> medicion: MedicionAlmacenada
    // ------------------------------------------------------------
    //
    // Simula la recuperación de la última medición.
    // ------------------------------------------------------------
    public function mostrarDatos(): ?object
    {
        $this->vecesMostrarDatos++;

        return $this->ultimaMedicion;
    }
}


// ------------------------------------------------------------
// condicion: B, mensaje: Text --> comprobar() -->
// ------------------------------------------------------------
//
// Comprueba el resultado de un test.
//
// Si la condición es falsa lanza una excepción.
// ------------------------------------------------------------
function comprobar(
    bool $condicion,
    string $mensaje
): void {

    if (!$condicion) {

        throw new Exception(
            "TEST FALLIDO: "
            . $mensaje
        );
    }


    echo
        "OK - "
        . $mensaje
        . PHP_EOL;
}


// ------------------------------------------------------------
// Preparar el servidor de pruebas.
// ------------------------------------------------------------

$logica =
    new LogicaNegocioFake();


$servidor =
    new ServidorREST(
        $logica
    );


// ============================================================
// TEST 1
//
// POST /medicion con una Medicion correcta.
// ============================================================

$jsonCorrecto =
    json_encode([
        "uuid" =>
            "45505347-2D47-5449-2D50-524F592D3341",

        "fecha" =>
            "2026-10-08 16:00:00",

        "major" =>
            2821,

        "minor" =>
            1234,

        "TxPower" =>
            -53
    ]);


$respuesta =
    $servidor->manejarPeticion(
        "POST",
        "/medicion",
        $jsonCorrecto
    );


comprobar(
    $respuesta["codigo"] === 201,
    "POST /medicion acepta una Medicion correcta"
);


// ============================================================
// TEST 2
//
// Comprobar que POST /medicion delega en leerDatos().
// ============================================================

comprobar(
    $logica->vecesLeerDatos === 1,
    "POST /medicion utiliza leerDatos()"
);


// ============================================================
// TEST 3
//
// Comprobar que POST /medicion devuelve JSON.
// ============================================================

$contenidoPost =
    json_decode(
        $respuesta["json"]
    );


comprobar(
    is_object($contenidoPost),
    "POST /medicion devuelve JSON valido"
);


// ============================================================
// TEST 4
//
// Comprobar que communication transforma correctamente
// el JSON recibido en una Medicion.
// ============================================================

comprobar(
    $logica->ultimaMedicion !== null,
    "POST /medicion entrega una Medicion a business_logic"
);


comprobar(
    $logica->ultimaMedicion->uuid
        ===
    "45505347-2D47-5449-2D50-524F592D3341",

    "UUID recibido correctamente"
);


comprobar(
    $logica->ultimaMedicion->major === 2821,
    "Major recibido correctamente"
);


comprobar(
    $logica->ultimaMedicion->minor === 1234,
    "Minor recibido correctamente"
);


comprobar(
    $logica->ultimaMedicion->TxPower === -53,
    "TxPower recibido correctamente"
);


// ============================================================
// TEST 5
//
// POST /medicion con una Medicion incompleta.
// ============================================================

$jsonIncompleto =
    json_encode([
        "uuid" =>
            "UUID-INCOMPLETO",

        "major" =>
            2821
    ]);


$respuestaIncompleta =
    $servidor->manejarPeticion(
        "POST",
        "/medicion",
        $jsonIncompleto
    );


comprobar(
    $respuestaIncompleta["codigo"] === 400,
    "POST /medicion rechaza una Medicion incompleta"
);


// ------------------------------------------------------------
// leerDatos() no debe haberse ejecutado nuevamente.
// ------------------------------------------------------------

comprobar(
    $logica->vecesLeerDatos === 1,
    "POST incorrecto no utiliza leerDatos()"
);


// ============================================================
// TEST 6
//
// POST /medicion con JSON incorrecto.
// ============================================================

$respuestaJsonIncorrecto =
    $servidor->manejarPeticion(
        "POST",
        "/medicion",
        "{esto no es json}"
    );


comprobar(
    $respuestaJsonIncorrecto["codigo"] === 400,
    "POST /medicion rechaza JSON incorrecto"
);


// ============================================================
// TEST 7
//
// GET /medicion.
// ============================================================

$respuestaGet =
    $servidor->manejarPeticion(
        "GET",
        "/medicion"
    );


comprobar(
    $respuestaGet["codigo"] === 200,
    "GET /medicion devuelve una Medicion"
);


// ============================================================
// TEST 8
//
// Comprobar que GET /medicion delega en mostrarDatos().
// ============================================================

comprobar(
    $logica->vecesMostrarDatos === 1,
    "GET /medicion utiliza mostrarDatos()"
);


// ============================================================
// TEST 9
//
// Comprobar que GET /medicion devuelve JSON válido.
// ============================================================

$medicionRecuperada =
    json_decode(
        $respuestaGet["json"]
    );


comprobar(
    is_object($medicionRecuperada),
    "GET /medicion devuelve JSON valido"
);


// ============================================================
// TEST 10
//
// Comprobar los datos devueltos por GET /medicion.
// ============================================================

comprobar(
    $medicionRecuperada->uuid
        ===
    "45505347-2D47-5449-2D50-524F592D3341",

    "UUID correcto"
);


comprobar(
    $medicionRecuperada->fecha
        ===
    "2026-10-08 16:00:00",

    "Fecha correcta"
);


comprobar(
    $medicionRecuperada->major === 2821,
    "Major correcto"
);


comprobar(
    $medicionRecuperada->minor === 1234,
    "Minor correcto"
);


comprobar(
    $medicionRecuperada->TxPower === -53,
    "TxPower correcto"
);


// ============================================================
// TEST 11
//
// GET /medicion cuando no existe ninguna medición.
// ============================================================

$logica->ultimaMedicion =
    null;


$respuestaSinMediciones =
    $servidor->manejarPeticion(
        "GET",
        "/medicion"
    );


comprobar(
    $respuestaSinMediciones["codigo"] === 404,
    "GET /medicion devuelve 404 si no hay mediciones"
);


// ============================================================
// TEST 12
//
// POST /medicion cuando business_logic no puede almacenar.
// ============================================================

$logica->resultadoLeerDatos =
    false;


$respuestaErrorNegocio =
    $servidor->manejarPeticion(
        "POST",
        "/medicion",
        $jsonCorrecto
    );


comprobar(
    $respuestaErrorNegocio["codigo"] === 500,
    "POST /medicion devuelve 500 si leerDatos() falla"
);


// ============================================================
// TEST 13
//
// Ruta inexistente.
// ============================================================

$respuestaRutaIncorrecta =
    $servidor->manejarPeticion(
        "GET",
        "/ruta-inexistente"
    );


comprobar(
    $respuestaRutaIncorrecta["codigo"] === 404,
    "Una ruta inexistente devuelve 404"
);


// ============================================================
// Resultado final.
// ============================================================

echo PHP_EOL;

echo
    "TODOS LOS TESTS DEL SERVIDOR REST "
    . "HAN FINALIZADO CORRECTAMENTE."
    . PHP_EOL;