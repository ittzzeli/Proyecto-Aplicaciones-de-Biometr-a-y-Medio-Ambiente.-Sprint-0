SENSOR O3 + IBEACON - VERSION CON CALIBRACION DE CERO
=====================================================

CONEXIONES
----------
ULPSM-O3 pin 1 (Vgas)  -> SparkFun A3
ULPSM-O3 pin 2 (Vref)  -> SparkFun A4
ULPSM-O3 pin 3 (Vtemp) -> SparkFun A5
GND                     -> GND
V+                      -> alimentación del montaje

ADC
---
Resolución: 12 bits, códigos 0..4095.
Escala usada por el código: 0..3.3 V.
Conversión:

    V = ADC * 3.3 / 4095

En el core nRF52 se selecciona AR_VDD4 para que el rango del ADC quede
referenciado a VDD. El código supone que VDD es 3.3 V.

La frecuencia/velocidad de muestreo no cambia esta fórmula de conversión:
lo que determina voltios por código es la resolución y el rango de tensión.

CALIBRACION EN AIRE LIMPIO
--------------------------
Al arrancar el programa, ANTES de meter el sensor en ozono, se toman
50 muestras de Vgas y Vref.

    Vgas0 = promedio de Vgas con 0 ppm
    Vref0 = promedio de Vref con 0 ppm

El offset real del conjunto sensor + electrónica es:

    offset = Vgas0 - Vref0

Este valor queda guardado en RAM durante la ejecución.

CALCULO DE LA SENSIBILIDAD
--------------------------
Para O3, SPEC indica un TIA Gain de 499 kV/A.

El código calcula M mediante:

    M [V/ppm] = SensitivityCode [nA/ppm]
                * TIA_Gain [kV/A]
                * 1e-6

IMPORTANTE:
En Medidor.h debes cambiar SENSIBILIDAD_NA_POR_PPM por el Sensitivity Code
que aparece en la etiqueta de TU sensor.

Como valor provisional se deja -60.12 nA/ppm, que produce:

    M ~= -0.030 V/ppm

coincidiendo aproximadamente con el span típico de -30 mV/ppm del
datasheet ULPSM-O3.

CALCULO DE O3 DURANTE LA MEDIDA
-------------------------------
Para cada lectura se promedian 50 muestras de los tres canales.

Primero:

    diferencia = Vgas - Vref

Después quitamos el cero medido en aire limpio:

    diferenciaCorregida = (Vgas - Vref) - offset

Finalmente:

    O3(ppm) = diferenciaCorregida / M

Como M es negativo para este sensor, una caída de Vgas respecto a Vref
da como resultado una concentración positiva de O3.

IBEACON
-------
UUID = EPSG-GTI-PROY-3A

MAJOR:
Actualmente se usa como contador de prueba:

    1, 2, 3, 4, ...

En Publicador.h:

    MAJOR_COMO_CONTADOR = true

Cuando quieras que Major sea el identificador fijo del sensor de ozono,
cambia a:

    MAJOR_COMO_CONTADOR = false

Entonces Major será 14 (OZONO).

MINOR:
Para la prueba actual, Minor se envia SIEMPRE con valor 0.
La concentracion de O3 se sigue calculando internamente y se muestra por
el puerto serie, pero temporalmente NO se codifica en el iBeacon.


    ppm = Minor / 1000.0

IMPORTANTE PARA LA PRACTICA
---------------------------
1. Enciende y deja estabilizar el sensor.
2. Arranca/resetéalo estando todavía en aire limpio.
3. Comprueba por Serial el OFFSET calculado.
4. Después introduce el sensor en la bolsa de ozono.
5. Observa Vgas, Vref, la diferencia corregida y los ppm.
6. Para medidas cuantitativas, usa el Sensitivity Code real de la etiqueta.
