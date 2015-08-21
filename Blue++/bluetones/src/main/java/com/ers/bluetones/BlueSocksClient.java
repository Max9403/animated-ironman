package com.ers.bluetones;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.UUID;

/**
 * Created by BClaa on 8/17/2015.
 */
public class BlueSocksClient implements Runnable {

    public static int STARTING = 0;
    public static int STARTED = 1;
    public static int CLOSED = 2;
    public int STATUS = STARTING;

    private BlueService contextRelay;
    protected final UUID uuid;
    private final String name;
    protected BluetoothCallback callback;

    BluetoothSocket socky;
    BluetoothAdapter bluetooth = BluetoothAdapter.getDefaultAdapter();

    protected DataOutputStream out;
    private DataInputStream in;

    public BlueSocksClient(BlueService context, BluetoothCallback callback, UUID uuid, String name) {
        this.callback = callback;
        this.contextRelay = context;
        this.uuid = uuid;
        this.name = name;
    }

    @Override
    public void run() {

        if (bluetooth != null) {
            try {
                BluetoothDevice device = bluetooth.getRemoteDevice(name);
                socky = device.createInsecureRfcommSocketToServiceRecord(uuid);
                socky.connect();
                in = new DataInputStream(socky.getInputStream());
                out = new DataOutputStream(socky.getOutputStream());
                STATUS = STARTED;
                while(true) {
                    String idd = in.readUTF();
                    int type = in.readInt();

                    byte[] data = new byte[type];
                    in.read(data);
                    out.writeUTF(idd);
                    out.writeInt(type);
                    out.write(data);
                    out.flush();
                    contextRelay.out.writeUTF(idd);
                    contextRelay.out.write(data);
                    contextRelay.out.flush();
                    System.out.println("asd : " + uuid.toString() + " : " + new String(data));
                    //contextRelay.sendData();
                }
            } catch (IOException e) {
                e.printStackTrace();
                STATUS = CLOSED;
            }
        }
    }

    public void close() throws IOException {
        if (socky != null) {
            socky.close();
        }
    }

    public interface BluetoothCallback {
    }
}