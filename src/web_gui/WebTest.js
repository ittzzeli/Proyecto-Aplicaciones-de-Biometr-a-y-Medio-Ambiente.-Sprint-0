// ------------------------------------------------------------
// Archivo: WebTest.js
//
// Descripción:
// Pruebas automatizadas de la interfaz web.
//
// Comprueba:
// - que mostrarUltimaMedicion() solicita los datos;
// - que muestra valor;
// - que muestra fecha_hora;
// - que muestra identificador_sensor;
// - que muestra tipo;
// - que muestra un mensaje si ocurre un error.
//
// Autor: Élia Garcia Pons
// Fecha: 14/10/2026
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
// ------------------------------------------------------------


const {
    mostrarUltimaMedicion
} =
    require("./app.js");


// ------------------------------------------------------------
// condicion: B,
// mensaje: Text
// --> comprobar() -->
// ------------------------------------------------------------
function comprobar(
    condicion,
    mensaje
) {

    if (!condicion) {

        throw new Error(
            "TEST FALLIDO: "
            + mensaje
        );
    }


    console.log(
        "OK - "
        + mensaje
    );
}


// ------------------------------------------------------------
// crearElementoFake()
// --> elemento: ElementoGUI
// ------------------------------------------------------------
//
// Crea un elemento sencillo con textContent para simular
// un elemento HTML durante los tests.
// ------------------------------------------------------------
function crearElementoFake() {

    return {
        textContent: ""
    };
}


// ------------------------------------------------------------
// testMostrarMedicion() -->
// ------------------------------------------------------------
//
// Comprueba que los datos de una Medicion se muestran
// correctamente en la interfaz.
// ------------------------------------------------------------
async function testMostrarMedicion() {

    const logicaFake = {

        async mostrarDatos() {

            return {

                valor:
                    1234,

                fecha_hora:
                    "2026-10-14 09:06:41",

                identificador_sensor:
                    "45505347-2D47-5449-2D50-524F592D3341",

                tipo:
                    11
            };
        }
    };


    const elementos = {

        valor:
            crearElementoFake(),

        fechaHora:
            crearElementoFake(),

        identificadorSensor:
            crearElementoFake(),

        tipo:
            crearElementoFake(),

        error:
            crearElementoFake()
    };


    await mostrarUltimaMedicion(
        logicaFake,
        elementos
    );


    comprobar(
        elementos.valor.textContent
            ===
        1234,

        "La web muestra el valor"
    );


    comprobar(
        elementos.fechaHora.textContent
            ===
        "2026-10-14 09:06:41",

        "La web muestra la fecha y hora"
    );


    comprobar(
        elementos.identificadorSensor.textContent
            ===
        "45505347-2D47-5449-2D50-524F592D3341",

        "La web muestra el identificador del sensor"
    );


    comprobar(
        elementos.tipo.textContent
            ===
        11,

        "La web muestra el tipo"
    );


    comprobar(
        elementos.error.textContent
            ===
        "",

        "La web no muestra error si la medicion es correcta"
    );
}


// ------------------------------------------------------------
// testMostrarError() -->
// ------------------------------------------------------------
//
// Comprueba que la interfaz informa cuando
// LogicaFakeNavegador no puede recuperar la medición.
// ------------------------------------------------------------
async function testMostrarError() {

    const logicaFake = {

        async mostrarDatos() {

            throw new Error(
                "Error simulado"
            );
        }
    };


    const elementos = {

        valor:
            crearElementoFake(),

        fechaHora:
            crearElementoFake(),

        identificadorSensor:
            crearElementoFake(),

        tipo:
            crearElementoFake(),

        error:
            crearElementoFake()
    };


    await mostrarUltimaMedicion(
        logicaFake,
        elementos
    );


    comprobar(
        elementos.error.textContent
            ===
        "No se ha podido recuperar la última medición.",

        "La web muestra un mensaje de error"
    );
}


// ------------------------------------------------------------
// ejecutarTests() -->
// ------------------------------------------------------------
//
// Ejecuta todas las pruebas de la interfaz web.
// ------------------------------------------------------------
async function ejecutarTests() {

    await testMostrarMedicion();

    await testMostrarError();


    console.log("");

    console.log(
        "TODOS LOS TESTS DE WEB_GUI "
        + "HAN FINALIZADO CORRECTAMENTE."
    );
}


// ------------------------------------------------------------
// Ejecutar tests.
// ------------------------------------------------------------

ejecutarTests()
    .catch(
        error => {

            console.error(
                error
            );

            process.exit(
                1
            );
        }
    );