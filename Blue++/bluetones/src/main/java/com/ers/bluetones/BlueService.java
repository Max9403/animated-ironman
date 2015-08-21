package com.ers.bluetones;

import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.IBinder;
import android.widget.Toast;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Arrays;
import java.util.UUID;

/**
 * Created by BClaa on 8/16/2015.
 */
public class BlueService extends Service implements Runnable {


    private static final UUID[] MY_UUIDs = new UUID[] {
            UUID.fromString("00001101-0000-1000-8000-00805F9B34F4"),
            UUID.fromString("00001101-0000-1000-8000-00805F9B34F5"),
            UUID.fromString("00001101-0000-1000-8000-00805F9B34F6")
    };

    private Thread thread;
    protected boolean run = true;

    protected boolean server = false;

    private DataInputStream in;
    protected DataOutputStream out;

    protected BlueSocksServer[] devices = new BlueSocksServer[3];

    BluetoothAdapter bluetooth = BluetoothAdapter.getDefaultAdapter();

    private final BroadcastReceiver receiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (action.equals(BluetoothAdapter.ACTION_STATE_CHANGED)) {
                System.out.println(intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, BluetoothAdapter.STATE_OFF));
                if (out != null) {
                    try {
                        out.writeUTF("APP");
                        out.writeByte(0x0);
                        out.writeInt(intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, BluetoothAdapter.STATE_OFF));
                        out.flush();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            } else if (action.equals(BluetoothDevice.ACTION_FOUND)) {
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                System.out.println(device.getName() + " : " + device.getAddress() + " : " + Arrays.toString(device.getUuids()));
                try {
                    out.writeUTF("APP");
                    out.writeByte(0x1);
                    out.writeUTF(device.getName() + " : " + device.getAddress());
                    out.flush();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            } else if (action.equals(BluetoothAdapter.ACTION_DISCOVERY_FINISHED)) {
                System.out.println("FINIShed Searching");
                try {
                    out.writeUTF("APP");
                    out.writeByte(0x2);
                    out.flush();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    };
    private BlueSocksClient connecton;

    public void sendData(byte[] data) throws IOException {
        out.write(data);
        out.flush();
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        //System.out.println("FFS");
        if (bluetooth == null) {
            throw new IllegalStateException("Bluetooth is not supported on this device");
        }
        Thread.setDefaultUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() {
            @Override
            public void uncaughtException(Thread thread, Throwable ex) {
                ex.printStackTrace();
            }
        });
        IntentFilter filter = new IntentFilter();
        filter.addAction(BluetoothAdapter.ACTION_STATE_CHANGED);
        filter.addAction(BluetoothDevice.ACTION_FOUND);
        filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        registerReceiver(receiver, filter);
        thread = new Thread(this);
        thread.start();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Toast.makeText(this, "Bluetooth shutting down", Toast.LENGTH_LONG).show();
    }

    @Override
    public void run() {
        try {
            ServerSocket serverSocket = new ServerSocket(23453);
            Socket socket = serverSocket.accept();
            out = new DataOutputStream(socket.getOutputStream());
            in = new DataInputStream(socket.getInputStream());
            while (run) {
                System.out.println("LOOP");
                byte read = in.readByte();
                switch (read) {
                    case 0:
                        if (!bluetooth.isEnabled()) {
                            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                            enableBtIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                            startActivity(enableBtIntent);
                        }
                        break;
                    case 1:
                        Intent discoverableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
                        discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
                        discoverableIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                        startActivity(discoverableIntent);
                        break;
                    case 2:
                        bluetooth.startDiscovery();
                        break;
                    case 3:
                        bluetooth.cancelDiscovery();
                        break;
                    case 4:
                        server = in.readByte() >= 1;
                        break;
                    case 5:
                        if (server) {
                            System.out.println("Serving");
                            for(byte cc = 0; cc < 3; cc++) {
                                devices[cc] = new BlueSocksServer(this, null, MY_UUIDs[cc], "AIHost");
                                new Thread(devices[cc]).start();
                            }
                        } else {
                            devi:
                            for (byte cc = 0; cc < 3; cc++) {
                                System.out.println("CONNECTING!");
                                connecton = new BlueSocksClient(this, null, MY_UUIDs[cc], in.readUTF());
                                new Thread(connecton).start();
                                while (connecton.STATUS != BlueSocksClient.STARTED) {
                                    Thread.sleep(10, 0);
                                    if(connecton.STATUS == BlueSocksClient.CLOSED) {
                                        continue devi;
                                    }
                                }
                                System.out.println("CONNECTED!");
                                if(connecton.STATUS == BlueSocksClient.STARTED) {
                                    break;
                                }
                            }
                        }
                        /*int setDev = devices[0] == null ? 0 : devices[1] == null ? 1 : devices[2] == null ? 2 : -1;
                        if (setDev > -1) {
                            BluetoothDevice device = bluetooth.getRemoteDevice(in.readUTF());
                            devices[setDev] = device.createInsecureRfcommSocketToServiceRecord(MY_UUID);
                            devices[setDev].connect();
                            System.out.println("\n...Connection established and data link opened...");
                        }*/
                        break;
                    case 6:
                        if (devices[0] != null) {
                            devices[0].close();
                            devices[0] = null;
                        }
                        if (devices[1] != null) {
                            devices[1].close();
                            devices[1] = null;
                        }
                        if (devices[2] != null) {
                            devices[2].close();
                            devices[2] = null;
                        }
                    case 7:
                        if(server) {
                            int size = in.readInt();
                            byte[] data = new byte[size];
                            for(BlueSocksServer server : devices) {
                                if(server != null && server.STATUS == BlueSocksServer.STARTED) {
                                    server.out.writeUTF(server.uuid.toString());
                                    server.out.writeInt(size);
                                    server.out.write(data);
                                }
                            }
                        } else {
                            if(connecton != null && connecton.STATUS == BlueSocksClient.STARTED) {
                                int size = in.readInt();
                                byte[] data = new byte[size];
                                in.read(data);
                                connecton.out.writeUTF(connecton.uuid.toString());
                                connecton.out.writeInt(size);
                                connecton.out.write(data);
                                connecton.out.flush();
                            }
                        }
                    case 10:
                        run = false;
                        break;
                }
            }
            serverSocket.close();
        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        }
    }
}
