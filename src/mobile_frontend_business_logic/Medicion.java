package com.example.biometriasprint0;

// ------------------------------------------------------------
// Archivo: Medicion.java
//
// Descripción:
// Representa una medición preparada para ser enviada
// al backend.
//
// Contiene únicamente los datos que utiliza el modelo
// de negocio:
//
// - valor
// - fecha_hora
// - identificador_sensor
// - tipo
//
// Esta clase no contiene código BLE, HTTP ni acceso a la
// base de datos.
//
// Autor: Élia Garcia Pons
// Fecha: 13/10/2026
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
//
// Aportación:
// Define el modelo de medición compartido por la lógica frontend
// del cliente Android.
// ------------------------------------------------------------

public class Medicion {

    private final int valor;
    private final String fechaHora;
    private final String identificadorSensor;
    private final int tipo;


    // ------------------------------------------------------------
    // valor: Z,
    // fecha_hora: Text,
    // identificador_sensor: Text,
    // tipo: Z
    // --> Medicion() -->
    // ------------------------------------------------------------
    //
    // Crea una medición con los datos preparados para
    // ser enviados al backend.
    // ------------------------------------------------------------
    public Medicion(
            int valor,
            String fechaHora,
            String identificadorSensor,
            int tipo
    ) {
        this.valor = valor;
        this.fechaHora = fechaHora;
        this.identificadorSensor = identificadorSensor;
        this.tipo = tipo;
    }


    // ------------------------------------------------------------
    // getValor() --> valor: Z
    // ------------------------------------------------------------
    //
    // Devuelve el valor de la medición.
    // ------------------------------------------------------------
    public int getValor() {
        return valor;
    }


    // ------------------------------------------------------------
    // getFechaHora() --> fecha_hora: Text
    // ------------------------------------------------------------
    //
    // Devuelve la fecha y hora asociadas a la medición.
    // ------------------------------------------------------------
    public String getFechaHora() {
        return fechaHora;
    }


    // ------------------------------------------------------------
    // getIdentificadorSensor()
    // --> identificador_sensor: Text
    // ------------------------------------------------------------
    //
    // Devuelve el identificador único del sensor.
    // ------------------------------------------------------------
    public String getIdentificadorSensor() {
        return identificadorSensor;
    }


    // ------------------------------------------------------------
    // getTipo() --> tipo: Z
    // ------------------------------------------------------------
    //
    // Devuelve el tipo de medición.
    // ------------------------------------------------------------
    public int getTipo() {
        return tipo;
    }
}