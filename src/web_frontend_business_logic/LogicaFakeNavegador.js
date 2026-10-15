// ------------------------------------------------------------
// Archivo: LogicaFakeNavegador.js
//
// Descripción:
// Implementa la lógica fake/proxy utilizada por el navegador.
//
// Permite a la interfaz web solicitar la última medición
// almacenada sin realizar directamente peticiones HTTP.
//
// La comunicación con el backend queda encapsulada en
// este componente.
//
// Autor: Élia Garcia Pons
// Fecha: 14/10/2026
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
//
// Aportación:
// Encapsula la recuperación de la última medición desde
// el backend para que la interfaz web no realice
// peticiones HTTP directamente.
// ------------------------------------------------------------


class LogicaFakeNavegador {

    // ------------------------------------------------------------
    // servidor: Text,
    // fetchImpl: Funcion
    // --> LogicaFakeNavegador() -->
    // ------------------------------------------------------------
    //
    // Recibe la dirección base del servidor.
    //
    // fetchImpl permite sustituir fetch durante las pruebas.
    // ------------------------------------------------------------
    constructor(
        servidor,
        fetchImpl = null
    ) {

        this.servidor =
            servidor;


        if (fetchImpl !== null) {

            this.fetchImpl =
                fetchImpl;

        } else {

            this.fetchImpl =
                (
                    url,
                    opciones
                ) =>
                    fetch(
                        url,
                        opciones
                    );
        }
    }


    // ------------------------------------------------------------
    // mostrarDatos()
    // --> medicion: Medicion
    // ------------------------------------------------------------
    //
    // Solicita al backend la última Medicion almacenada.
    //
    // Utiliza:
    //
    // GET /medicion
    //
    // La Medicion recibida contiene:
    //
    // - valor
    // - fecha_hora
    // - identificador_sensor
    // - tipo
    // ------------------------------------------------------------
    async mostrarDatos() {

        const respuesta =
            await this.fetchImpl(
                this.servidor + "/medicion",
                {
                    method:
                        "GET",

                    cache:
                        "no-store",

                    headers: {
                        "Accept":
                            "application/json"
                    }
                }
            );


        if (!respuesta.ok) {

            throw new Error(
                "No se ha podido recuperar la medición."
            );
        }


        return await respuesta.json();
    }
}


// ------------------------------------------------------------
// Permite utilizar la clase desde los tests ejecutados con Node.
// ------------------------------------------------------------
if (
    typeof module !== "undefined"
    &&
    module.exports
) {

    module.exports =
        LogicaFakeNavegador;
}