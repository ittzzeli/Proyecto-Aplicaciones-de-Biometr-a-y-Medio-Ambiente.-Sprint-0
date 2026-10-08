package com.example.logica_fake;

import org.junit.Test;

import static org.junit.Assert.*;


// ------------------------------------------------------------
// Tests automáticos de LogicaFakeTelefono.
// ------------------------------------------------------------

public class LogicaFakeTelefonoTest {


    private static class LogicaFakePrueba
            extends LogicaFakeTelefono {

        String metodoRecibido;
        String rutaRecibida;
        String cuerpoRecibido;


        public LogicaFakePrueba() {

            super("http://localhost:8000");
        }


        @Override
        protected boolean enviarPeticion(
                String metodo,
                String ruta,
                String cuerpo
        ) {

            metodoRecibido = metodo;
            rutaRecibida = ruta;
            cuerpoRecibido = cuerpo;

            return true;
        }
    }


    @Test
    public void leerDatosRealizaPostMedicion()
            throws Exception {

        LogicaFakePrueba logica =
                new LogicaFakePrueba();


        Medicion medicion =
                new Medicion(
                        "45505347-2D47-5449-2D50-524F592D3341",
                        "2026-10-02 16:00:00",
                        30,
                        0,
                        -53
                );


        boolean resultado =
                logica.leerDatos(medicion);


        assertTrue(resultado);

        assertEquals(
                "POST",
                logica.metodoRecibido
        );

        assertEquals(
                "/medicion",
                logica.rutaRecibida
        );

        assertTrue(
                logica.cuerpoRecibido.contains(
                        "\"uuid\":\"45505347-2D47-5449-2D50-524F592D3341\""
                )
        );

        assertTrue(
                logica.cuerpoRecibido.contains(
                        "\"major\":30"
                )
        );

        assertTrue(
                logica.cuerpoRecibido.contains(
                        "\"minor\":0"
                )
        );

        assertTrue(
                logica.cuerpoRecibido.contains(
                        "\"TxPower\":-53"
                )
        );
    }
}