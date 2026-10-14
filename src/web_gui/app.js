// ------------------------------------------------------------
// Archivo: app.js
//
// Descripción:
// Gestiona la interfaz gráfica web del Sprint 0.
//
// Solicita la última medición mediante
// LogicaFakeNavegador y muestra sus datos en pantalla.
//
// El usuario puede solicitar una nueva actualización
// mediante el botón Actualizar.
//
// Este archivo no realiza peticiones HTTP directamente.
//
// Autor: Élia Garcia Pons
// Fecha: 14/10/2026
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
// Solicita la última Medicion mediante la lógica frontend
// y muestra sus datos en la interfaz.
// ------------------------------------------------------------
async function mostrarUltimaMedicion(
    logica,
    elementos
) {

    try {

        const medicion =
            await logica.mostrarDatos();


        elementos.valor.textContent =
            medicion.valor;


        elementos.fechaHora.textContent =
            medicion.fecha_hora;


        elementos.identificadorSensor.textContent =
            medicion.identificador_sensor;


        elementos.tipo.textContent =
            medicion.tipo;


        elementos.error.textContent =
            "";

    } catch (error) {

        elementos.error.textContent =
            "No se ha podido recuperar la última medición.";
    }
}


// ------------------------------------------------------------
// iniciarInterfaz() -->
// ------------------------------------------------------------
//
// Obtiene los elementos HTML, crea LogicaFakeNavegador,
// muestra la última medición y configura el botón Actualizar.
// ------------------------------------------------------------
function iniciarInterfaz() {

    const elementos = {

        valor:
            document.getElementById(
                "valor"
            ),

        fechaHora:
            document.getElementById(
                "fechaHora"
            ),

        identificadorSensor:
            document.getElementById(
                "identificadorSensor"
            ),

        tipo:
            document.getElementById(
                "tipo"
            ),

        error:
            document.getElementById(
                "error"
            )
    };


    const botonActualizar =
        document.getElementById(
            "botonActualizar"
        );


    const logica =
        new LogicaFakeNavegador(
            ""
        );


    // --------------------------------------------------------
    // Mostrar la última medición al abrir la página.
    // --------------------------------------------------------

    mostrarUltimaMedicion(
        logica,
        elementos
    );


    // --------------------------------------------------------
    // Actualizar la medición cuando el usuario pulse
    // el botón.
    //
    // Mientras se realiza la actualización, el botón queda
    // desactivado para evitar varias peticiones simultáneas.
    // --------------------------------------------------------

    botonActualizar.addEventListener(
        "click",
        async () => {

            botonActualizar.disabled =
                true;


            botonActualizar.textContent =
                "Actualizando...";


            await mostrarUltimaMedicion(
                logica,
                elementos
            );


            botonActualizar.disabled =
                false;


            botonActualizar.textContent =
                "Actualizar";
        }
    );
}


// ------------------------------------------------------------
// Iniciar la interfaz cuando el documento esté preparado.
// ------------------------------------------------------------

if (
    typeof document !== "undefined"
) {

    document.addEventListener(
        "DOMContentLoaded",
        iniciarInterfaz
    );
}


// ------------------------------------------------------------
// Permitir utilizar mostrarUltimaMedicion() desde los tests
// ejecutados con Node.
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