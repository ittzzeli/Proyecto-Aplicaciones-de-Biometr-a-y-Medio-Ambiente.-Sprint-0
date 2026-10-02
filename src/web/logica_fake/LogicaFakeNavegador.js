// ------------------------------------------------------------
// LogicaFakeNavegador
//
// Representa mostrarDatos() desde el punto de vista
// del navegador.
//
// En lugar de acceder directamente a la lógica de negocio:
//
// GET /medicion
//
// y devuelve la Medicion recibida como JSON.
// ------------------------------------------------------------

class LogicaFakeNavegador {

    constructor(
        servidor,
        fetchImpl = fetch
    ) {
        this.servidor = servidor;
        this.fetchImpl = fetchImpl;
    }


    // ------------------------------------------------------------
    // mostrarDatos() --> medicion: Medicion
    // ------------------------------------------------------------

    async mostrarDatos() {

        const respuesta =
            await this.fetchImpl(
                this.servidor + "/medicion",
                {
                    method: "GET",
                    headers: {
                        "Accept": "application/json"
                    }
                }
            );


        if (!respuesta.ok) {

            throw new Error(
                "No se pudo obtener la Medicion"
            );
        }


        return await respuesta.json();
    }
}


// Permite utilizar la clase también desde tests con Node.js.
if (
    typeof module !== "undefined"
    && module.exports
) {
    module.exports =
        LogicaFakeNavegador;
}