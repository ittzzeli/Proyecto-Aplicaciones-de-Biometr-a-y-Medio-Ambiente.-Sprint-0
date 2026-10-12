package com.example.biometriasprint0;

// ------------------------------------------------------------
// Archivo: MainActivity.java
// Descripción:
// Actividad principal de la aplicación Android del Sprint 0.
// Gestiona permisos, estado del Bluetooth y escaneo BLE.
//
// Autor: Elia
// Fecha: 06/10/2026
// Aportación:
// Adaptación del ejemplo BTLE proporcionado en la asignatura
// para localizar el beacon "Elia_GTI".
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

import java.util.ArrayList;
import java.util.List;


// ------------------------------------------------------------
// MainActivity
//
// Responsabilidad:
// Gestionar la interacción con el usuario y coordinar el
// escaneo Bluetooth Low Energy.
//
// Permite:
// - buscar todos los dispositivos BLE;
// - detener el escaneo;
// - buscar específicamente "Elia_GTI".
//
// En esta fase todavía no se interpreta la trama iBeacon
// ni se realizan peticiones REST.
// ------------------------------------------------------------
public class MainActivity extends AppCompatActivity {

    private static final String ETIQUETA_LOG = ">>>>";

    private static final String NOMBRE_BEACON = "Elia_GTI";

    private static final int CODIGO_PERMISOS_BLUETOOTH = 100;


    private BluetoothAdapter bluetoothAdapter;

    private BluetoothLeScanner bluetoothLeScanner;

    private TextView textoEstado;


    // false -> procesar todos los dispositivos.
    // true  -> procesar únicamente Elia_GTI.
    private boolean buscarSoloEliaGTI = false;


