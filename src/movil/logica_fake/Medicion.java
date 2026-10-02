package com.example.logica_fake;

// ------------------------------------------------------------
// Medicion
//
// Representa los mismos datos utilizados por el servidor REST.
// ------------------------------------------------------------

public class Medicion {

    private String uuid;
    private String fecha;
    private int major;
    private int minor;
    private int TxPower;


    // ------------------------------------------------------------
    // uuid: Text, fecha: Text, major: N, minor: N, TxPower: Z
    // --> Medicion()
    // ------------------------------------------------------------

    public Medicion(
            String uuid,
            String fecha,
            int major,
            int minor,
            int TxPower
    ) {
        this.uuid = uuid;
        this.fecha = fecha;
        this.major = major;
        this.minor = minor;
        this.TxPower = TxPower;
    }


    public String getUuid() {
        return uuid;
    }

    public String getFecha() {
        return fecha;
    }

    public int getMajor() {
        return major;
    }

    public int getMinor() {
        return minor;
    }

    public int getTxPower() {
        return TxPower;
    }
}