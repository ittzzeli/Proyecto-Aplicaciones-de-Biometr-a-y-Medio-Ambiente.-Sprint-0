// ------------------------------------------------------------
// Archivo: WebTest.js
//
// Descripción:
// Pruebas automatizadas de la interfaz gráfica web.
//
// Comprueba que mostrarUltimaMedicion():
// - solicita los datos mediante la lógica fake;
// - muestra correctamente la medición;
// - muestra un mensaje cuando ocurre un error.
//
// Estas pruebas no realizan peticiones HTTP.
//
// Autor: Élia Garcia Pons
// Fecha: 13/10/2026
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
// ------------------------------------------------------------


const {
    mostrarUltimaMedicion
} = require("./app.js");


// ------------------------------------------------------------
// condicion: B,
// mensaje: Text
// --> comprobar() -->
// ------------------------------------------------------------
//
// Comprueba el resultado de un test.
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
// crearElemento() --> elemento: ElementoGUI
// ------------------------------------------------------------
//
// Crea un elemento HTML falso para probar la interfaz
// sin necesitar un navegador real.
// ------------------------------------------------------------
function crearElemento() {

    return {
        textContent: "",
        hidden: false
    };
}


// ------------------------------------------------------------
// ejecutarTests() -->
// ------------------------------------------------------------
//
// Ejecuta las pruebas automatizadas de la interfaz web.
// ------------------------------------------------------------
async function ejecutarTests() {


    // ========================================================
    // PRUEBAS CON UNA MEDICION CORRECTA
    // ========================================================


    // --------------------------------------------------------
    // mostrarDatos() --> medicion: MedicionAlmacenada
    // --------------------------------------------------------
    //
    // Lógica fake controlada para probar exclusivamente
    // la interfaz gráfica.
    // --------------------------------------------------------
    const logicaFake = {

        async mostrarDatos() {

            return {

                id:
                    1,

                uuid:
                    "45505347-2D47-5449-2D50-524F592D3341",

                fecha:
                    "2026-10-13 10:00:00",

                major:
                    2821,

                minor:
                    1234,

                TxPower:
                    -53
            };
        }
    };


    const elementos = {

        fecha:
            crearElemento(),

        major:
            crearElemento(),

        minor:
            crearElemento(),

        txPower:
            crearElemento(),

        error:
            crearElemento()
    };


    await mostrarUltimaMedicion(
        logicaFake,
        elementos
    );


    // --------------------------------------------------------
    // TEST 1
    // --------------------------------------------------------

    comprobar(
        elementos.fecha.textContent
            ===
        "2026-10-13 10:00:00",

        "La Fecha se muestra correctamente"
    );


    // --------------------------------------------------------
    // TEST 2
    // --------------------------------------------------------

    comprobar(
        elementos.major.textContent
            ===
        2821,

        "Major se muestra correctamente"
    );


    // --------------------------------------------------------
    // TEST 3
    // --------------------------------------------------------

    comprobar(
        elementos.minor.textContent
            ===
        1234,

        "Minor se muestra correctamente"
    );


    // --------------------------------------------------------
    // TEST 4
    // --------------------------------------------------------

    comprobar(
        elementos.txPower.textContent
            ===
        -53,

        "TxPower se muestra correctamente"
    );


    // --------------------------------------------------------
    // TEST 5
    // --------------------------------------------------------

    comprobar(
        elementos.error.hidden
            ===
        true,

        "La interfaz muestra la Medicion sin errores"
    );


    // ========================================================
    // PRUEBA DE ERROR
    // ========================================================


    const logicaFakeError = {

        async mostrarDatos() {

            throw new Error(
                "Error simulado"
            );
        }
    };


    const elementosError = {

        fecha:
            crearElemento(),

        major:
            crearElemento(),

        minor:
            crearElemento(),

        txPower:
            crearElemento(),

        error:
            crearElemento()
    };


    await mostrarUltimaMedicion(
        logicaFakeError,
        elementosError
    );


    // --------------------------------------------------------
    // TEST 6
    // --------------------------------------------------------

    comprobar(
        elementosError.error.hidden
            ===
        false,

        "La interfaz muestra el error cuando falla mostrarDatos()"
    );


    // --------------------------------------------------------
    // TEST 7
    // --------------------------------------------------------

    comprobar(
        elementosError.error.textContent
            ===
        "No se pudo obtener la última medición.",

        "El mensaje de error es correcto"
    );


    console.log("");

    console.log(
        "TODOS LOS TESTS DE LA INTERFAZ WEB "
            + "HAN FINALIZADO CORRECTAMENTE."
    );
}


// ------------------------------------------------------------
// Ejecutar las pruebas.
// ------------------------------------------------------------

ejecutarTests();