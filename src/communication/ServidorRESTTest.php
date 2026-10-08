<?php

require_once "ServidorREST.php";


// ------------------------------------------------------------
// Lógica de negocio falsa utilizada exclusivamente
// durante los tests del servidor REST.
// ------------------------------------------------------------

class LogicaNegocioFake
{
    public int $vecesLeerDatos = 0;

    public int $vecesMostrarDatos = 0;

    public ?object $ultimaMedicion = null;


    // ------------------------------------------------------------
    // medicion: Medicion --> leerDatos() --> resultado: B
    // ------------------------------------------------------------
    public function leerDatos(object $medicion): bool
    {
        $this->vecesLeerDatos++;

        $this->ultimaMedicion =
            $medicion;

        return true;
    }


    // ------------------------------------------------------------
    // mostrarDatos() --> medicion: Medicion
    // ------------------------------------------------------------
    public function mostrarDatos(): ?object
    {
        $this->vecesMostrarDatos++;

        return $this->ultimaMedicion;
    }
}


// ------------------------------------------------------------
// Función auxiliar para los tests.
// ------------------------------------------------------------

function comprobar(
    bool $condicion,
    string $mensaje
): void {

    if (!$condicion) {

        throw new Exception(
            "TEST FALLIDO: " . $mensaje
        );
    }

    echo "OK - "
        . $mensaje
        . PHP_EOL;
}


// ------------------------------------------------------------
// Preparar servidor de pruebas.
// ------------------------------------------------------------

$logica =
    new LogicaNegocioFake();

$servidor =
    new ServidorREST($logica);


// ============================================================
// TEST 1
// POST /medicion con datos correctos.
// ============================================================

$jsonCorrecto = json_encode([
    "uuid" =>
        "45505347-2D47-5449-2D50-524F592D3341",

    "fecha" =>
        "2026-10-01 19:00:00",

    "major" => 20,

    "minor" => 0,

    "TxPower" => -53
]);


$respuesta =
    $servidor->manejarPeticion(
        "POST",
        "/medicion",
        $jsonCorrecto
    );


comprobar(
    $respuesta["codigo"] === 201,
    "POST /medicion acepta datos correctos"
);


// ============================================================
// TEST 2
// Comprobar que POST utiliza leerDatos().
// ============================================================

comprobar(
    $logica->vecesLeerDatos === 1,
    "POST /medicion utiliza leerDatos()"
);


// ============================================================
// TEST 3
// Comprobar que la respuesta del POST es JSON.
// ============================================================

comprobar(
    json_decode(
        $respuesta["json"]
    ) !== null,

    "POST /medicion devuelve JSON"
);


// ============================================================
// TEST 4
// POST /medicion con datos incorrectos.
// ============================================================

$jsonIncorrecto = json_encode([
    "uuid" => "UUID-INCOMPLETO",
    "major" => 20
]);


$respuestaIncorrecta =
    $servidor->manejarPeticion(
        "POST",
        "/medicion",
        $jsonIncorrecto
    );


comprobar(
    $respuestaIncorrecta["codigo"] === 400,
    "POST /medicion rechaza datos incorrectos"
);


// Comprobar que leerDatos() NO se ejecutó otra vez.
comprobar(
    $logica->vecesLeerDatos === 1,
    "POST incorrecto no utiliza leerDatos()"
);


// ============================================================
// TEST 5
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
// TEST 6
// Comprobar que GET utiliza mostrarDatos().
// ============================================================

comprobar(
    $logica->vecesMostrarDatos === 1,
    "GET /medicion utiliza mostrarDatos()"
);


// ============================================================
// TEST 7
// Comprobar que GET devuelve JSON.
// ============================================================

$medicionRecuperada =
    json_decode(
        $respuestaGet["json"]
    );


comprobar(
    is_object($medicionRecuperada),
    "GET /medicion devuelve JSON"
);


// ============================================================
// TEST 8
// Comprobar los datos devueltos.
// ============================================================

comprobar(
    $medicionRecuperada->uuid ===
        "45505347-2D47-5449-2D50-524F592D3341",

    "UUID correcto"
);


comprobar(
    $medicionRecuperada->fecha ===
        "2026-10-01 19:00:00",

    "Fecha correcta"
);


comprobar(
    $medicionRecuperada->major === 20,
    "Major correcto"
);


comprobar(
    $medicionRecuperada->minor === 0,
    "Minor correcto"
);


comprobar(
    $medicionRecuperada->TxPower === -53,
    "TxPower correcto"
);


echo PHP_EOL;

echo
    "TODOS LOS TESTS DEL SERVIDOR REST "
    . "HAN FINALIZADO CORRECTAMENTE."
    . PHP_EOL;