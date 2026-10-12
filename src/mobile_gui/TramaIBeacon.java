package com.example.biometriasprint0;

// ------------------------------------------------------------
// Archivo: TramaIBeacon.java
//
// Descripción:
// Representa y descompone una trama iBeacon recibida mediante
// Bluetooth Low Energy.
//
// Extrae de los bytes recibidos:
// - prefijo;
// - UUID;
// - Major;
// - Minor;
// - TxPower.
//
// Esta clase no realiza escaneo BLE ni comunicaciones HTTP.
//
// Autor: Élia Garcia Pons
// Fecha: 12/10/2026
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
// ------------------------------------------------------------

import java.util.Arrays;

public class TramaIBeacon {

    private final byte[] prefijo;

    private final byte[] uuid;

    private final byte[] major;

    private final byte[] minor;

    private final int txPower;

    private final byte[] advFlags;

    private final byte[] advHeader;

    private final byte[] companyID;

    private final int iBeaconType;

    private final int iBeaconLength;


    // ------------------------------------------------------------
    // bytes: [ N ] --> TramaIBeacon() -->
    // ------------------------------------------------------------
    //
    // Recibe los bytes de una trama iBeacon y separa
    // cada uno de sus campos.
    //
    // Formato utilizado:
    //
    // bytes 0 - 2   -> ADV Flags
    // bytes 3 - 4   -> ADV Header
    // bytes 5 - 6   -> Company ID
    // byte 7        -> iBeacon Type
    // byte 8        -> iBeacon Length
    // bytes 9 - 24  -> UUID
    // bytes 25 - 26 -> Major
    // bytes 27 - 28 -> Minor
    // byte 29       -> TxPower
    // ------------------------------------------------------------
    public TramaIBeacon(byte[] bytes) {

        if (bytes == null || bytes.length < 30) {

            throw new IllegalArgumentException(
                    "La trama iBeacon debe contener al menos 30 bytes."
            );
        }


        advFlags =
                Arrays.copyOfRange(
                        bytes,
                        0,
                        3
                );


        advHeader =
                Arrays.copyOfRange(
                        bytes,
                        3,
                        5
                );


        companyID =
                Arrays.copyOfRange(
                        bytes,
                        5,
                        7
                );


        iBeaconType =
                bytes[7] & 0xFF;


        iBeaconLength =
                bytes[8] & 0xFF;


        prefijo =
                Arrays.copyOfRange(
                        bytes,
                        0,
                        9
                );


        uuid =
                Arrays.copyOfRange(
                        bytes,
                        9,
                        25
                );


        major =
                Arrays.copyOfRange(
                        bytes,
                        25,
                        27
                );


        minor =
                Arrays.copyOfRange(
                        bytes,
                        27,
                        29
                );


        txPower =
                bytes[29];
    }


    // ------------------------------------------------------------
    // getPrefijo() --> prefijo: [ N ]_9
    // ------------------------------------------------------------
    public byte[] getPrefijo() {

        return Arrays.copyOf(
                prefijo,
                prefijo.length
        );
    }


    // ------------------------------------------------------------
    // getUUID() --> uuid: [ N ]_16
    // ------------------------------------------------------------
    public byte[] getUUID() {

        return Arrays.copyOf(
                uuid,
                uuid.length
        );
    }


    // ------------------------------------------------------------
    // getMajor() --> major: [ N ]_2
    // ------------------------------------------------------------
    public byte[] getMajor() {

        return Arrays.copyOf(
                major,
                major.length
        );
    }


    // ------------------------------------------------------------
    // getMinor() --> minor: [ N ]_2
    // ------------------------------------------------------------
    public byte[] getMinor() {

        return Arrays.copyOf(
                minor,
                minor.length
        );
    }


    // ------------------------------------------------------------
    // getTxPower() --> TxPower: Z
    // ------------------------------------------------------------
    public int getTxPower() {

        return txPower;
    }


    // ------------------------------------------------------------
    // getAdvFlags() --> adv_flags: [ N ]_3
    // ------------------------------------------------------------
    public byte[] getAdvFlags() {

        return Arrays.copyOf(
                advFlags,
                advFlags.length
        );
    }


    // ------------------------------------------------------------
    // getAdvHeader() --> adv_header: [ N ]_2
    // ------------------------------------------------------------
    public byte[] getAdvHeader() {

        return Arrays.copyOf(
                advHeader,
                advHeader.length
        );
    }


    // ------------------------------------------------------------
    // getCompanyID() --> company_id: [ N ]_2
    // ------------------------------------------------------------
    public byte[] getCompanyID() {

        return Arrays.copyOf(
                companyID,
                companyID.length
        );
    }


    // ------------------------------------------------------------
    // getIBeaconType() --> ibeacon_type: N
    // ------------------------------------------------------------
    public int getIBeaconType() {

        return iBeaconType;
    }


    // ------------------------------------------------------------
    // getIBeaconLength() --> ibeacon_length: N
    // ------------------------------------------------------------
    public int getIBeaconLength() {

        return iBeaconLength;
    }
}