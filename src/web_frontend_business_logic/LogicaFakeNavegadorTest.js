// ------------------------------------------------------------
// Archivo: LogicaFakeNavegadorTest.js
//
// Descripción:
// Pruebas automatizadas de LogicaFakeNavegador.
//
// Comprueba:
// - uso de GET /medicion;
// - recuperación de MedicionAlmacenada;
// - datos devueltos por el backend;
// - gestión de respuestas HTTP incorrectas.
//
// Autor: Élia Garcia Pons
// Fecha: 13/10/2026
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
// ------------------------------------------------------------


const LogicaFakeNavegador =
    require("./LogicaFakeNavegador.js");


// ------------------------------------------------------------
// condicion: B,
// mensaje: Text
// --> comprobar() -->
// ------------------------------------------------------------
//
// Comprueba el resultado de un test.
//
// Si la condición no se cumple lanza un error.
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
// ejecutarTests() -->
// ------------------------------------------------------------
//
// Ejecuta las pruebas automatizadas de
// LogicaFakeNavegador.
// ------------------------------------------------------------
async function ejecutarTests() {

    let urlRecibida =
        null;

    let opcionesRecibidas =
        null;


    // --------------------------------------------------------
    // url: Text,
    // opciones: PeticionHTTP
    // --> fetchFake()
    // --> respuesta: RespuestaHTTP
    // --------------------------------------------------------
    //
    // Simula una respuesta correcta del backend.
    // --------------------------------------------------------
    async function fetchFake(
        url,
        opciones
    ) {

        urlRecibida =
            url;

        opcionesRecibidas =
            opciones;


        return {

            ok:
                true,

            async json() {

                return {
                    id: 1,

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
    }


    const logica =
        new LogicaFakeNavegador(
            "http://127.0.0.1:8000",
            fetchFake
        );


    const medicion =
        await logica.mostrarDatos();


    // --------------------------------------------------------
    // TEST 1
    //
    // Comprobar la ruta utilizada.
    // --------------------------------------------------------

    comprobar(
        urlRecibida
            ===
        "http://127.0.0.1:8000/medicion",

        "mostrarDatos() utiliza /medicion"
    );


    // --------------------------------------------------------
    // TEST 2
    //
    // Comprobar el método HTTP.
    // --------------------------------------------------------

    comprobar(
        opcionesRecibidas.method
            ===
        "GET",

        "mostrarDatos() realiza una peticion GET"
    );


    // --------------------------------------------------------
    // TEST 3
    //
    // Comprobar id.
    // --------------------------------------------------------

    comprobar(
        medicion.id
            ===
        1,

        "ID correcto"
    );


    // --------------------------------------------------------
    // TEST 4
    //
    // Comprobar UUID.
    // --------------------------------------------------------

    comprobar(
        medicion.uuid
            ===
        "45505347-2D47-5449-2D50-524F592D3341",

        "UUID correcto"
    );


    // --------------------------------------------------------
    // TEST 5
    //
    // Comprobar fecha.
    // --------------------------------------------------------

    comprobar(
        medicion.fecha
            ===
        "2026-10-13 10:00:00",

        "Fecha correcta"
    );


    // --------------------------------------------------------
    // TEST 6
    //
    // Comprobar Major.
    // --------------------------------------------------------

    comprobar(
        medicion.major
            ===
        2821,

        "Major correcto"
    );


    // --------------------------------------------------------
    // TEST 7
    //
    // Comprobar Minor.
    // --------------------------------------------------------

    comprobar(
        medicion.minor
            ===
        1234,

        "Minor correcto"
    );


    // --------------------------------------------------------
    // TEST 8
    //
    // Comprobar TxPower.
    // --------------------------------------------------------

    comprobar(
        medicion.TxPower
            ===
        -53,

        "TxPower correcto"
    );


    // --------------------------------------------------------
    // TEST 9
    //
    // Comprobar respuesta HTTP incorrecta.
    // --------------------------------------------------------

    async function fetchError() {

        return {
            ok: false
        };
    }


    const logicaError =
        new LogicaFakeNavegador(
            "http://127.0.0.1:8000",
            fetchError
        );


    let errorDetectado =
        false;


    try {

        await logicaError.mostrarDatos();

    } catch (error) {

        errorDetectado =
            true;
    }


    comprobar(
        errorDetectado,
        "mostrarDatos() detecta errores HTTP"
    );


    console.log("");

    console.log(
        "TODOS LOS TESTS DE LA LOGICA FAKE "
            + "DEL NAVEGADOR HAN FINALIZADO CORRECTAMENTE."
    );
}


// ------------------------------------------------------------
// Ejecutar las pruebas.
// ------------------------------------------------------------

ejecutarTests();