// ------------------------------------------------------------
// Archivo: app.js
//
// Descripción:
// Gestiona la interfaz gráfica de la aplicación web.
//
// Solicita la última medición exclusivamente mediante
// LogicaFakeNavegador y muestra los datos obtenidos.
//
// Este archivo no realiza peticiones HTTP directamente.
//
// Autor: Élia Garcia Pons
// Fecha: 13/10/2026
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
// ------------------------------------------------------------


// ------------------------------------------------------------
// logica: LogicaFakeNavegador,
// elementos: ElementosGUI
// --> mostrarUltimaMedicion() -->
// ------------------------------------------------------------
//
// Solicita la última medición mediante la lógica fake
// y coloca sus valores en la interfaz.
//
// Si ocurre un error, muestra un mensaje al usuario.
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


        elementos.error.hidden =
            true;

    } catch (error) {

        elementos.error.textContent =
            "No se pudo obtener la última medición.";

        elementos.error.hidden =
            false;
    }
}


// ------------------------------------------------------------
// Inicialización de la interfaz.
// ------------------------------------------------------------
//
// Cuando el documento está preparado:
// - crea la lógica fake;
// - obtiene los elementos de la interfaz;
// - solicita la última medición.
// ------------------------------------------------------------
if (typeof document !== "undefined") {

    document.addEventListener(
        "DOMContentLoaded",
        async function () {

            // ----------------------------------------------------
            // La GUI solo conoce LogicaFakeNavegador.
            //
            // Una dirección vacía indica que el backend se
            // encuentra en el mismo servidor que la web.
            // ----------------------------------------------------

            const logica =
                new LogicaFakeNavegador(
                    ""
                );


            const elementos = {

                fecha:
                    document.getElementById(
                        "fecha"
                    ),

                major:
                    document.getElementById(
                        "major"
                    ),

                minor:
                    document.getElementById(
                        "minor"
                    ),

                txPower:
                    document.getElementById(
                        "txPower"
                    ),

                error:
                    document.getElementById(
                        "error"
                    )
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
    &&
    module.exports
) {

    module.exports = {
        mostrarUltimaMedicion
    };
}