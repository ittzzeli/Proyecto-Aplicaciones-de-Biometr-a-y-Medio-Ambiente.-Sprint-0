package com.example.biometriasprint0;

// ------------------------------------------------------------
// Archivo: Utilidades.java
//
// Descripción:
// Contiene operaciones auxiliares para convertir entre
// texto, UUID, arrays de bytes y valores numéricos.
//
// Estas operaciones se utilizan principalmente para
// interpretar los datos obtenidos de una trama iBeacon.
//
// Esta clase no realiza escaneo BLE, peticiones HTTP
// ni acceso a la base de datos.
//
// Autor: Élia Garcia Pons
// Fecha: 12/10/2026
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
//
// Aportación:
// Agrupa conversiones auxiliares utilizadas al interpretar los
// campos binarios de una trama iBeacon.
// ------------------------------------------------------------

import java.nio.charset.StandardCharsets;
import java.util.UUID;

public class Utilidades {


    // ------------------------------------------------------------
    // texto: Text --> stringToBytes() --> bytes: [ N ]
    // ------------------------------------------------------------
    //
    // Convierte un texto a un array de bytes utilizando UTF-8.
    // ------------------------------------------------------------
    public static byte[] stringToBytes(
            String texto
    ) {

        return texto.getBytes(
                StandardCharsets.UTF_8
        );
    }


    // ------------------------------------------------------------
    // uuid_texto: Text --> stringToUUID() --> uuid
    // ------------------------------------------------------------
    //
    // Convierte un UUID escrito como texto al tipo UUID
    // utilizado por Java.
    // ------------------------------------------------------------
    public static UUID stringToUUID(
            String uuidTexto
    ) {

        return UUID.fromString(
                uuidTexto
        );
    }


    // ------------------------------------------------------------
    // bytes: [ N ] --> bytesToString() --> texto: Text
    // ------------------------------------------------------------
    //
    // Convierte un array de bytes a texto utilizando UTF-8.
    // ------------------------------------------------------------
    public static String bytesToString(
            byte[] bytes
    ) {

        return new String(
                bytes,
                StandardCharsets.UTF_8
        );
    }


    // ------------------------------------------------------------
    // bytes: [ N ] --> bytesToInt() --> valor: Z
    // ------------------------------------------------------------
    //
    // Convierte un array de bytes en un número entero.
    //
    // Los bytes se interpretan en orden big-endian,
    // que es el utilizado por Major y Minor en iBeacon.
    // ------------------------------------------------------------
    public static int bytesToInt(
            byte[] bytes
    ) {

        int valor = 0;

        for (byte b : bytes) {

            valor =
                    (valor << 8)
                            |
                            (b & 0xFF);
        }

        return valor;
    }


    // ------------------------------------------------------------
    // bytes: [ N ] --> bytesToLong() --> valor: Z
    // ------------------------------------------------------------
    //
    // Convierte un array de bytes en un valor long.
    // ------------------------------------------------------------
    public static long bytesToLong(
            byte[] bytes
    ) {

        long valor = 0;

        for (byte b : bytes) {

            valor =
                    (valor << 8)
                            |
                            (b & 0xFF);
        }

        return valor;
    }


    // ------------------------------------------------------------
    // bytes: [ N ] --> bytesToHexString() --> texto: Text
    // ------------------------------------------------------------
    //
    // Convierte un array de bytes a una cadena hexadecimal.
    //
    // Ejemplo:
    //
    // 45 50 53 47
    //
    // se convierte en:
    //
    // 45505347
    // ------------------------------------------------------------
    public static String bytesToHexString(
            byte[] bytes
    ) {

        StringBuilder resultado =
                new StringBuilder();


        for (byte b : bytes) {

            resultado.append(
                    String.format(
                            "%02X",
                            b & 0xFF
                    )
            );
        }


        return resultado.toString();
    }
}