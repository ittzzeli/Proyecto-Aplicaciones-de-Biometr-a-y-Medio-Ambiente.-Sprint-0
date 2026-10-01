// -*- mode: c++ -*-

#ifndef LED_H_INCLUIDO
#define LED_H_INCLUIDO


// ----------------------------------------------------------
// Jordi Bataller i Mascarell
// 2019-07-07
// ----------------------------------------------------------


// ------------------------------------------------------------
// tiempo: Z --> esperar() -->
// ------------------------------------------------------------
//
// Detiene la ejecución durante el número de milisegundos
// indicado.
//
// Esta función produce un efecto temporal sobre la ejecución.
// ------------------------------------------------------------
void esperar(long tiempo) {

  delay(tiempo);
}


// ----------------------------------------------------------
// LED
//
// Responsabilidad:
// Gestionar un LED conectado a un pin digital de la placa.
// ----------------------------------------------------------
class LED {

private:

  int numeroLED;

  bool encendido;


public:

  // ------------------------------------------------------------
  // numero: Z --> LED()
  // ------------------------------------------------------------
  //
  // Inicializa el pin asociado al LED como salida y deja
  // inicialmente el LED apagado.
  // ------------------------------------------------------------
  LED(int numero)
    :
      numeroLED(numero),
      encendido(false)
  {

    pinMode(numeroLED, OUTPUT);

    apagar();
  }


  // ------------------------------------------------------------
  // encender() -->
  // ------------------------------------------------------------
  //
  // Enciende el LED y actualiza su estado interno.
  // ------------------------------------------------------------
  void encender() {

    digitalWrite(numeroLED, HIGH);

    encendido = true;
  }


  // ------------------------------------------------------------
  // apagar() -->
  // ------------------------------------------------------------
  //
  // Apaga el LED y actualiza su estado interno.
  // ------------------------------------------------------------
  void apagar() {

    digitalWrite(numeroLED, LOW);

    encendido = false;
  }


  // ------------------------------------------------------------
  // alternar() -->
  // ------------------------------------------------------------
  //
  // Cambia el estado actual del LED:
  //
  // encendido -> apagado
  // apagado   -> encendido
  // ------------------------------------------------------------
  void alternar() {

    if (encendido) {

      apagar();

    } else {

      encender();
    }
  }


  // ------------------------------------------------------------
  // tiempo: Z --> brillar() -->
  // ------------------------------------------------------------
  //
  // Enciende el LED durante el tiempo indicado y después
  // vuelve a apagarlo.
  // ------------------------------------------------------------
  void brillar(long tiempo) {

    encender();

    esperar(tiempo);

    apagar();
  }

}; // class LED


#endif