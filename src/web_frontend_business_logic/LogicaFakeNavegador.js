// ------------------------------------------------------------
// Archivo: LogicaFakeNavegador.js
//
// Descripción:
// Implementa la lógica de negocio fake/proxy utilizada
// por el cliente web.
//
// Representa la operación mostrarDatos() del backend,
// realizando internamente una petición GET /medicion.
//
// La interfaz gráfica no necesita conocer cómo se realiza
// la comunicación HTTP.
//
// Autor: Élia Garcia Pons
// Fecha: 13/10/2026
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
// ------------------------------------------------------------


class LogicaFakeNavegador {

    // ------------------------------------------------------------
    // servidor: Text,
    // fetch_impl: FuncionHTTP
    // --> LogicaFakeNavegador() -->
    // ------------------------------------------------------------
    //
    // Recibe la dirección base del servidor y la función
    // utilizada para realizar peticiones HTTP.
    //
    // fetchImpl puede sustituirse durante los tests para evitar
    // realizar comunicaciones HTTP reales.
    // ------------------------------------------------------------
    constructor(
        servidor,
        fetchImpl = fetch
    ) {

        this.servidor =
            servidor;

        this.fetchImpl =
            fetchImpl;
    }


    // ------------------------------------------------------------
    // mostrarDatos()
    // --> medicion: MedicionAlmacenada
    // ------------------------------------------------------------
    //
    // Mantiene la misma operación lógica mostrarDatos()
    // que business_logic.
    //
    // Realiza internamente:
    //
    // GET /medicion
    //
    // y devuelve la última Medicion almacenada.
    // ------------------------------------------------------------
    async mostrarDatos() {

        const respuesta =
            await this.fetchImpl(
                this.servidor
                    + "/medicion",
                {
                    method:
                        "GET",

                    headers: {
                        "Accept":
                            "application/json"
                    }
                }
            );


        if (!respuesta.ok) {

            throw new Error(
                "No se pudo obtener la Medicion"
            );
        }


        const medicion =
            await respuesta.json();


        return medicion;
    }
}


// ------------------------------------------------------------
// Exportación utilizada exclusivamente por los tests Node.js.
// ------------------------------------------------------------

if (
    typeof module !== "undefined"
    &&
    module.exports
) {

    module.exports =
        LogicaFakeNavegador;
}