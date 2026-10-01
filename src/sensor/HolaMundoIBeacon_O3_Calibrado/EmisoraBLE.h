// -*- mode: c++ -*-

// ----------------------------------------------------------
// Jordi Bataller i Mascarell
// 2019-07-07
// ----------------------------------------------------------

#ifndef EMISORA_H_INCLUIDO
#define EMISORA_H_INCLUIDO


#include "ServicioEnEmisora.h"


// ----------------------------------------------------------
// EmisoraBLE
//
// Responsabilidad:
// Gestionar la comunicación Bluetooth Low Energy (BLE),
// incluyendo la configuración y emisión de anuncios iBeacon.
//
// Esta clase NO realiza mediciones ni calcula concentraciones.
// ----------------------------------------------------------

class EmisoraBLE {

private:

  const char * nombreEmisora;

  const uint16_t fabricanteID;

  const int8_t txPower;


public:

  using CallbackConexionEstablecida =
    void (uint16_t connHandle);

  using CallbackConexionTerminada =
    void (uint16_t connHandle, uint8_t reason);


  // ------------------------------------------------------------
  // nombre_emisora: Text,
  // fabricante_id: N,
  // tx_power: Z
  // --> EmisoraBLE()
  // ------------------------------------------------------------
  EmisoraBLE(const char * nombreEmisora_,
             const uint16_t fabricanteID_,
             const int8_t txPower_)
    :
      nombreEmisora(nombreEmisora_),
      fabricanteID(fabricanteID_),
      txPower(txPower_)
  {
    // La emisora no se inicia en el constructor.
    //
    // Se inicia posteriormente mediante encenderEmisora(),
    // cuando el resto del sistema ya está configurado.
  }


  // ------------------------------------------------------------
  // encenderEmisora() -->
  // ------------------------------------------------------------
  //
  // Inicializa Bluefruit y garantiza que no exista
  // ningún anuncio BLE activo previamente.
  // ------------------------------------------------------------
  void encenderEmisora() {

    Bluefruit.begin();

    detenerAnuncio();
  }


  // ------------------------------------------------------------
  // callback_conexion: CallbackConexionEstablecida,
  // callback_desconexion: CallbackConexionTerminada
  // --> encenderEmisora() -->
  // ------------------------------------------------------------
  //
  // Inicializa la emisora BLE e instala los callbacks
  // de conexión y desconexión.
  // ------------------------------------------------------------
  void encenderEmisora(
    CallbackConexionEstablecida cbce,
    CallbackConexionTerminada cbct
  ) {

    encenderEmisora();

    instalarCallbackConexionEstablecida(cbce);

    instalarCallbackConexionTerminada(cbct);
  }


  // ------------------------------------------------------------
  // detenerAnuncio() -->
  // ------------------------------------------------------------
  //
  // Detiene el anuncio BLE si actualmente existe
  // uno en ejecución.
  // ------------------------------------------------------------
  void detenerAnuncio() {

    if (estaAnunciando()) {

      Bluefruit.Advertising.stop();
    }
  }


  // ------------------------------------------------------------
  // estaAnunciando() --> anunciando: B
  // ------------------------------------------------------------
  //
  // Indica si la emisora BLE está anunciando actualmente.
  // ------------------------------------------------------------
  bool estaAnunciando() {

    return Bluefruit.Advertising.isRunning();
  }


