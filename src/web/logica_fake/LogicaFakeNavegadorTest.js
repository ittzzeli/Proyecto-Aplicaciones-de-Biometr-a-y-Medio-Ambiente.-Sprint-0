const LogicaFakeNavegador =
    require("./LogicaFakeNavegador.js");


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


async function ejecutarTests() {

    let urlRecibida = null;
    let opcionesRecibidas = null;


    // --------------------------------------------------------
    // fetch falso.
    // --------------------------------------------------------

    async function fetchFake(
        url,
        opciones
    ) {

        urlRecibida = url;
        opcionesRecibidas = opciones;


        return {

            ok: true,

            async json() {

                return {
                    id: 1,
                    uuid: "45505347-2D47-5449-2D50-524F592D3341",
                    fecha: "2026-10-02 16:00:00",
                    major: 30,
                    minor: 0,
                    TxPower: -53
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


    comprobar(
        urlRecibida ===
            "http://127.0.0.1:8000/medicion",

        "mostrarDatos() utiliza /medicion"
    );


    comprobar(
        opcionesRecibidas.method === "GET",

        "mostrarDatos() realiza una petición GET"
    );


    comprobar(
        medicion.uuid ===
            "45505347-2D47-5449-2D50-524F592D3341",

        "UUID correcto"
    );


    comprobar(
        medicion.fecha ===
            "2026-10-02 16:00:00",

        "Fecha correcta"
    );


    comprobar(
        medicion.major === 30,

        "Major correcto"
    );


    comprobar(
        medicion.minor === 0,

        "Minor correcto"
    );


    comprobar(
        medicion.TxPower === -53,

        "TxPower correcto"
    );


    console.log("");

    console.log(
        "TODOS LOS TESTS DE LA LOGICA FAKE "
        + "DEL NAVEGADOR HAN FINALIZADO CORRECTAMENTE."
    );
}


ejecutarTests();