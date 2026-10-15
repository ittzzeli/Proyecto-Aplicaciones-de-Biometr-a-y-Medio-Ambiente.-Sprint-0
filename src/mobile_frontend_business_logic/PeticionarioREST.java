package com.example.biometriasprint0;

// ------------------------------------------------------------
// Archivo: PeticionarioREST.java
//
// Descripción:
// Encapsula la comunicación HTTP con el backend.
//
// Recibe un método HTTP, una ruta y un cuerpo opcional,
// realiza la petición y devuelve una RespuestaREST.
//
// Esta clase no contiene lógica relacionada con BLE,
// iBeacon ni almacenamiento en la base de datos.
//
// Autor: Élia Garcia Pons
// Fecha: 12/10/2026
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
//
// Aportación:
// Encapsula las peticiones HTTP para mantener los detalles de
// transporte fuera de la interfaz gráfica Android.
// ------------------------------------------------------------

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.StandardCharsets;

public class PeticionarioREST {

    private final String servidor;


    // ------------------------------------------------------------
    // servidor: Text --> PeticionarioREST() -->
    // ------------------------------------------------------------
    //
    // Recibe la dirección base del servidor REST.
    //
    // Ejemplo:
    //
    // http://192.168.1.100
    //
    // ------------------------------------------------------------
    public PeticionarioREST(
            String servidor
    ) {

        this.servidor =
                servidor;
    }


    // ------------------------------------------------------------
    // metodo: Text,
    // ruta: Text,
    // cuerpo: Text
    // --> realizarPeticion()
    // --> respuesta: RespuestaREST
    // ------------------------------------------------------------
    //
    // Realiza una petición HTTP al backend.
    //
    // Devuelve el código HTTP y el cuerpo de la respuesta.
    //
    // IMPORTANTE:
    // Este método debe ejecutarse fuera del hilo principal
    // de Android.
    // ------------------------------------------------------------
    public RespuestaREST realizarPeticion(
            String metodo,
            String ruta,
            String cuerpo
    ) throws IOException {

        String direccion =
                construirUrl(
                        ruta
                );


        URL url =
                new URL(
                        direccion
                );


        HttpURLConnection conexion =
                (HttpURLConnection)
                        url.openConnection();


        try {

            conexion.setRequestMethod(
                    metodo
            );


            conexion.setConnectTimeout(
                    5000
            );


            conexion.setReadTimeout(
                    5000
            );


            conexion.setRequestProperty(
                    "Accept",
                    "application/json"
            );


            conexion.setRequestProperty(
                    "Content-Type",
                    "application/json; charset=utf-8"
            );


            // ----------------------------------------------------
            // Si existe cuerpo, se envía al servidor.
            // ----------------------------------------------------

            if (
                    cuerpo != null
                            &&
                            !cuerpo.isEmpty()
            ) {

                conexion.setDoOutput(
                        true
                );


                byte[] bytes =
                        cuerpo.getBytes(
                                StandardCharsets.UTF_8
                        );


                try (
                        OutputStream salida =
                                conexion.getOutputStream()
                ) {

                    salida.write(
                            bytes
                    );


                    salida.flush();
                }
            }


            int codigo =
                    conexion.getResponseCode();


            String json =
                    leerCuerpoRespuesta(
                            conexion,
                            codigo
                    );


            return new RespuestaREST(
                    codigo,
                    json
            );

        } finally {

            conexion.disconnect();
        }
    }


    // ------------------------------------------------------------
    // conexion: ConexionHTTP,
    // codigo: N
    // --> leerCuerpoRespuesta()
    // --> cuerpo: Text
    // ------------------------------------------------------------
    //
    // Lee el cuerpo devuelto por el servidor.
    //
    // Para respuestas correctas utiliza el flujo normal.
    // Para respuestas de error utiliza el flujo de error.
    // ------------------------------------------------------------
    private String leerCuerpoRespuesta(
            HttpURLConnection conexion,
            int codigo
    ) throws IOException {

        InputStream entrada;


        if (
                codigo >= 200
                        &&
                        codigo < 400
        ) {

            entrada =
                    conexion.getInputStream();

        } else {

            entrada =
                    conexion.getErrorStream();
        }


        if (entrada == null) {

            return "";
        }


        StringBuilder contenido =
                new StringBuilder();


        try (
                BufferedReader lector =
                        new BufferedReader(
                                new InputStreamReader(
                                        entrada,
                                        StandardCharsets.UTF_8
                                )
                        )
        ) {

            String linea;


            while (
                    (linea = lector.readLine())
                            != null
            ) {

                contenido.append(
                        linea
                );
            }
        }


        return contenido.toString();
    }


    // ------------------------------------------------------------
    // ruta: Text --> construirUrl() --> url: Text
    // ------------------------------------------------------------
    //
    // Combina la dirección base del servidor y la ruta REST
    // evitando barras duplicadas o ausentes.
    // ------------------------------------------------------------
    private String construirUrl(
            String ruta
    ) {

        if (
                servidor.endsWith("/")
                        &&
                        ruta.startsWith("/")
        ) {

            return servidor
                    + ruta.substring(1);
        }


        if (
                !servidor.endsWith("/")
                        &&
                        !ruta.startsWith("/")
        ) {

            return servidor
                    + "/"
                    + ruta;
        }


        return servidor
                + ruta;
    }
}