  // ------------------------------------------------------------
  // beacon_uuid: N[16],
  // major: N,
  // minor: N,
  // rssi: Z
  // --> emitirAnuncioIBeacon() -->
  // ------------------------------------------------------------
  //
  // Construye y comienza a emitir un anuncio iBeacon
  // con el UUID, Major, Minor y RSSI indicados.
  // ------------------------------------------------------------
  void emitirAnuncioIBeacon(uint8_t * beaconUUID,
                            uint16_t major,
                            uint16_t minor,
                            int8_t rssi) {

    // Detenemos cualquier anuncio anterior.
    detenerAnuncio();


    // Limpiamos cualquier paquete anterior antes
    // de construir el siguiente iBeacon.
    //
    // Esto evita conservar datos correspondientes
    // a anuncios anteriores.
    Bluefruit.Advertising.clearData();

    Bluefruit.ScanResponse.clearData();


    // Creamos el beacon.
    BLEBeacon elBeacon(
      beaconUUID,
      major,
      minor,
      rssi
    );


    // Establecemos el fabricante.
    elBeacon.setManufacturer(fabricanteID);


    // Configuración de la emisora.
    Bluefruit.setTxPower(txPower);

    Bluefruit.setName(nombreEmisora);


    // El nombre del dispositivo se añade
    // a la respuesta de escaneo.
    Bluefruit.ScanResponse.addName();


    // Añadimos el beacon al paquete de advertising.
    Bluefruit.Advertising.setBeacon(elBeacon);


    // Si se produce una desconexión,
    // se reinicia automáticamente el advertising.
    Bluefruit.Advertising.restartOnDisconnect(true);


    // Intervalo en unidades de 0.625 ms.
    Bluefruit.Advertising.setInterval(100, 100);


    // 0 indica emisión sin límite de tiempo.
    // El anuncio se detendrá posteriormente de forma explícita.
    Bluefruit.Advertising.start(0);
  }


  // ------------------------------------------------------------
  // carga: Text,
  // tamanyo_carga: N
  // --> emitirAnuncioIBeaconLibre() -->
  // ------------------------------------------------------------
  //
  // Permite emitir como Manufacturer Specific Data una carga
  // personalizada de hasta 21 bytes utilizando la estructura
  // correspondiente a un paquete iBeacon.
  // ------------------------------------------------------------
  void emitirAnuncioIBeaconLibre(
    const char * carga,
    const uint8_t tamanyoCarga
  ) {

    detenerAnuncio();


    Bluefruit.Advertising.clearData();

    Bluefruit.ScanResponse.clearData();


    Bluefruit.setName(nombreEmisora);

    Bluefruit.ScanResponse.addName();


    Bluefruit.Advertising.addFlags(
      BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE
    );


    // Los cuatro primeros bytes corresponden a:
    //
    // 0x4c, 0x00 -> Apple Company ID
    // 0x02       -> tipo iBeacon
    // 21         -> longitud de la carga posterior
    //
    // Los siguientes 21 bytes contienen la carga.
    uint8_t restoPrefijoYCarga[4 + 21] = {

      0x4c, 0x00,

      0x02,

      21,

      '-', '-', '-', '-',
      '-', '-', '-', '-',
      '-', '-', '-', '-',
      '-', '-', '-', '-',
      '-', '-', '-', '-',
      '-'
    };


    // Copiamos como máximo 21 bytes.
    memcpy(
      &restoPrefijoYCarga[4],
      &carga[0],
      (tamanyoCarga > 21 ? 21 : tamanyoCarga)
    );


    Bluefruit.Advertising.addData(
      BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA,
      &restoPrefijoYCarga[0],
      4 + 21
    );


    Bluefruit.Advertising.restartOnDisconnect(true);


    // Intervalo en unidades de 0.625 ms.
    Bluefruit.Advertising.setInterval(100, 100);


    // Número de segundos en modo rápido.
    Bluefruit.Advertising.setFastTimeout(1);


    // 0 indica emisión indefinida.
    Bluefruit.Advertising.start(0);


    Globales::elPuerto.escribir(
      "emitiriBeacon libre Bluefruit.Advertising.start(0);\n"
    );
  }


