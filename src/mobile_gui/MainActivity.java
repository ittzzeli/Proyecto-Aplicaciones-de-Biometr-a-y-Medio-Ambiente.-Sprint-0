package com.example.biometriasprint0;

// ------------------------------------------------------------
// Archivo: MainActivity.java
//
// Descripción:
// Actividad principal de la aplicación Android del Sprint 0.
//
// Gestiona:
// - permisos Bluetooth;
// - escaneo BLE;
// - detección del beacon Elia_GTI;
// - interpretación de la trama iBeacon;
// - control de mediciones repetidas;
// - creación de Medicion;
// - entrega de la medición a LogicaFakeTelefono.
//
// Autor: Élia Garcia Pons
// Fecha: 14/10/2026
//
// Copyright:
// Uso académico - Proyecto de Biometría y Medio Ambiente.
// ------------------------------------------------------------

import android.Manifest;
import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothManager;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanRecord;
import android.bluetooth.le.ScanResult;
import android.content.pm.PackageManager;
import android.location.LocationManager;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.widget.Button;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;


public class MainActivity extends AppCompatActivity {

    private static final String ETIQUETA_LOG =
            ">>>>";

    private static final String NOMBRE_BEACON =
            "Elia_GTI";

    private static final int CODIGO_PERMISOS_BLUETOOTH =
            100;

    private static final int COMPANY_ID_APPLE =
            0x004C;


    // ------------------------------------------------------------
    // Dirección del servidor.
    //
    // LogicaFakeTelefono encapsula la comunicación
    // con el backend.
    // ------------------------------------------------------------
    private static final String SERVIDOR =
            "https://egarpon.upv.edu.es";


    private BluetoothAdapter bluetoothAdapter;

    private BluetoothLeScanner bluetoothLeScanner;

    private TextView textoEstado;

    private LogicaFakeTelefono logicaFakeTelefono;


    // ------------------------------------------------------------
    // Último contador procesado.
    //
    // El contador corresponde al byte inferior del Major.
    //
    // Se utiliza únicamente para evitar procesar varias veces
    // la misma medición.
    // ------------------------------------------------------------
    private int ultimoContadorProcesado =
            -1;


    // ------------------------------------------------------------
    // onCreate() -->
    // ------------------------------------------------------------
    //
    // Inicializa la actividad y sus componentes.
    // ------------------------------------------------------------
    @Override
    protected void onCreate(
            Bundle savedInstanceState
    ) {

        super.onCreate(
                savedInstanceState
        );


        setContentView(
                R.layout.activity_main
        );


        Button botonBuscarEliaGTI =
                findViewById(
                        R.id.botonBuscarEliaGTI
                );


        Button botonDetenerBusqueda =
                findViewById(
                        R.id.botonDetenerBusqueda
                );


        textoEstado =
                findViewById(
                        R.id.textoEstado
                );


        // --------------------------------------------------------
        // Crear la lógica frontend utilizada por la GUI.
        // --------------------------------------------------------

        logicaFakeTelefono =
                new LogicaFakeTelefono(
                        SERVIDOR
                );


        BluetoothManager bluetoothManager =
                (BluetoothManager)
                        getSystemService(
                                BLUETOOTH_SERVICE
                        );


        bluetoothAdapter =
                bluetoothManager.getAdapter();


        if (bluetoothAdapter == null) {

            textoEstado.setText(
                    "Este dispositivo no dispone de Bluetooth."
            );

            return;
        }


        solicitarPermisosBluetooth();


        botonBuscarEliaGTI.setOnClickListener(
                v ->
                        buscarEliaGTI()
        );


        botonDetenerBusqueda.setOnClickListener(
                v ->
                        detenerBusquedaDispositivosBTLE()
        );
    }


