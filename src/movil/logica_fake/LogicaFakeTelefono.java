package com.example.logica_fake;

import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.StandardCharsets;


// ------------------------------------------------------------
// LogicaFakeTelefono
//
// Representa leerDatos() desde el punto de vista del teléfono.
//
// En vez de acceder directamente a la lógica de negocio,
// realiza:
//
// POST /medicion
//
// enviando la Medicion en formato JSON.
// ------------------------------------------------------------

public class LogicaFakeTelefono {

    private final String servidor;


    // ------------------------------------------------------------
    // servidor: Text --> LogicaFakeTelefono()
    // ------------------------------------------------------------

    public LogicaFakeTelefono(String servidor) {
        this.servidor = servidor;
    }


    // ------------------------------------------------------------
    // medicion: Medicion --> leerDatos() --> resultado: B
    // ------------------------------------------------------------

    public boolean leerDatos(Medicion medicion) throws Exception {

        String json = crearJson(medicion);

        return enviarPeticion(
                "POST",
                "/medicion",
                json
        );
    }


    // ------------------------------------------------------------
    // medicion: Medicion --> crearJson() --> json: Text
    // ------------------------------------------------------------

    protected String crearJson(Medicion medicion) {

        return "{"
                + "\"uuid\":\"" + medicion.getUuid() + "\","
                + "\"fecha\":\"" + medicion.getFecha() + "\","
                + "\"major\":" + medicion.getMajor() + ","
                + "\"minor\":" + medicion.getMinor() + ","
                + "\"TxPower\":" + medicion.getTxPower()
                + "}";
    }


    // ------------------------------------------------------------
    // metodo: Text, ruta: Text, cuerpo: Text
    // --> enviarPeticion() --> resultado: B
    // ------------------------------------------------------------
    //
    // Método separado para poder sustituir la comunicación
    // HTTP durante los tests.
    // ------------------------------------------------------------

    protected boolean enviarPeticion(
            String metodo,
            String ruta,
            String cuerpo
    ) throws Exception {

        URL url = new URL(
                servidor + ruta
        );

        HttpURLConnection conexion =
                (HttpURLConnection) url.openConnection();

        conexion.setRequestMethod(metodo);

        conexion.setRequestProperty(
                "Content-Type",
                "application/json"
        );

        conexion.setDoOutput(true);


        byte[] datos =
                cuerpo.getBytes(StandardCharsets.UTF_8);


        try (OutputStream salida =
                     conexion.getOutputStream()) {

            salida.write(datos);
        }


        int codigo =
                conexion.getResponseCode();

        conexion.disconnect();


        return codigo >= 200
                && codigo < 300;
    }
}