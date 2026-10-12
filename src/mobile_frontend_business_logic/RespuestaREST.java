package com.example.biometriasprint0;

// ------------------------------------------------------------
// Archivo: RespuestaREST.java
//
// Descripción:
// Representa la respuesta obtenida después de realizar
// una petición REST al backend.
//
// Contiene:
// - código HTTP;
// - cuerpo de la respuesta.
//
// Esta clase no realiza peticiones HTTP.
//
// Autor: Élia Garcia Pons
// Fecha: 12/10/2026
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
// ------------------------------------------------------------

public class RespuestaREST {

    private final int codigo;
    private final String json;


    // ------------------------------------------------------------
    // codigo: N,
    // json: Text
    // --> RespuestaREST() -->
    // ------------------------------------------------------------
    //
    // Crea una respuesta REST con el código HTTP y
    // el cuerpo recibido del servidor.
    // ------------------------------------------------------------
    public RespuestaREST(
            int codigo,
            String json
    ) {

        this.codigo = codigo;
        this.json = json;
    }


    // ------------------------------------------------------------
    // getCodigo() --> codigo: N
    // ------------------------------------------------------------
    public int getCodigo() {

        return codigo;
    }


    // ------------------------------------------------------------
    // getJson() --> json: Text
    // ------------------------------------------------------------
    public String getJson() {

        return json;
    }
}