    // ------------------------------------------------------------
    // tienePermisosBluetooth()
    // --> tiene_permisos: B
    // ------------------------------------------------------------
    //
    // Comprueba los permisos necesarios para realizar
    // el escaneo Bluetooth.
    // ------------------------------------------------------------
    private boolean tienePermisosBluetooth() {

        boolean permisoUbicacion =
                ContextCompat.checkSelfPermission(
                        this,
                        Manifest.permission.ACCESS_FINE_LOCATION
                )
                        ==
                        PackageManager.PERMISSION_GRANTED

                        &&

                        ContextCompat.checkSelfPermission(
                                this,
                                Manifest.permission.ACCESS_COARSE_LOCATION
                        )
                                ==
                                PackageManager.PERMISSION_GRANTED;


        if (
                Build.VERSION.SDK_INT
                        >=
                        Build.VERSION_CODES.S
        ) {

            boolean permisoScan =
                    ContextCompat.checkSelfPermission(
                            this,
                            Manifest.permission.BLUETOOTH_SCAN
                    )
                            ==
                            PackageManager.PERMISSION_GRANTED;


            boolean permisoConnect =
                    ContextCompat.checkSelfPermission(
                            this,
                            Manifest.permission.BLUETOOTH_CONNECT
                    )
                            ==
                            PackageManager.PERMISSION_GRANTED;


            return permisoScan
                    &&
                    permisoConnect
                    &&
                    permisoUbicacion;
        }


        return permisoUbicacion;
    }


    // ------------------------------------------------------------
    // solicitarPermisosBluetooth() -->
    // ------------------------------------------------------------
    //
    // Solicita los permisos necesarios para utilizar BLE.
    // ------------------------------------------------------------
    private void solicitarPermisosBluetooth() {

        if (tienePermisosBluetooth()) {

            textoEstado.setText(
                    "Permisos Bluetooth disponibles."
            );

            return;
        }


        List<String> permisos =
                new ArrayList<>();


        if (
                Build.VERSION.SDK_INT
                        >=
                        Build.VERSION_CODES.S
        ) {

            permisos.add(
                    Manifest.permission.BLUETOOTH_SCAN
            );

            permisos.add(
                    Manifest.permission.BLUETOOTH_CONNECT
            );
        }


        permisos.add(
                Manifest.permission.ACCESS_COARSE_LOCATION
        );


        permisos.add(
                Manifest.permission.ACCESS_FINE_LOCATION
        );


        ActivityCompat.requestPermissions(
                this,
                permisos.toArray(
                        new String[0]
                ),
                CODIGO_PERMISOS_BLUETOOTH
        );
    }


    // ------------------------------------------------------------
    // serviciosUbicacionActivos()
    // --> activos: B
    // ------------------------------------------------------------
    //
    // Comprueba si los servicios de ubicación están activos.
    // ------------------------------------------------------------
    private boolean serviciosUbicacionActivos() {

        LocationManager locationManager =
                (LocationManager)
                        getSystemService(
                                LOCATION_SERVICE
                        );


        if (
                Build.VERSION.SDK_INT
                        >=
                        Build.VERSION_CODES.P
        ) {

            return locationManager.isLocationEnabled();
        }


        return locationManager.isProviderEnabled(
                LocationManager.GPS_PROVIDER
        )

                ||

                locationManager.isProviderEnabled(
                        LocationManager.NETWORK_PROVIDER
                );
    }


    // ------------------------------------------------------------
    // buscarEliaGTI() -->
    // ------------------------------------------------------------
    //
    // Inicia la búsqueda del beacon Elia_GTI.
    // ------------------------------------------------------------
    private void buscarEliaGTI() {

        if (!prepararEscaneo()) {

            return;
        }


        textoEstado.setText(
                "Buscando Elia_GTI..."
        );


        iniciarEscaneo();
    }


    // ------------------------------------------------------------
    // iniciarEscaneo() -->
    // ------------------------------------------------------------
    //
    // Inicia el escaneo Bluetooth Low Energy.
    // ------------------------------------------------------------
    @SuppressLint("MissingPermission")
    private void iniciarEscaneo() {

        if (!tienePermisosBluetooth()) {

            solicitarPermisosBluetooth();

            return;
        }


        if (bluetoothLeScanner == null) {

            textoEstado.setText(
                    "No se ha podido obtener el escáner BLE."
            );

            return;
        }


        // Detener un posible escaneo anterior.
        bluetoothLeScanner.stopScan(
                scanCallback
        );


        bluetoothLeScanner.startScan(
                scanCallback
        );


        Log.d(
                ETIQUETA_LOG,
                "Escaneo BLE iniciado"
        );
    }


