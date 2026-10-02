// ------------------------------------------------------------
// Interfaz gráfica del navegador.
//
// La interfaz NO se comunica directamente con REST.
// Los datos se obtienen exclusivamente mediante
// LogicaFakeNavegador.mostrarDatos().
// ------------------------------------------------------------


// ------------------------------------------------------------
// logica: LogicaFakeNavegador, elementos: Object
// --> mostrarUltimaMedicion()
// ------------------------------------------------------------
//
// Solicita la última medición a la lógica fake y coloca
// sus datos en los elementos correspondientes de la interfaz.
// ------------------------------------------------------------

async function mostrarUltimaMedicion(
    logica,
    elementos
) {

    try {

        const medicion =
            await logica.mostrarDatos();


        elementos.fecha.textContent =
            medicion.fecha;

        elementos.major.textContent =
            medicion.major;

        elementos.minor.textContent =
            medicion.minor;

        elementos.txPower.textContent =
            medicion.TxPower;


        elementos.error.hidden = true;

    } catch (error) {

        elementos.error.textContent =
            "No se pudo obtener la última medición.";

        elementos.error.hidden = false;
    }
}


// ------------------------------------------------------------
// Inicialización de la interfaz en el navegador.
// ------------------------------------------------------------

if (typeof document !== "undefined") {

    document.addEventListener(
        "DOMContentLoaded",
        async function () {

            // La lógica fake es la única capa utilizada
            // por la interfaz para obtener datos.
            const logica =
                new LogicaFakeNavegador(
                    "http://127.0.0.1:8000"
                );


            const elementos = {

                fecha:
                    document.getElementById("fecha"),

                major:
                    document.getElementById("major"),

                minor:
                    document.getElementById("minor"),

                txPower:
                    document.getElementById("txPower"),

                error:
                    document.getElementById("error")
            };


            await mostrarUltimaMedicion(
                logica,
                elementos
            );
        }
    );
}


// ------------------------------------------------------------
// Exportación utilizada exclusivamente por los tests Node.js.
// ------------------------------------------------------------

if (
    typeof module !== "undefined"
    && module.exports
) {

    module.exports = {
        mostrarUltimaMedicion
    };
}