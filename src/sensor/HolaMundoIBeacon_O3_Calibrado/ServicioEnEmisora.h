// -*- mode: c++ -*-

// ----------------------------------------------------------
// Jordi Bataller i Mascarell
// 2019-07-17
// ----------------------------------------------------------

#ifndef SERVICIO_EMISORA_H_INCLUIDO
#define SERVICIO_EMISORA_H_INCLUIDO


#include <vector>


// ------------------------------------------------------------
// p: T[], n: Z --> alReves() --> resultado: T[]
// ------------------------------------------------------------
//
// Invierte el orden de los primeros n elementos de un array.
//
// La inversión se realiza sobre el propio array recibido.
// ------------------------------------------------------------
template<typename T>
T * alReves(T * p, int n) {

  T aux;

  for (int i = 0; i < n / 2; i++) {

    aux = p[i];

    p[i] = p[n - i - 1];

    p[n - i - 1] = aux;
  }

  return p;
}


// ------------------------------------------------------------
// texto: Text, destino: N[], tam_max: Z
// --> stringAUint8AlReves()
// --> resultado: N[]
// ------------------------------------------------------------
//
// Copia una cadena de caracteres dentro de un array uint8_t
// colocando los caracteres en orden inverso.
//
// Se copian como máximo tamMax caracteres.
//
// Esta función se utiliza para construir los UUID de los
// servicios y características BLE.
// ------------------------------------------------------------
uint8_t * stringAUint8AlReves(
  const char * pString,
  uint8_t * pUint,
  int tamMax
) {

  int longitudString =
    strlen(pString);

  int longitudCopiar =
    (
      longitudString > tamMax
      ? tamMax
      : longitudString
    );


  for (int i = 0; i <= longitudCopiar - 1; i++) {

    pUint[tamMax - i - 1] =
      pString[i];
  }


  return pUint;
}


// ----------------------------------------------------------
// ServicioEnEmisora
//
// Responsabilidad:
// Representar y gestionar un servicio BLE junto con las
// características que forman parte de dicho servicio.
//
// Esta clase encapsula la configuración y activación de
// BLEService y BLECharacteristic.
// ----------------------------------------------------------
class ServicioEnEmisora {

public:

  // ----------------------------------------------------------
  // Tipo utilizado para callbacks cuando una característica
  // BLE recibe una escritura.
  // ----------------------------------------------------------
  using CallbackCaracteristicaEscrita =
    void (
      uint16_t conn_handle,
      BLECharacteristic * chr,
      uint8_t * data,
      uint16_t len
    );


  // ==========================================================
  // Caracteristica
  // ==========================================================
  //
  // Responsabilidad:
  // Representar una característica perteneciente a un
  // servicio BLE.
  //
  // Permite configurar:
  //
  // - UUID
  // - propiedades
  // - permisos
  // - tamaño máximo
  // - escritura
  // - notificación
  // - callback de escritura
  // ==========================================================
  class Caracteristica {

  private:

    // --------------------------------------------------------
    // UUID de 16 bytes.
    //
    // El UUID recibido como texto se copia aquí en orden
    // inverso.
    // --------------------------------------------------------
    uint8_t uuidCaracteristica[16] = {

      '0', '1', '2', '3',

      '4', '5', '6', '7',

      '8', '9', 'A', 'B',

      'C', 'D', 'E', 'F'
    };


    BLECharacteristic laCaracteristica;


  public:

    // ------------------------------------------------------------
    // nombre_caracteristica: Text --> Caracteristica()
    // ------------------------------------------------------------
    //
    // Construye una característica BLE utilizando como UUID
    // el identificador recibido.
    // ------------------------------------------------------------
    Caracteristica(
      const char * nombreCaracteristica_
    )
      :
        laCaracteristica(
          stringAUint8AlReves(
            nombreCaracteristica_,
            &uuidCaracteristica[0],
            16
          )
        )
    {
    }