    // ------------------------------------------------------------
    // onCreate() -->
    // ------------------------------------------------------------
    //
    // Inicializa la actividad, enlaza los controles de la
    // interfaz, obtiene el adaptador Bluetooth y solicita
    // los permisos necesarios.
    // ------------------------------------------------------------
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);


        Button botonBuscarTodos =
                findViewById(R.id.botonBuscarTodos);

        Button botonDetenerBusqueda =
                findViewById(R.id.botonDetenerBusqueda);

        Button botonBuscarEliaGTI =
                findViewById(R.id.botonBuscarEliaGTI);

        textoEstado =
                findViewById(R.id.textoEstado);


        BluetoothManager bluetoothManager =
                (BluetoothManager) getSystemService(
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


        botonBuscarTodos.setOnClickListener(
                v -> buscarTodosLosDispositivosBTLE()
        );


        botonDetenerBusqueda.setOnClickListener(
                v -> detenerBusquedaDispositivosBTLE()
        );


        botonBuscarEliaGTI.setOnClickListener(
                v -> buscarEliaGTI()
        );
    }


    // ------------------------------------------------------------
    // tienePermisosBluetooth() --> tiene_permisos: B
    // ------------------------------------------------------------
    //
    // Comprueba que la aplicación dispone de los permisos
    // Bluetooth y ubicación necesarios para realizar el escaneo.
    // ------------------------------------------------------------
    private boolean tienePermisosBluetooth() {

        boolean permisoUbicacion =
                ContextCompat.checkSelfPermission(
                        this,
                        Manifest.permission.ACCESS_FINE_LOCATION
                ) == PackageManager.PERMISSION_GRANTED

                        &&

                        ContextCompat.checkSelfPermission(
                                this,
                                Manifest.permission.ACCESS_COARSE_LOCATION
                        ) == PackageManager.PERMISSION_GRANTED;


        if (
                Build.VERSION.SDK_INT
                        >= Build.VERSION_CODES.S
        ) {

            boolean permisoScan =
                    ContextCompat.checkSelfPermission(
                            this,
                            Manifest.permission.BLUETOOTH_SCAN
                    ) == PackageManager.PERMISSION_GRANTED;


            boolean permisoConnect =
                    ContextCompat.checkSelfPermission(
                            this,
                            Manifest.permission.BLUETOOTH_CONNECT
                    ) == PackageManager.PERMISSION_GRANTED;


            return permisoScan
                    && permisoConnect
                    && permisoUbicacion;
        }


        return permisoUbicacion;
    }


    // ------------------------------------------------------------
    // solicitarPermisosBluetooth() -->
    // ------------------------------------------------------------
    //
    // Solicita los permisos Bluetooth y ubicación necesarios
    // dependiendo de la versión de Android.
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
                        >= Build.VERSION_CODES.S
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
                permisos.toArray(new String[0]),
                CODIGO_PERMISOS_BLUETOOTH
        );
    }


    // ------------------------------------------------------------
    // serviciosUbicacionActivos() --> activos: B
    // ------------------------------------------------------------
    //
    // Comprueba que los servicios de ubicación del teléfono
    // se encuentran activados.
    //
    // Algunos dispositivos requieren este ajuste para entregar
    // correctamente los resultados del escaneo BLE.
    // ------------------------------------------------------------
    private boolean serviciosUbicacionActivos() {

        LocationManager locationManager =
                (LocationManager) getSystemService(
                        LOCATION_SERVICE
                );


        if (
                Build.VERSION.SDK_INT
                        >= Build.VERSION_CODES.P
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
    // buscarTodosLosDispositivosBTLE() -->
    // ------------------------------------------------------------
    //
    // Inicia un escaneo BLE general sin filtrar los dispositivos.
    // ------------------------------------------------------------
    private void buscarTodosLosDispositivosBTLE() {

        if (!prepararEscaneo()) {

            return;
        }


        buscarSoloEliaGTI = false;


        textoEstado.setText(
                "Buscando todos los dispositivos BTLE..."
        );


        iniciarEscaneo();
    }


    // ------------------------------------------------------------
    // buscarEliaGTI() -->
    // ------------------------------------------------------------
    //
    // Inicia un escaneo BLE general, pero únicamente procesa
    // los resultados cuyo nombre sea "Elia_GTI".
    // ------------------------------------------------------------
    private void buscarEliaGTI() {

        if (!prepararEscaneo()) {

            return;
        }


        buscarSoloEliaGTI = true;


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
    //
    // No aplica filtros directamente en Android. Los resultados
    // se filtran posteriormente en procesarResultadoBLE().
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


        // Detenemos un posible escaneo anterior.
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
    // prepararEscaneo() --> preparado: B
    // ------------------------------------------------------------
    //
    // Comprueba:
    //
    // - permisos Bluetooth;
    // - permisos de ubicación;
    // - servicios de ubicación activos;
    // - Bluetooth activado;
    // - disponibilidad del escáner BLE.
    //
    // Devuelve true si el dispositivo está preparado.
    // ------------------------------------------------------------
    @SuppressLint("MissingPermission")
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
    // resultado: ResultadoBLE --> procesarResultadoBLE() -->
    // ------------------------------------------------------------
    //
    // Obtiene el nombre del dispositivo BLE recibido.
    //
    // Cuando se utiliza "Buscar Elia_GTI", descarta todos los
    // dispositivos que no correspondan a nuestro beacon.
    //
    // Solo se escribe en Logcat cuando se encuentra Elia_GTI,
    // evitando mostrar continuamente todos los anuncios BLE.
    // ------------------------------------------------------------
    @SuppressLint("MissingPermission")
    private void procesarResultadoBLE(
            ScanResult resultado
    ) {

        ScanRecord scanRecord =
                resultado.getScanRecord();


        String nombre = null;


        if (scanRecord != null) {

            nombre =
                    scanRecord.getDeviceName();
        }


        if (
                nombre == null
                        &&
                        resultado.getDevice() != null
        ) {

            nombre =
                    resultado.getDevice().getName();
        }


        if (nombre == null) {

            nombre = "(sin nombre)";
        }


        // --------------------------------------------------------
        // Si buscamos exclusivamente Elia_GTI,
        // descartamos el resto de dispositivos.
        // --------------------------------------------------------

        if (
                buscarSoloEliaGTI
                        &&
                        !NOMBRE_BEACON.equals(nombre)
        ) {

            return;
        }


        // --------------------------------------------------------
        // Solo registramos nuestro beacon para mantener
        // limpio Logcat.
        // --------------------------------------------------------

        if (NOMBRE_BEACON.equals(nombre)) {

            Log.d(
                    ETIQUETA_LOG,
                    "Elia_GTI encontrado | RSSI="
                            + resultado.getRssi()
            );
        }


        final String nombreFinal =
                nombre;


        runOnUiThread(
                () -> {

                    if (
                            NOMBRE_BEACON.equals(
                                    nombreFinal
                            )
                    ) {

                        textoEstado.setText(
                                "Elia_GTI encontrado."
                        );

                    } else {

                        textoEstado.setText(
                                "Dispositivo encontrado: "
                                        + nombreFinal
                        );
                    }
                }
        );
    }


    // ------------------------------------------------------------
    // Callback utilizado para recibir los resultados del
    // escaneo Bluetooth Low Energy.
    // ------------------------------------------------------------
    private final ScanCallback scanCallback =
            new ScanCallback() {


                // ------------------------------------------------
                // resultado: ResultadoBLE --> onScanResult() -->
                // ------------------------------------------------
                //
                // Recibe individualmente cada dispositivo BLE
                // detectado y delega su procesamiento.
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
                // resultados: [ResultadoBLE]
                // --> onBatchScanResults() -->
                // ------------------------------------------------
                //
                // Procesa varios resultados BLE cuando Android
                // los entrega de forma agrupada.
                // ------------------------------------------------
                @Override
                public void onBatchScanResults(
                        List<ScanResult> results
                ) {

                    super.onBatchScanResults(
                            results
                    );


                    for (ScanResult result : results) {

                        procesarResultadoBLE(
                                result
                        );
                    }
                }


                // ------------------------------------------------
                // codigo_error: Z --> onScanFailed() -->
                // ------------------------------------------------
                //
                // Informa si Android detecta un error durante
                // el escaneo Bluetooth.
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
    // Recibe la respuesta del usuario a la solicitud de permisos
    // Bluetooth y ubicación.
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
                        != CODIGO_PERMISOS_BLUETOOTH
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
    // Detiene un posible escaneo BLE antes de destruir
    // la actividad.
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