  // ------------------------------------------------------------
  // servicio: ServicioEnEmisora
  // --> anyadirServicio()
  // --> resultado: B
  // ------------------------------------------------------------
  //
  // Añade un servicio BLE a los datos de advertising.
  // ------------------------------------------------------------
  bool anyadirServicio(
    ServicioEnEmisora & servicio
  ) {

    Globales::elPuerto.escribir(
      "Bluefruit.Advertising.addService(servicio);\n"
    );


    bool resultado =
      Bluefruit.Advertising.addService(servicio);


    if (!resultado) {

      Serial.println(
        "SERVICIO NO AÑADIDO"
      );
    }


    // ServicioEnEmisora proporciona la conversión
    // necesaria al tipo BLEService.
    return resultado;
  }


  // ------------------------------------------------------------
  // servicio: ServicioEnEmisora
  // --> anyadirServicioConSusCaracteristicas()
  // --> resultado: B
  // ------------------------------------------------------------
  //
  // Caso base de la incorporación de servicios
  // y características.
  // ------------------------------------------------------------
  bool anyadirServicioConSusCaracteristicas(
    ServicioEnEmisora & servicio
  ) {

    return anyadirServicio(servicio);
  }


  // ------------------------------------------------------------
  // servicio: ServicioEnEmisora,
  // caracteristica: Caracteristica,
  // resto_caracteristicas: ...
  // --> anyadirServicioConSusCaracteristicas()
  // --> resultado: B
  // ------------------------------------------------------------
  //
  // Añade de forma recursiva las características
  // indicadas a un servicio BLE.
  // ------------------------------------------------------------
  template <typename ... T>
  bool anyadirServicioConSusCaracteristicas(
    ServicioEnEmisora & servicio,
    ServicioEnEmisora::Caracteristica & caracteristica,
    T& ... restoCaracteristicas
  ) {

    servicio.anyadirCaracteristica(
      caracteristica
    );


    return anyadirServicioConSusCaracteristicas(
      servicio,
      restoCaracteristicas...
    );
  }


  // ------------------------------------------------------------
  // servicio: ServicioEnEmisora,
  // caracteristicas: ...
  // --> anyadirServicioConSusCaracteristicasYActivar()
  // --> resultado: B
  // ------------------------------------------------------------
  //
  // Añade las características al servicio y posteriormente
  // activa dicho servicio.
  // ------------------------------------------------------------
  template <typename ... T>
  bool anyadirServicioConSusCaracteristicasYActivar(
    ServicioEnEmisora & servicio,
    T& ... restoCaracteristicas
  ) {

    bool resultado =
      anyadirServicioConSusCaracteristicas(
        servicio,
        restoCaracteristicas...
      );


    servicio.activarServicio();


    return resultado;
  }


  // ------------------------------------------------------------
  // callback: CallbackConexionEstablecida
  // --> instalarCallbackConexionEstablecida() -->
  // ------------------------------------------------------------
  //
  // Registra la función que será ejecutada cuando
  // se establezca una conexión BLE.
  // ------------------------------------------------------------
  void instalarCallbackConexionEstablecida(
    CallbackConexionEstablecida cb
  ) {

    Bluefruit.Periph.setConnectCallback(cb);
  }


  // ------------------------------------------------------------
  // callback: CallbackConexionTerminada
  // --> instalarCallbackConexionTerminada() -->
  // ------------------------------------------------------------
  //
  // Registra la función que será ejecutada cuando
  // termine una conexión BLE.
  // ------------------------------------------------------------
  void instalarCallbackConexionTerminada(
    CallbackConexionTerminada cb
  ) {

    Bluefruit.Periph.setDisconnectCallback(cb);
  }


  // ------------------------------------------------------------
  // conn_handle: N
  // --> getConexion()
  // --> conexion: BLEConnection
  // ------------------------------------------------------------
  //
  // Recupera la conexión BLE correspondiente
  // al identificador indicado.
  // ------------------------------------------------------------
  BLEConnection * getConexion(
    uint16_t connHandle
  ) {

    return Bluefruit.Connection(
      connHandle
    );
  }

}; // class EmisoraBLE


#endif