    // ------------------------------------------------------------
    // nombre_caracteristica: Text,
    // propiedades: N,
    // permiso_lectura: SecureMode,
    // permiso_escritura: SecureMode,
    // tamanyo: N
    // --> Caracteristica()
    // ------------------------------------------------------------
    //
    // Construye una característica BLE y configura sus
    // propiedades, permisos y tamaño máximo de datos.
    // ------------------------------------------------------------
    Caracteristica(
      const char * nombreCaracteristica_,
      uint8_t props,
      SecureMode_t permisoRead,
      SecureMode_t permisoWrite,
      uint8_t tam
    )
      :
        Caracteristica(
          nombreCaracteristica_
        )
    {

      asignarPropiedadesPermisosYTamanyoDatos(
        props,
        permisoRead,
        permisoWrite,
        tam
      );
    }


  private:

    // ------------------------------------------------------------
    // propiedades: N --> asignarPropiedades() -->
    // ------------------------------------------------------------
    //
    // Configura las propiedades de la característica BLE.
    //
    // Ejemplos:
    // CHR_PROPS_WRITE
    // CHR_PROPS_READ
    // CHR_PROPS_NOTIFY
    // ------------------------------------------------------------
    void asignarPropiedades(
      uint8_t props
    ) {

      laCaracteristica.setProperties(
        props
      );
    }


    // ------------------------------------------------------------
    // permiso_lectura: SecureMode,
    // permiso_escritura: SecureMode
    // --> asignarPermisos() -->
    // ------------------------------------------------------------
    //
    // Establece los permisos de lectura y escritura de la
    // característica BLE.
    // ------------------------------------------------------------
    void asignarPermisos(
      SecureMode_t permisoRead,
      SecureMode_t permisoWrite
    ) {

      laCaracteristica.setPermission(
        permisoRead,
        permisoWrite
      );
    }


    // ------------------------------------------------------------
    // tamanyo: N --> asignarTamanyoDatos() -->
    // ------------------------------------------------------------
    //
    // Establece el tamaño máximo de datos admitido por
    // la característica BLE.
    // ------------------------------------------------------------
    void asignarTamanyoDatos(
      uint8_t tam
    ) {

      laCaracteristica.setMaxLen(
        tam
      );
    }


  public:

    // ------------------------------------------------------------
    // propiedades: N,
    // permiso_lectura: SecureMode,
    // permiso_escritura: SecureMode,
    // tamanyo: N
    // --> asignarPropiedadesPermisosYTamanyoDatos() -->
    // ------------------------------------------------------------
    //
    // Configura conjuntamente propiedades, permisos y tamaño
    // máximo de una característica BLE.
    // ------------------------------------------------------------
    void asignarPropiedadesPermisosYTamanyoDatos(
      uint8_t props,
      SecureMode_t permisoRead,
      SecureMode_t permisoWrite,
      uint8_t tam
    ) {

      asignarPropiedades(
        props
      );

      asignarPermisos(
        permisoRead,
        permisoWrite
      );

      asignarTamanyoDatos(
        tam
      );
    }


    // ------------------------------------------------------------
    // datos: Text --> escribirDatos() --> bytes_escritos: N
    // ------------------------------------------------------------
    //
    // Escribe una cadena de datos en la característica BLE
    // y devuelve el número de bytes escritos.
    // ------------------------------------------------------------
    uint16_t escribirDatos(
      const char * str
    ) {

      uint16_t resultado =
        laCaracteristica.write(
          str
        );

      return resultado;
    }


    // ------------------------------------------------------------
    // datos: Text --> notificarDatos() --> resultado: N
    // ------------------------------------------------------------
    //
    // Envía una notificación BLE con los datos indicados.
    // ------------------------------------------------------------
    uint16_t notificarDatos(
      const char * str
    ) {

      uint16_t resultado =
        laCaracteristica.notify(
          &str[0]
        );

      return resultado;
    }