    // ------------------------------------------------------------
    // detenerBusquedaDispositivosBTLE() -->
    // ------------------------------------------------------------
    //
    // Detiene el escaneo Bluetooth Low Energy activo.
    // ------------------------------------------------------------
    @SuppressLint("MissingPermission")
    private void detenerBusquedaDispositivosBTLE() {

        if (
                bluetoothLeScanner == null
                        ||
                        !tienePermisosBluetooth()
        ) {

            return;
        }


        bluetoothLeScanner.stopScan(
                scanCallback
        );


        textoEstado.setText(
                "Búsqueda detenida."
        );


        Log.d(
                ETIQUETA_LOG,
                "Escaneo BLE detenido"
        );
    }


    // ------------------------------------------------------------
    // prepararEscaneo()
    // --> preparado: B
    // ------------------------------------------------------------
    //
    // Comprueba que la aplicación está preparada para
    // realizar el escaneo BLE.
    // ------------------------------------------------------------
    private boolean prepararEscaneo() {

        if (!tienePermisosBluetooth()) {

            textoEstado.setText(
                    "Faltan permisos Bluetooth/ubicación."
            );


            solicitarPermisosBluetooth();


            return false;
        }


        if (!serviciosUbicacionActivos()) {

            textoEstado.setText(
                    "Activa Ubicación/GPS para escanear BLE."
            );


            return false;
        }


        if (!bluetoothAdapter.isEnabled()) {

            textoEstado.setText(
                    "Activa el Bluetooth del teléfono."
            );


            return false;
        }


        bluetoothLeScanner =
                bluetoothAdapter.getBluetoothLeScanner();


        if (bluetoothLeScanner == null) {

            textoEstado.setText(
                    "No se ha podido obtener el escáner BLE."
            );


            return false;
        }


        return true;
    }


    // ------------------------------------------------------------
    // resultado: ResultadoBLE
    // --> procesarResultadoBLE() -->
    // ------------------------------------------------------------
    //
    // Procesa un resultado BLE.
    //
    // Cualquier dispositivo que no sea Elia_GTI
    // se ignora directamente.
    // ------------------------------------------------------------
    @SuppressLint("MissingPermission")
    private void procesarResultadoBLE(
            ScanResult resultado
    ) {

        ScanRecord scanRecord =
                resultado.getScanRecord();


        if (scanRecord == null) {

            return;
        }


        String nombre =
                scanRecord.getDeviceName();


        if (
                nombre == null
                        &&
                        resultado.getDevice() != null
        ) {

            nombre =
                    resultado.getDevice().getName();
        }


        // --------------------------------------------------------
        // Ignorar cualquier dispositivo que no sea Elia_GTI.
        // --------------------------------------------------------

        if (
                !NOMBRE_BEACON.equals(
                        nombre
                )
        ) {

            return;
        }


        Log.d(
                ETIQUETA_LOG,
                "Elia_GTI encontrado | RSSI="
                        + resultado.getRssi()
        );


        procesarIBeacon(
                scanRecord
        );
    }


