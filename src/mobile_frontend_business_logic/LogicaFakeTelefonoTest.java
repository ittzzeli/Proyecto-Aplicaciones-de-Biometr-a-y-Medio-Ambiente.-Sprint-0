package com.example.biometriasprint0;

// ------------------------------------------------------------
// Archivo: LogicaFakeTelefonoTest.java
//
// Descripción:
// Pruebas automatizadas de LogicaFakeTelefono.
//
// Comprueba:
// - creación del JSON;
// - uso de POST /medicion;
// - respuesta correcta del backend;
// - respuesta de error;
// - error de comunicación.
//
// Autor: Élia Garcia Pons
// Fecha: 12/10/2026
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
// ------------------------------------------------------------

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import java.io.IOException;

public class LogicaFakeTelefonoTest {


    // ------------------------------------------------------------
    // PeticionarioRESTFake
    //
    // Responsabilidad:
    // Simular PeticionarioREST durante los tests sin realizar
    // conexiones HTTP reales.
    // ------------------------------------------------------------
    private static class PeticionarioRESTFake
            extends PeticionarioREST {

        String ultimoMetodo;

        String ultimaRuta;

        String ultimoCuerpo;

        int codigoRespuesta =
                201;

        boolean provocarError =
                false;


        // ------------------------------------------------------------
        // PeticionarioRESTFake() -->
        // ------------------------------------------------------------
        public PeticionarioRESTFake() {

            super(
                    "http://servidor-pruebas"
            );
        }


        // ------------------------------------------------------------
        // metodo: Text,
        // ruta: Text,
        // cuerpo: Text
        // --> realizarPeticion()
        // --> respuesta: RespuestaREST
        // ------------------------------------------------------------
        //
        // Guarda los datos de la petición para poder comprobarlos
        // desde los tests.
        // ------------------------------------------------------------
        @Override
        public RespuestaREST realizarPeticion(
                String metodo,
                String ruta,
                String cuerpo
        ) throws IOException {

            if (provocarError) {

                throw new IOException(
                        "Error simulado"
                );
            }


            ultimoMetodo =
                    metodo;

            ultimaRuta =
                    ruta;

            ultimoCuerpo =
                    cuerpo;


            return new RespuestaREST(
                    codigoRespuesta,
                    "{\"resultado\":\"prueba\"}"
            );
        }
    }


    // ------------------------------------------------------------
    // crearMedicionPrueba()
    // --> medicion: Medicion
    // ------------------------------------------------------------
    //
    // Crea una medición conocida utilizada por los tests.
    // ------------------------------------------------------------
    private Medicion crearMedicionPrueba() {

        return new Medicion(
                "45505347-2D47-5449-2D50-524F592D3341",
                "2026-10-12 20:00:00",
                2821,
                1234,
                -53
        );
    }


    // ------------------------------------------------------------
    // testCrearJson() -->
    // ------------------------------------------------------------
    //
    // Comprueba que una Medicion se transforma al JSON
    // esperado por el backend.
    // ------------------------------------------------------------
    @Test
    public void testCrearJson() {

        PeticionarioRESTFake peticionario =
                new PeticionarioRESTFake();


        LogicaFakeTelefono logica =
                new LogicaFakeTelefono(
                        peticionario
                );


        Medicion medicion =
                crearMedicionPrueba();


        String json =
                logica.crearJson(
                        medicion
                );


        assertEquals(
                "{"
                        + "\"uuid\":\"45505347-2D47-5449-2D50-524F592D3341\","
                        + "\"fecha\":\"2026-10-12 20:00:00\","
                        + "\"major\":2821,"
                        + "\"minor\":1234,"
                        + "\"TxPower\":-53"
                        + "}",
                json
        );
    }


    // ------------------------------------------------------------
    // testLeerDatosRealizaPostMedicion() -->
    // ------------------------------------------------------------
    //
    // Comprueba que leerDatos() utiliza:
    //
    // POST /medicion
    //
    // y envía la Medicion en el cuerpo de la petición.
    // ------------------------------------------------------------
    @Test
    public void testLeerDatosRealizaPostMedicion() {

        PeticionarioRESTFake peticionario =
                new PeticionarioRESTFake();


        LogicaFakeTelefono logica =
                new LogicaFakeTelefono(
                        peticionario
                );


        Medicion medicion =
                crearMedicionPrueba();


        boolean resultado =
                logica.leerDatos(
                        medicion
                );


        assertTrue(
                resultado
        );


        assertEquals(
                "POST",
                peticionario.ultimoMetodo
        );


        assertEquals(
                "/medicion",
                peticionario.ultimaRuta
        );


        assertEquals(
                "{"
                        + "\"uuid\":\"45505347-2D47-5449-2D50-524F592D3341\","
                        + "\"fecha\":\"2026-10-12 20:00:00\","
                        + "\"major\":2821,"
                        + "\"minor\":1234,"
                        + "\"TxPower\":-53"
                        + "}",
                peticionario.ultimoCuerpo
        );
    }


    // ------------------------------------------------------------
    // testLeerDatosRespuestaCorrecta() -->
    // ------------------------------------------------------------
    //
    // Comprueba que leerDatos() devuelve true cuando el
    // servidor responde con un código HTTP 2xx.
    // ------------------------------------------------------------
    @Test
    public void testLeerDatosRespuestaCorrecta() {

        PeticionarioRESTFake peticionario =
                new PeticionarioRESTFake();


        peticionario.codigoRespuesta =
                201;


        LogicaFakeTelefono logica =
                new LogicaFakeTelefono(
                        peticionario
                );


        boolean resultado =
                logica.leerDatos(
                        crearMedicionPrueba()
                );


        assertTrue(
                resultado
        );
    }


    // ------------------------------------------------------------
    // testLeerDatosRespuestaError() -->
    // ------------------------------------------------------------
    //
    // Comprueba que leerDatos() devuelve false cuando el
    // backend responde con un código de error.
    // ------------------------------------------------------------
    @Test
    public void testLeerDatosRespuestaError() {

        PeticionarioRESTFake peticionario =
                new PeticionarioRESTFake();


        peticionario.codigoRespuesta =
                500;


        LogicaFakeTelefono logica =
                new LogicaFakeTelefono(
                        peticionario
                );


        boolean resultado =
                logica.leerDatos(
                        crearMedicionPrueba()
                );


        assertFalse(
                resultado
        );
    }


    // ------------------------------------------------------------
    // testLeerDatosErrorComunicacion() -->
    // ------------------------------------------------------------
    //
    // Comprueba que leerDatos() devuelve false cuando ocurre
    // un error durante la comunicación HTTP.
    // ------------------------------------------------------------
    @Test
    public void testLeerDatosErrorComunicacion() {

        PeticionarioRESTFake peticionario =
                new PeticionarioRESTFake();


        peticionario.provocarError =
                true;


        LogicaFakeTelefono logica =
                new LogicaFakeTelefono(
                        peticionario
                );


        boolean resultado =
                logica.leerDatos(
                        crearMedicionPrueba()
                );


        assertFalse(
                resultado
        );
    }
}