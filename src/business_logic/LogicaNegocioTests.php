<?php

// ------------------------------------------------------------
// Archivo: LogicaNegocioTests.php
//
// Descripción:
// Pruebas automatizadas del componente business_logic.
//
// Comprueba:
// - almacenamiento de una Medicion mediante leerDatos();
// - recuperación mediante mostrarDatos();
// - correspondencia con el nuevo modelo de MEDICIONES.
//
// Autor: Élia Garcia Pons
// Fecha: 13/10/2026
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
// ------------------------------------------------------------


require_once __DIR__ . "/LogicaNegocio.php";


// ------------------------------------------------------------
// Configuración de la base de datos de pruebas.
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
// condicion: B,
// mensaje: Text
// --> comprobar() -->
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
// Crear conexión con la base de datos.
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
// Se utiliza una fecha futura únicamente para garantizar
// que mostrarDatos() la recupere como última medición.
// ------------------------------------------------------------

$medicion =
    new stdClass();


$medicion->valor =
    1234;

$medicion->fecha_hora =
    "2099-12-31 23:59:59";

$medicion->identificador_sensor =
    "00000000-0000-0000-0000-000000000001";

$medicion->tipo =
    11;


// ------------------------------------------------------------
// Limpiar una posible ejecución anterior del mismo test.
// ------------------------------------------------------------

$consultaLimpieza =
    $conexion->prepare(
        "
        DELETE FROM MEDICIONES
        WHERE fecha_hora = :fecha_hora
        AND identificador_sensor = :identificador_sensor
        "
    );


$consultaLimpieza->execute([
    ':fecha_hora' =>
        $medicion->fecha_hora,

    ':identificador_sensor' =>
        $medicion->identificador_sensor
]);


// ============================================================
// TEST 1
//
// Comprobar que la Medicion de prueba es un objeto.
// ============================================================

comprobar(
    is_object($medicion),
    "La Medicion de prueba es un objeto"
);


// ============================================================
// TEST 2
//
// medicion: Medicion
// --> leerDatos()
// --> resultado: B
//
// Comprobar que la medición se almacena correctamente.
// ============================================================

$resultado =
    $logica->leerDatos(
        $medicion
    );


comprobar(
    $resultado === true,
    "leerDatos() almacena correctamente una Medicion"
);


// ============================================================
// TEST 3
//
// mostrarDatos()
// --> medicion: Medicion
//
// Comprobar que se recupera una medición.
// ============================================================

$medicionRecuperada =
    $logica->mostrarDatos();


comprobar(
    is_object(
        $medicionRecuperada
    ),
    "mostrarDatos() devuelve una Medicion"
);


// ============================================================
// TEST 4
//
// Comprobar valor.
// ============================================================

comprobar(
    (int) $medicionRecuperada->valor
        ===
    $medicion->valor,

    "Valor correcto"
);


// ============================================================
// TEST 5
//
// Comprobar fecha_hora.
// ============================================================

comprobar(
    $medicionRecuperada->fecha_hora
        ===
    $medicion->fecha_hora,

    "Fecha y hora correctas"
);


// ============================================================
// TEST 6
//
// Comprobar identificador_sensor.
// ============================================================

comprobar(
    $medicionRecuperada->identificador_sensor
        ===
    $medicion->identificador_sensor,

    "Identificador del sensor correcto"
);


// ============================================================
// TEST 7
//
// Comprobar tipo.
// ============================================================

comprobar(
    (int) $medicionRecuperada->tipo
        ===
    $medicion->tipo,

    "Tipo correcto"
);


// ------------------------------------------------------------
// Limpiar la medición utilizada por los tests.
// ------------------------------------------------------------

$consultaLimpieza->execute([
    ':fecha_hora' =>
        $medicion->fecha_hora,

    ':identificador_sensor' =>
        $medicion->identificador_sensor
]);


echo PHP_EOL;

echo
    "TODOS LOS TESTS DE BUSINESS_LOGIC "
    . "HAN FINALIZADO CORRECTAMENTE."
    . PHP_EOL;