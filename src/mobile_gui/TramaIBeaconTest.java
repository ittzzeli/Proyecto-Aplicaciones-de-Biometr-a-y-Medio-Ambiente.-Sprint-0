package com.example.biometriasprint0;

// ------------------------------------------------------------
// Archivo: TramaIBeaconTest.java
//
// Descripción:
// Pruebas unitarias para comprobar la interpretación de
// una trama iBeacon conocida.
//
// Autor: Élia Garcia Pons
// Fecha: 12/10/2026
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
// ------------------------------------------------------------

import static org.junit.Assert.assertEquals;

import org.junit.Test;

public class TramaIBeaconTest {


    // ------------------------------------------------------------
    // testInterpretarTramaIBeacon() -->
    // ------------------------------------------------------------
    //
    // Comprueba que una trama iBeacon conocida se interpreta
    // correctamente.
    //
    // Valores esperados:
    //
    // UUID:
    // 45505347-2D47-5449-2D50-524F592D3341
    //
    // Major:
    // 0x0B05 = 2821
    //
    // Minor:
    // 0x04D2 = 1234
    //
    // TxPower:
    // -53
    // ------------------------------------------------------------
    @Test
    public void testInterpretarTramaIBeacon() {

        byte[] bytes = new byte[] {

                // ADV Flags
                0x02,
                0x01,
                0x06,

                // ADV Header
                0x1A,
                (byte) 0xFF,

                // Company ID: Apple = 0x004C
                0x4C,
                0x00,

                // iBeacon Type
                0x02,

                // iBeacon Length = 0x15
                0x15,

                // UUID = EPSG-GTI-PROY-3A
                0x45,
                0x50,
                0x53,
                0x47,
                0x2D,
                0x47,
                0x54,
                0x49,
                0x2D,
                0x50,
                0x52,
                0x4F,
                0x59,
                0x2D,
                0x33,
                0x41,

                // Major = 0x0B05 = 2821
                0x0B,
                0x05,

                // Minor = 0x04D2 = 1234
                0x04,
                (byte) 0xD2,

                // TxPower = -53
                (byte) 0xCB
        };


        TramaIBeacon trama =
                new TramaIBeacon(bytes);


        int major =
                Utilidades.bytesToInt(
                        trama.getMajor()
                );


        int minor =
                Utilidades.bytesToInt(
                        trama.getMinor()
                );


        String uuid =
                Utilidades.bytesToHexString(
                        trama.getUUID()
                );


        // --------------------------------------------------------
        // Comprobar Major.
        // --------------------------------------------------------

        assertEquals(
                2821,
                major
        );


        // --------------------------------------------------------
        // Comprobar Minor.
        // --------------------------------------------------------

        assertEquals(
                1234,
                minor
        );


        // --------------------------------------------------------
        // Comprobar TxPower.
        // --------------------------------------------------------

        assertEquals(
                -53,
                trama.getTxPower()
        );


        // --------------------------------------------------------
        // Comprobar UUID.
        // --------------------------------------------------------

        assertEquals(
                "455053472D4754492D50524F592D3341",
                uuid
        );


        // --------------------------------------------------------
        // Comprobar tipo iBeacon.
        // --------------------------------------------------------

        assertEquals(
                0x02,
                trama.getIBeaconType()
        );


        // --------------------------------------------------------
        // Comprobar longitud iBeacon.
        // --------------------------------------------------------

        assertEquals(
                0x15,
                trama.getIBeaconLength()
        );
    }
}