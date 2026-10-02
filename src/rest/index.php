<?php

require_once __DIR__ . "/ServidorREST.php";
require_once __DIR__ . "/../logica_negocio/LogicaNegocio.php";


// ------------------------------------------------------------
// Configuración de la conexión con la base de datos.
// ------------------------------------------------------------

$host = "localhost";
$baseDatos = "biometria_sprint0";
$usuario = "root";
$contrasenya = "";

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
// Crear la lógica de negocio y el servidor REST.
// ------------------------------------------------------------

$logica = new LogicaNegocio($conexion);

$servidor = new ServidorREST($logica);


// ------------------------------------------------------------
// Obtener los datos de la petición HTTP.
// ------------------------------------------------------------

$metodo = $_SERVER["REQUEST_METHOD"];

$ruta = parse_url(
    $_SERVER["REQUEST_URI"],
    PHP_URL_PATH
);

$cuerpo = file_get_contents("php://input");


// ------------------------------------------------------------
// Procesar la petición.
// ------------------------------------------------------------

$respuesta = $servidor->manejarPeticion(
    $metodo,
    $ruta,
    $cuerpo
);


// ------------------------------------------------------------
// Enviar la respuesta al cliente en formato JSON.
// ------------------------------------------------------------

http_response_code(
    $respuesta["codigo"]
);

header(
    "Content-Type: application/json; charset=utf-8"
);

echo $respuesta["json"];