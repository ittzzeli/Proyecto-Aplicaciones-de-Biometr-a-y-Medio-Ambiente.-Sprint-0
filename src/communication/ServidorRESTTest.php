<?php

// ------------------------------------------------------------
// Archivo: ServidorRESTTest.php
//
// Descripción:
// Pruebas automatizadas del componente communication.
//
// Comprueba:
// - POST /medicion correcto;
// - POST con medición incompleta;
// - JSON incorrecto;
// - GET /medicion;
// - delegación en business_logic;
// - respuestas de error;
// - rutas inexistentes.
//
// Autor: Élia Garcia Pons
// Fecha: 13/10/2026
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
// ------------------------------------------------------------


require_once __DIR__ . "/ServidorREST.php";


// ------------------------------------------------------------
// LogicaNegocioFake
//
// Responsabilidad:
// Simular business_logic durante los tests sin utilizar
// una base de datos real.
// ------------------------------------------------------------
class LogicaNegocioFake
{
    public int $vecesLeerDatos = 0;

    public int $vecesMostrarDatos = 0;

    public ?object $ultimaMedicion = null;

    public bool $resultadoLeerDatos = true;


    // ------------------------------------------------------------
    // medicion: Medicion
    // --> leerDatos()
    // --> resultado: B
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
    // mostrarDatos()
    // --> medicion: Medicion
    // ------------------------------------------------------------
    public function mostrarDatos(): ?object
    {
        $this->vecesMostrarDatos++;

        return $this->ultimaMedicion;
    }
}


// ------------------------------------------------------------
// condicion: B,
// mensaje: Text
// --> comprobar() -->
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
// Preparar servidor de pruebas.
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
        "valor" =>
            1234,

        "fecha_hora" =>
            "2026-10-13 17:00:00",

        "identificador_sensor" =>
            "45505347-2D47-5449-2D50-524F592D3341",

        "tipo" =>
            11
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
// Comprobar que POST utiliza leerDatos().
// ============================================================

comprobar(
    $logica->vecesLeerDatos === 1,
    "POST /medicion utiliza leerDatos()"
);


// ============================================================
// TEST 3
//
// Comprobar que POST entrega la Medicion correctamente.
// ============================================================

comprobar(
    $logica->ultimaMedicion !== null,
    "POST /medicion entrega una Medicion a business_logic"
);


comprobar(
    $logica->ultimaMedicion->valor === 1234,
    "Valor recibido correctamente"
);


comprobar(
    $logica->ultimaMedicion->fecha_hora
        ===
    "2026-10-13 17:00:00",

    "Fecha y hora recibidas correctamente"
);


comprobar(
    $logica->ultimaMedicion->identificador_sensor
        ===
    "45505347-2D47-5449-2D50-524F592D3341",

    "Identificador del sensor recibido correctamente"
);


comprobar(
    $logica->ultimaMedicion->tipo === 11,
    "Tipo recibido correctamente"
);


// ============================================================
// TEST 4
//
// Comprobar que POST devuelve JSON.
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
// TEST 5
//
// POST /medicion con una Medicion incompleta.
// ============================================================

$jsonIncompleto =
    json_encode([
        "valor" =>
            1234,

        "tipo" =>
            11
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
// Comprobar que GET utiliza mostrarDatos().
// ============================================================

comprobar(
    $logica->vecesMostrarDatos === 1,
    "GET /medicion utiliza mostrarDatos()"
);


// ============================================================
// TEST 9
//
// Comprobar que GET devuelve JSON válido.
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
// Comprobar los datos devueltos.
// ============================================================

comprobar(
    $medicionRecuperada->valor === 1234,
    "Valor correcto"
);


comprobar(
    $medicionRecuperada->fecha_hora
        ===
    "2026-10-13 17:00:00",

    "Fecha y hora correctas"
);


comprobar(
    $medicionRecuperada->identificador_sensor
        ===
    "45505347-2D47-5449-2D50-524F592D3341",

    "Identificador del sensor correcto"
);


comprobar(
    $medicionRecuperada->tipo === 11,
    "Tipo correcto"
);


// ============================================================
// TEST 11
//
// GET /medicion cuando no existen mediciones.
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
// POST cuando business_logic no puede almacenar.
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


echo PHP_EOL;

echo
    "TODOS LOS TESTS DEL SERVIDOR REST "
    . "HAN FINALIZADO CORRECTAMENTE."
    . PHP_EOL;