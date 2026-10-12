package com.example.biometriasprint0;

// ------------------------------------------------------------
// Archivo: LogicaFakeTelefono.java
//
// Descripción:
// Implementa la lógica de negocio fake/proxy utilizada
// por el cliente Android.
//
// Recibe una Medicion y delega su envío al backend mediante
// PeticionarioREST.
//
// Esta clase encapsula los detalles de JSON, HTTP y de la
// ruta REST utilizada para almacenar mediciones.
//
// Autor: Élia Garcia Pons
// Fecha: 12/10/2026
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
// ------------------------------------------------------------

import java.io.IOException;

public class LogicaFakeTelefono {

    private final PeticionarioREST peticionario;


    // ------------------------------------------------------------
    // servidor: Text --> LogicaFakeTelefono() -->
    // ------------------------------------------------------------
    //
    // Crea la lógica fake utilizando la dirección base
    // del servidor REST.
    // ------------------------------------------------------------
    public LogicaFakeTelefono(
            String servidor
    ) {

        this.peticionario =
                new PeticionarioREST(
                        servidor
                );
    }


    // ------------------------------------------------------------
    // peticionario: PeticionarioREST
    // --> LogicaFakeTelefono() -->
    // ------------------------------------------------------------
    //
    // Permite recibir directamente un PeticionarioREST.
    //
    // Se utiliza principalmente para poder realizar pruebas
    // automatizadas sin acceder a un servidor real.
    // ------------------------------------------------------------
    public LogicaFakeTelefono(
            PeticionarioREST peticionario
    ) {

        this.peticionario =
                peticionario;
    }


    // ------------------------------------------------------------
    // medicion: Medicion
    // --> leerDatos()
    // --> resultado: B
    // ------------------------------------------------------------
    //
    // Mantiene la misma operación lógica leerDatos() que
    // business_logic.
    //
    // Convierte la Medicion a JSON y la envía mediante:
    //
    // POST /medicion
    //
    // Devuelve true si el servidor responde con un código
    // HTTP correcto (2xx).
    //
    // IMPORTANTE:
    // Esta operación realiza comunicación de red y debe
    // ejecutarse fuera del hilo principal de Android.
    // ------------------------------------------------------------
    public boolean leerDatos(
            Medicion medicion
    ) {

        String json =
                crearJson(
                        medicion
                );


        try {

            RespuestaREST respuesta =
                    peticionario.realizarPeticion(
                            "POST",
                            "/medicion",
                            json
                    );


            return respuesta.getCodigo() >= 200
                    &&
                    respuesta.getCodigo() < 300;

        } catch (IOException excepcion) {

            return false;
        }
    }


    // ------------------------------------------------------------
    // medicion: Medicion
    // --> crearJson()
    // --> json: Text
    // ------------------------------------------------------------
    //
    // Convierte una Medicion a la representación JSON
    // esperada por el backend.
    //
    // Campos:
    // - uuid
    // - fecha
    // - major
    // - minor
    // - TxPower
    // ------------------------------------------------------------
    String crearJson(
            Medicion medicion
    ) {

        return "{"
                + "\"uuid\":\""
                + escaparTextoJson(
                medicion.getUuid()
        )
                + "\","

                + "\"fecha\":\""
                + escaparTextoJson(
                medicion.getFecha()
        )
                + "\","

                + "\"major\":"
                + medicion.getMajor()
                + ","

                + "\"minor\":"
                + medicion.getMinor()
                + ","

                + "\"TxPower\":"
                + medicion.getTxPower()

                + "}";
    }


    // ------------------------------------------------------------
    // texto: Text
    // --> escaparTextoJson()
    // --> texto_escapado: Text
    // ------------------------------------------------------------
    //
    // Escapa los caracteres básicos necesarios para incluir
    // un texto dentro de una cadena JSON.
    // ------------------------------------------------------------
    private String escaparTextoJson(
            String texto
    ) {

        return texto
                .replace(
                        "\\",
                        "\\\\"
                )
                .replace(
                        "\"",
                        "\\\""
                );
    }
}