    // ------------------------------------------------------------
    // registro: RegistroBLE
    // --> procesarIBeacon() -->
    // ------------------------------------------------------------
    //
    // Extrae la información iBeacon del anuncio recibido.
    //
    // Conversión:
    //
    // UUID  -> identificador_sensor
    // Minor -> valor
    // Major byte superior -> tipo
    // Major byte inferior -> contador
    // ------------------------------------------------------------
    private void procesarIBeacon(
            ScanRecord registro
    ) {

        TramaIBeacon trama =
                obtenerTramaIBeacon(
                        registro
                );


        if (trama == null) {

            Log.d(
                    ETIQUETA_LOG,
                    "Elia_GTI encontrado, pero la trama no es iBeacon."
            );


            return;
        }


        int major =
                Utilidades.bytesToInt(
                        trama.getMajor()
                );


        int tipo =
                (major >> 8)
                        &
                        0xFF;


        int contador =
                major
                        &
                        0xFF;


        int valor =
                Utilidades.bytesToInt(
                        trama.getMinor()
                );


        String identificadorSensor =
                formatearUUID(
                        trama.getUUID()
                );


        // --------------------------------------------------------
        // Evitar procesar varias veces la misma medición.
        // --------------------------------------------------------

        if (
                contador
                        ==
                        ultimoContadorProcesado
        ) {

            return;
        }


        ultimoContadorProcesado =
                contador;


        String fechaHora =
                obtenerFechaHoraActual();


        Medicion medicion =
                new Medicion(
                        valor,
                        fechaHora,
                        identificadorSensor,
                        tipo
                );


        Log.d(
                ETIQUETA_LOG,
                "NUEVA MEDICION"
                        + " | valor="
                        + valor
                        + " | fecha_hora="
                        + fechaHora
                        + " | identificador_sensor="
                        + identificadorSensor
                        + " | tipo="
                        + tipo
                        + " | contador="
                        + contador
        );


        textoEstado.setText(
                "Nueva medición: "
                        + valor
        );


        enviarMedicion(
                medicion
        );
    }


    // ------------------------------------------------------------
    // registro: RegistroBLE
    // --> obtenerTramaIBeacon()
    // --> trama: TramaIBeacon
    // ------------------------------------------------------------
    //
    // Obtiene los datos iBeacon del bloque Manufacturer Data.
    //
    // Android entrega los datos del fabricante sin incluir
    // el Company ID, por lo que se reconstruyen los 30 bytes
    // que espera TramaIBeacon.
    // ------------------------------------------------------------
    private TramaIBeacon obtenerTramaIBeacon(
            ScanRecord registro
    ) {

        byte[] datosFabricante =
                registro.getManufacturerSpecificData(
                        COMPANY_ID_APPLE
                );


        if (
                datosFabricante == null
                        ||
                        datosFabricante.length < 23
        ) {

            return null;
        }


        // --------------------------------------------------------
        // Un iBeacon comienza por:
        //
        // 0x02 -> tipo
        // 0x15 -> longitud
        // --------------------------------------------------------

        if (
                (datosFabricante[0] & 0xFF)
                        !=
                        0x02

                        ||

                        (datosFabricante[1] & 0xFF)
                                !=
                                0x15
        ) {

            return null;
        }


        byte[] bytes =
                new byte[30];


        // ADV Flags
        bytes[0] =
                0x02;

        bytes[1] =
                0x01;

        bytes[2] =
                0x06;


        // ADV Header
        bytes[3] =
                0x1A;

        bytes[4] =
                (byte) 0xFF;


        // Company ID = Apple 0x004C
        bytes[5] =
                0x4C;

        bytes[6] =
                0x00;


        // Copiar:
        //
        // iBeacon Type
        // iBeacon Length
        // UUID
        // Major
        // Minor
        // TxPower
        System.arraycopy(
                datosFabricante,
                0,
                bytes,
                7,
                23
        );


        return new TramaIBeacon(
                bytes
        );
    }


    // ------------------------------------------------------------
    // uuid: [ N ]_16
    // --> formatearUUID()
    // --> identificador_sensor: Text
    // ------------------------------------------------------------
    //
    // Convierte los 16 bytes del UUID al formato:
    //
    // XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
    // ------------------------------------------------------------
    private String formatearUUID(
            byte[] uuid
    ) {

        String texto =
                Utilidades.bytesToHexString(
                        uuid
                );


        return texto.substring(
                0,
                8
        )
                + "-"
                + texto.substring(
                        8,
                        12
                )
                + "-"
                + texto.substring(
                        12,
                        16
                )
                + "-"
                + texto.substring(
                        16,
                        20
                )
                + "-"
                + texto.substring(
                        20,
                        32
                );
    }


    // ------------------------------------------------------------
    // obtenerFechaHoraActual()
    // --> fecha_hora: Text
    // ------------------------------------------------------------
    //
    // Genera la fecha y hora utilizada por Medicion.
    // ------------------------------------------------------------
    private String obtenerFechaHoraActual() {

        SimpleDateFormat formato =
                new SimpleDateFormat(
                        "yyyy-MM-dd HH:mm:ss",
                        Locale.getDefault()
                );


        return formato.format(
                new Date()
        );
    }


