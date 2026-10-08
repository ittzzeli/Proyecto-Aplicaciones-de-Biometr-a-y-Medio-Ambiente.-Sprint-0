<?php

// ------------------------------------------------------------
// Archivo: LogicaNegocioTests.php
//
// Descripción:
// Pruebas automatizadas del componente business_logic.
//
// Comprueba los métodos principales de la lógica de negocio:
// - leerDatos()
// - mostrarDatos()
//
// Las pruebas trabajan directamente con objetos de dominio
// Medicion, sin utilizar JSON ni mecanismos de comunicación.
//
// Autor: Elia
// Fecha: 08/10/2026
//
// Aportación:
// Pruebas de integración de la lógica de negocio con
// la base de datos.
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
// ------------------------------------------------------------


require_once __DIR__ . "/LogicaNegocio.php";


// ------------------------------------------------------------
// Configuración de conexión a la base de datos de pruebas.
// ------------------------------------------------------------

$host =
    "localhost";

$baseDatos =
    "biometria_sprint0";

$usuario =
    "root";

$contrasenya =
    "";


// ------------------------------------------------------------
// condicion: B, mensaje: Text --> comprobar() -->
// ------------------------------------------------------------
//
// Comprueba el resultado de una prueba.
//
// Si la condición es verdadera muestra un mensaje OK.
// Si es falsa lanza una excepción y detiene los tests.
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
// Crear la conexión con la base de datos.
// ------------------------------------------------------------

$conexion =
    new PDO(
        "mysql:host=$host;dbname=$baseDatos;charset=utf8mb4",
        $usuario,
        $contrasenya
    );


$conexion->setAttribute(
    PDO::ATTR_ERRMODE,
    PDO::ERRMODE_EXCEPTION
);


// ------------------------------------------------------------
// Crear la lógica de negocio.
// ------------------------------------------------------------

$logica =
    new LogicaNegocio(
        $conexion
    );


// ------------------------------------------------------------
// Crear una Medicion de prueba.
//
// Se crea directamente como objeto de dominio.
// No se utiliza JSON porque pertenece a la capa
// de comunicación y no a business_logic.
// ------------------------------------------------------------

$medicion =
    new stdClass();


$medicion->uuid =
    "45505347-2D47-5449-2D50-524F592D3341";

$medicion->fecha =
    "2026-10-08 15:00:00";

$medicion->major =
    2821;

$medicion->minor =
    1234;

$medicion->TxPower =
    -53;


// ------------------------------------------------------------
// TEST 1
//
// Comprueba que la medición de prueba es un objeto.
// ------------------------------------------------------------

comprobar(
    is_object($medicion),
    "La Medicion de prueba es un objeto"
);


// ------------------------------------------------------------
// TEST 2
//
// medicion: Medicion --> leerDatos() --> resultado: B
//
// Comprueba que leerDatos() almacena correctamente
// la medición en la base de datos.
// ------------------------------------------------------------

$resultado =
    $logica->leerDatos(
        $medicion
    );


comprobar(
    $resultado === true,
    "leerDatos() almacena correctamente una Medicion"
);


// ------------------------------------------------------------
// TEST 3
//
// mostrarDatos() --> medicion: MedicionAlmacenada
//
// Comprueba que mostrarDatos() devuelve una medición.
// ------------------------------------------------------------

$medicionRecuperada =
    $logica->mostrarDatos();


comprobar(
    is_object(
        $medicionRecuperada
    ),
    "mostrarDatos() devuelve una Medicion almacenada"
);


// ------------------------------------------------------------
// TEST 4
//
// Comprueba que la medición recuperada contiene
// un identificador generado por la base de datos.
// ------------------------------------------------------------

comprobar(
    isset(
        $medicionRecuperada->id
    )
    &&
    (int) $medicionRecuperada->id > 0,

    "La Medicion recuperada contiene un id valido"
);


// ------------------------------------------------------------
// TEST 5
//
// Comprueba que el UUID recuperado coincide con
// el UUID almacenado.
// ------------------------------------------------------------

comprobar(
    $medicionRecuperada->uuid
        ===
    $medicion->uuid,

    "UUID correcto"
);


// ------------------------------------------------------------
// TEST 6
//
// Comprueba que la fecha recuperada coincide con
// la fecha almacenada.
// ------------------------------------------------------------

comprobar(
    $medicionRecuperada->fecha
        ===
    $medicion->fecha,

    "Fecha correcta"
);


// ------------------------------------------------------------
// TEST 7
//
// Comprueba que el Major recuperado coincide con
// el Major almacenado.
// ------------------------------------------------------------

comprobar(
    (int) $medicionRecuperada->major
        ===
    $medicion->major,

    "Major correcto"
);


// ------------------------------------------------------------
// TEST 8
//
// Comprueba que el Minor recuperado coincide con
// el Minor almacenado.
// ------------------------------------------------------------

comprobar(
    (int) $medicionRecuperada->minor
        ===
    $medicion->minor,

    "Minor correcto"
);


// ------------------------------------------------------------
// TEST 9
//
// Comprueba que TxPower recuperado coincide con
// el valor almacenado.
// ------------------------------------------------------------

comprobar(
    (int) $medicionRecuperada->TxPower
        ===
    $medicion->TxPower,

    "TxPower correcto"
);


// ------------------------------------------------------------
// Resultado final de las pruebas.
// ------------------------------------------------------------

echo PHP_EOL;

echo
    "TODOS LOS TESTS HAN FINALIZADO CORRECTAMENTE."
    . PHP_EOL;