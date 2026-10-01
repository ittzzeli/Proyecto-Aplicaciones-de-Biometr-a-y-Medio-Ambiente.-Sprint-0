<?php

require_once "LogicaNegocio.php";


// ------------------------------------------------------------
// Configuración de la base de datos de pruebas.
// ------------------------------------------------------------

$host = "localhost";
$baseDatos = "biometria_sprint0";
$usuario = "root";
$contrasenya = "";


// ------------------------------------------------------------
// Función auxiliar para comprobar los tests.
// ------------------------------------------------------------
function comprobar(bool $condicion, string $mensaje): void
{
    if (!$condicion) {
        throw new Exception("TEST FALLIDO: " . $mensaje);
    }

    echo "OK - " . $mensaje . PHP_EOL;
}


// ------------------------------------------------------------
// Conexión con la base de datos.
// ------------------------------------------------------------

$conexion = new PDO(
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

$logica = new LogicaNegocio($conexion);


// ------------------------------------------------------------
// TEST 1
// comprobar que leerDatos() recibe una Medicion.
// ------------------------------------------------------------

$json = '{
    "uuid": "45505347-2D47-5449-2D50-524F592D3341",
    "fecha": "2026-10-01 18:00:00",
    "major": 10,
    "minor": 0,
    "TxPower": -53
}';

$medicion = json_decode($json);

comprobar(
    is_object($medicion),
    "La Medicion JSON se convierte correctamente en un objeto"
);


// ------------------------------------------------------------
// TEST 2
// comprobar que leerDatos() almacena los datos.
// ------------------------------------------------------------

$resultado = $logica->leerDatos($medicion);

comprobar(
    $resultado === true,
    "leerDatos() almacena correctamente una Medicion"
);


// ------------------------------------------------------------
// TEST 3
// comprobar que mostrarDatos() devuelve una Medicion.
// ------------------------------------------------------------

$medicionRecuperada = $logica->mostrarDatos();

comprobar(
    is_object($medicionRecuperada),
    "mostrarDatos() devuelve una Medicion"
);


// ------------------------------------------------------------
// TEST 4
// comprobar que se mantienen todos los datos.
// ------------------------------------------------------------

comprobar(
    $medicionRecuperada->uuid === $medicion->uuid,
    "UUID correcto"
);

comprobar(
    $medicionRecuperada->fecha === $medicion->fecha,
    "Fecha correcta"
);

comprobar(
    (int) $medicionRecuperada->major === $medicion->major,
    "Major correcto"
);

comprobar(
    (int) $medicionRecuperada->minor === $medicion->minor,
    "Minor correcto"
);

comprobar(
    (int) $medicionRecuperada->TxPower === $medicion->TxPower,
    "TxPower correcto"
);


echo PHP_EOL;
echo "TODOS LOS TESTS HAN FINALIZADO CORRECTAMENTE." . PHP_EOL;