    // ------------------------------------------------------------
    // medicion: Medicion
    // --> enviarMedicion() -->
    // ------------------------------------------------------------
    //
    // Entrega la medición a LogicaFakeTelefono.
    //
    // La operación se realiza en otro hilo para no bloquear
    // la interfaz de Android.
    // ------------------------------------------------------------
    private void enviarMedicion(
            Medicion medicion
    ) {

        new Thread(
                () -> {

                    boolean resultado =
                            logicaFakeTelefono.leerDatos(
                                    medicion
                            );


                    Log.d(
                            ETIQUETA_LOG,
                            resultado
                                    ?
                                    "Medición enviada correctamente."
                                    :
                                    "No se ha podido enviar la medición."
                    );


                    runOnUiThread(
                            () -> {

                                if (resultado) {

                                    textoEstado.setText(
                                            "Medición enviada: "
                                                    + medicion.getValor()
                                    );

                                } else {

                                    textoEstado.setText(
                                            "Medición detectada, pero no enviada."
                                    );
                                }
                            }
                    );
                }
        ).start();
    }


    // ------------------------------------------------------------
    // Callback del escaneo BLE.
    // ------------------------------------------------------------
    private final ScanCallback scanCallback =
            new ScanCallback() {


                // ------------------------------------------------
                // resultado: ResultadoBLE
                // --> onScanResult() -->
                // ------------------------------------------------
                @Override
                public void onScanResult(
                        int callbackType,
                        ScanResult result
                ) {

                    super.onScanResult(
                            callbackType,
                            result
                    );


                    procesarResultadoBLE(
                            result
                    );
                }


                // ------------------------------------------------
                // resultados: [ ResultadoBLE ]
                // --> onBatchScanResults() -->
                // ------------------------------------------------
                @Override
                public void onBatchScanResults(
                        List<ScanResult> results
                ) {

                    super.onBatchScanResults(
                            results
                    );


                    for (
                            ScanResult result
                            :
                            results
                    ) {

                        procesarResultadoBLE(
                                result
                        );
                    }
                }


                // ------------------------------------------------
                // codigo_error: Z
                // --> onScanFailed() -->
                // ------------------------------------------------
                @Override
                public void onScanFailed(
                        int errorCode
                ) {

                    super.onScanFailed(
                            errorCode
                    );


                    Log.e(
                            ETIQUETA_LOG,
                            "Error durante el escaneo BLE: "
                                    + errorCode
                    );


                    textoEstado.setText(
                            "Error durante el escaneo BLE: "
                                    + errorCode
                    );
                }
            };


    // ------------------------------------------------------------
    // resultado: ResultadoPermisos
    // --> onRequestPermissionsResult() -->
    // ------------------------------------------------------------
    //
    // Procesa la respuesta a la solicitud de permisos.
    // ------------------------------------------------------------
    @Override
    public void onRequestPermissionsResult(
            int requestCode,
            @NonNull String[] permissions,
            @NonNull int[] grantResults
    ) {

        super.onRequestPermissionsResult(
                requestCode,
                permissions,
                grantResults
        );


        if (
                requestCode
                        !=
                        CODIGO_PERMISOS_BLUETOOTH
        ) {

            return;
        }


        if (tienePermisosBluetooth()) {

            textoEstado.setText(
                    "Permisos Bluetooth y ubicación concedidos."
            );

        } else {

            textoEstado.setText(
                    "Faltan permisos Bluetooth o ubicación."
            );
        }
    }


    // ------------------------------------------------------------
    // onDestroy() -->
    // ------------------------------------------------------------
    //
    // Detiene el escaneo antes de destruir la actividad.
    // ------------------------------------------------------------
    @SuppressLint("MissingPermission")
    @Override
    protected void onDestroy() {

        if (
                bluetoothLeScanner != null
                        &&
                        tienePermisosBluetooth()
        ) {

            bluetoothLeScanner.stopScan(
                    scanCallback
            );
        }


        super.onDestroy();
    }
}