    // ------------------------------------------------------------
    // callback: CallbackCaracteristicaEscrita
    // --> instalarCallbackCaracteristicaEscrita() -->
    // ------------------------------------------------------------
    //
    // Instala la función que será ejecutada cuando otro
    // dispositivo escriba sobre la característica.
    // ------------------------------------------------------------
    void instalarCallbackCaracteristicaEscrita(
      CallbackCaracteristicaEscrita cb
    ) {

      laCaracteristica.setWriteCallback(
        cb
      );
    }


    // ------------------------------------------------------------
    // activar() -->
    // ------------------------------------------------------------
    //
    // Inicializa la característica BLE.
    // ------------------------------------------------------------
    void activar() {

      err_t error =
        laCaracteristica.begin();


      Globales::elPuerto.escribir(
        "(*this).laCaracteristica.begin(); error = "
      );

      Globales::elPuerto.escribir(
        error
      );
    }

  }; // class Caracteristica


private:

  // --------------------------------------------------------
  // UUID correspondiente al servicio BLE.
  //
  // El UUID recibido como texto se almacena en este array
  // en orden inverso.
  // --------------------------------------------------------
  uint8_t uuidServicio[16] = {

    '0', '1', '2', '3',

    '4', '5', '6', '7',

    '8', '9', 'A', 'B',

    'C', 'D', 'E', 'F'
  };


  BLEService elServicio;


  // --------------------------------------------------------
  // Características pertenecientes al servicio.
  //
  // Se almacenan punteros porque las características son
  // creadas externamente y posteriormente añadidas aquí.
  // --------------------------------------------------------
  std::vector<Caracteristica *> lasCaracteristicas;


public:

  // ------------------------------------------------------------
  // nombre_servicio: Text --> ServicioEnEmisora()
  // ------------------------------------------------------------
  //
  // Construye un servicio BLE utilizando el texto recibido
  // como identificador UUID.
  // ------------------------------------------------------------
  ServicioEnEmisora(
    const char * nombreServicio_
  )
    :
      elServicio(
        stringAUint8AlReves(
          nombreServicio_,
          &uuidServicio[0],
          16
        )
      )
  {
  }


  // ------------------------------------------------------------
  // escribeUUID() -->
  // ------------------------------------------------------------
  //
  // Muestra por el puerto serie los 16 caracteres
  // almacenados en el UUID del servicio.
  //
  // Se utiliza principalmente para depuración.
  // ------------------------------------------------------------
  void escribeUUID() {

    Serial.println(
      "**********"
    );


    for (int i = 0; i <= 15; i++) {

      Serial.print(
        (char) uuidServicio[i]
      );
    }


    Serial.println(
      "\n**********"
    );
  }


  // ------------------------------------------------------------
  // caracteristica: Caracteristica
  // --> anyadirCaracteristica() -->
  // ------------------------------------------------------------
  //
  // Añade una característica a la lista de características
  // pertenecientes al servicio.
  // ------------------------------------------------------------
  void anyadirCaracteristica(
    Caracteristica & car
  ) {

    lasCaracteristicas.push_back(
      &car
    );
  }


  // ------------------------------------------------------------
  // activarServicio() -->
  // ------------------------------------------------------------
  //
  // Inicializa el servicio BLE y posteriormente activa
  // todas las características asociadas a él.
  // ------------------------------------------------------------
  void activarServicio() {

    err_t error =
      elServicio.begin();


    Serial.print(
      "(*this).elServicio.begin(); error = "
    );

    Serial.println(
      error
    );


    for (
      auto pCar : lasCaracteristicas
    ) {

      (*pCar).activar();
    }
  }


  // ------------------------------------------------------------
  // ServicioEnEmisora --> BLEService
  // ------------------------------------------------------------
  //
  // Conversión implícita utilizada cuando una función de la
  // librería Bluefruit necesita recibir un BLEService.
  // ------------------------------------------------------------
  operator BLEService&() {

    return elServicio;
  }

}; // class ServicioEnEmisora


#endif