package com.example.biometriasprint0;

// ------------------------------------------------------------
// Archivo: Medicion.java
//
// Descripción:
// Representa una medición obtenida a partir de una trama
// iBeacon.
//
// Contiene los datos que posteriormente serán entregados a
// LogicaFakeTelefono para enviarlos al backend.
//
// Esta clase no contiene código BLE, HTTP ni acceso a la
// base de datos.
//
// Autor: Élia Garcia Pons
// Fecha: 08/10/2026
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
// ------------------------------------------------------------

public class Medicion {

    private final String uuid;
    private final String fecha;
    private final int major;
    private final int minor;
    private final int txPower;


    // ------------------------------------------------------------
    // uuid: Text,
    // fecha: Text,
    // major: Z,
    // minor: Z,
    // TxPower: Z
    // --> Medicion() -->
    // ------------------------------------------------------------
    //
    // Crea una medición con los datos obtenidos del iBeacon.
    // ------------------------------------------------------------
    public Medicion(
            String uuid,
            String fecha,
            int major,
            int minor,
            int txPower
    ) {
        this.uuid = uuid;
        this.fecha = fecha;
        this.major = major;
        this.minor = minor;
        this.txPower = txPower;
    }


    // ------------------------------------------------------------
    // getUuid() --> uuid: Text
    // ------------------------------------------------------------
    public String getUuid() {
        return uuid;
    }


    // ------------------------------------------------------------
    // getFecha() --> fecha: Text
    // ------------------------------------------------------------
    public String getFecha() {
        return fecha;
    }


    // ------------------------------------------------------------
    // getMajor() --> major: Z
    // ------------------------------------------------------------
    public int getMajor() {
        return major;
    }


    // ------------------------------------------------------------
    // getMinor() --> minor: Z
    // ------------------------------------------------------------
    public int getMinor() {
        return minor;
    }


    // ------------------------------------------------------------
    // getTxPower() --> TxPower: Z
    // ------------------------------------------------------------
    public int getTxPower() {
        return txPower;
    }
}