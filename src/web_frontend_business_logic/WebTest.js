const {
    mostrarUltimaMedicion
} = require("./app.js");


// ------------------------------------------------------------
// Función auxiliar para los tests.
// ------------------------------------------------------------

function comprobar(
    condicion,
    mensaje
) {

    if (!condicion) {

        throw new Error(
            "TEST FALLIDO: " + mensaje
        );
    }

    console.log(
        "OK - " + mensaje
    );
}


// ------------------------------------------------------------
// Elemento HTML falso.
//
// Es suficiente para comprobar los valores escritos por
// mostrarUltimaMedicion() sin necesitar un navegador real.
// ------------------------------------------------------------

function crearElemento() {

    return {
        textContent: "",
        hidden: false
    };
}


// ------------------------------------------------------------
// Ejecutar tests.
// ------------------------------------------------------------

async function ejecutarTests() {

    // --------------------------------------------------------
    // Lógica fake controlada para probar exclusivamente
    // la interfaz.
    // --------------------------------------------------------

    const logicaFake = {

        async mostrarDatos() {

            return {

                uuid:
                    "45505347-2D47-5449-2D50-524F592D3341",

                fecha:
                    "2026-10-02 16:30:00",

                major: 40,

                minor: 0,

                TxPower: -53
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
    // TEST 1 - Fecha
    // --------------------------------------------------------

    comprobar(
        elementos.fecha.textContent ===
            "2026-10-02 16:30:00",

        "La Fecha se muestra correctamente"
    );


    // --------------------------------------------------------
    // TEST 2 - Major
    // --------------------------------------------------------

    comprobar(
        elementos.major.textContent === 40,

        "Major se muestra correctamente"
    );


    // --------------------------------------------------------
    // TEST 3 - Minor
    // --------------------------------------------------------

    comprobar(
        elementos.minor.textContent === 0,

        "Minor se muestra correctamente"
    );


    // --------------------------------------------------------
    // TEST 4 - TxPower
    // --------------------------------------------------------

    comprobar(
        elementos.txPower.textContent === -53,

        "TxPower se muestra correctamente"
    );


    // --------------------------------------------------------
    // TEST 5
    // Comprobar que mostrarDatos() no ha producido error.
    // --------------------------------------------------------

    comprobar(
        elementos.error.hidden === true,

        "La interfaz muestra la Medicion sin errores"
    );


    console.log("");

    console.log(
        "TODOS LOS TESTS DE LA INTERFAZ WEB "
        + "HAN FINALIZADO CORRECTAMENTE."
    );
}


ejecutarTests();