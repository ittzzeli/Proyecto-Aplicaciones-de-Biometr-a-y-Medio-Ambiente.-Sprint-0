// ------------------------------------------------------------
// Archivo: LogicaFakeNavegadorTest.js
//
// Descripción:
// Pruebas automatizadas de LogicaFakeNavegador.
//
// Comprueba:
// - uso de GET /medicion;
// - recuperación de una Medicion;
// - campos del nuevo modelo;
// - gestión de respuestas HTTP incorrectas.
//
// Autor: Élia Garcia Pons
// Fecha: 14/10/2026
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
// testMostrarDatos() -->
// ------------------------------------------------------------
//
// Comprueba que mostrarDatos() utiliza GET /medicion
// y devuelve correctamente una Medicion.
// ------------------------------------------------------------
async function testMostrarDatos() {

    let metodoRecibido =
        null;

    let urlRecibida =
        null;


    const medicionEsperada = {

        valor:
            1234,

        fecha_hora:
            "2026-10-14 09:06:41",

        identificador_sensor:
            "45505347-2D47-5449-2D50-524F592D3341",

        tipo:
            11
    };


    const fetchFake =
        async (
            url,
            opciones
        ) => {

            urlRecibida =
                url;

            metodoRecibido =
                opciones.method;


            return {

                ok:
                    true,

                json:
                    async () =>
                        medicionEsperada
            };
        };


    const logica =
        new LogicaFakeNavegador(
            "https://servidor-pruebas",
            fetchFake
        );


    const medicion =
        await logica.mostrarDatos();


    comprobar(
        urlRecibida
            ===
        "https://servidor-pruebas/medicion",

        "mostrarDatos() utiliza /medicion"
    );


    comprobar(
        metodoRecibido
            ===
        "GET",

        "mostrarDatos() utiliza GET"
    );


    comprobar(
        medicion.valor
            ===
        1234,

        "Valor correcto"
    );


    comprobar(
        medicion.fecha_hora
            ===
        "2026-10-14 09:06:41",

        "Fecha y hora correctas"
    );


    comprobar(
        medicion.identificador_sensor
            ===
        "45505347-2D47-5449-2D50-524F592D3341",

        "Identificador del sensor correcto"
    );


    comprobar(
        medicion.tipo
            ===
        11,

        "Tipo correcto"
    );
}


// ------------------------------------------------------------
// testErrorHttp() -->
// ------------------------------------------------------------
//
// Comprueba que mostrarDatos() informa de un error
// cuando el backend devuelve una respuesta incorrecta.
// ------------------------------------------------------------
async function testErrorHttp() {

    const fetchFake =
        async () => {

            return {

                ok:
                    false,

                status:
                    500
            };
        };


    const logica =
        new LogicaFakeNavegador(
            "https://servidor-pruebas",
            fetchFake
        );


    let errorDetectado =
        false;


    try {

        await logica.mostrarDatos();

    } catch (error) {

        errorDetectado =
            true;
    }


    comprobar(
        errorDetectado,
        "mostrarDatos() detecta errores HTTP"
    );
}


// ------------------------------------------------------------
// ejecutarTests() -->
// ------------------------------------------------------------
//
// Ejecuta todas las pruebas del componente.
// ------------------------------------------------------------
async function ejecutarTests() {

    await testMostrarDatos();

    await testErrorHttp();


    console.log("");

    console.log(
        "TODOS LOS TESTS DE "
        + "WEB_FRONTEND_BUSINESS_LOGIC "
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