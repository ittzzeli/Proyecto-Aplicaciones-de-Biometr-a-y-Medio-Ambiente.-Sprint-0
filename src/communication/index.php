<?php

// ------------------------------------------------------------
// Archivo: index.php
//
// Descripción:
// Punto de entrada HTTP del componente communication.
//
// Obtiene los datos de la petición HTTP, construye las
// dependencias necesarias y delega el procesamiento de
// la petición en ServidorREST.
//
// Este archivo no contiene consultas SQL ni lógica de negocio.
//
// Autor: Elia
// Fecha: 08/10/2026
//
// Aportación:
// Adaptación del punto de entrada REST para mantener la
// separación entre communication y business_logic.
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
// ------------------------------------------------------------


require_once __DIR__ . "/ServidorREST.php";

require_once __DIR__
    . "/../business_logic/LogicaNegocio.php";


// ------------------------------------------------------------
// Configuración de la conexión con la base de datos.
//
// La conexión se crea aquí únicamente para construir
// LogicaNegocio.
//
// Las operaciones sobre la tabla Medicion siguen siendo
// responsabilidad del componente business_logic.
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
// Crear la conexión utilizada por business_logic.
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
// Construir los componentes.
//
// Flujo de dependencias:
//
// communication
//      |
//      v
// business_logic
//      |
//      v
// database
//
// ServidorREST recibe LogicaNegocio como dependencia.
// ------------------------------------------------------------

$logica =
    new LogicaNegocio(
        $conexion
    );


$servidor =
    new ServidorREST(
        $logica
    );


// ------------------------------------------------------------
// Obtener los datos de la petición HTTP.
// ------------------------------------------------------------

$metodo =
    $_SERVER["REQUEST_METHOD"];


$ruta =
    parse_url(
        $_SERVER["REQUEST_URI"],
        PHP_URL_PATH
    );


$cuerpo =
    file_get_contents(
        "php://input"
    );


// ------------------------------------------------------------
// Delegar el procesamiento de la petición en ServidorREST.
// ------------------------------------------------------------

$respuesta =
    $servidor->manejarPeticion(
        $metodo,
        $ruta,
        $cuerpo
    );


// ------------------------------------------------------------
// Enviar la respuesta HTTP al cliente.
//
// ServidorREST ya ha decidido:
// - código HTTP;
// - cuerpo JSON.
//
// index.php únicamente envía esa respuesta.
// ------------------------------------------------------------

http_response_code(
    $respuesta["codigo"]
);


header(
    "Content-Type: application/json; charset=utf-8"
);


echo $respuesta["json"];