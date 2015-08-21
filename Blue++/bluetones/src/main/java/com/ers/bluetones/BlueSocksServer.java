package com.ers.bluetones;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.Context;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.UUID;

/**
 * Created by BClaa on 8/16/2015.
 */
public class BlueSocksServer implements Runnable {

    public static int STARTING = 0;
    public static int STARTED = 1;
    public static int CLOSED = 2;
    public int STATUS = STARTING;

    private BlueService contextRelay;
    public final UUID uuid;
    private final String name;
    protected BluetoothCallback callback;

    BluetoothServerSocket socky;
    BluetoothAdapter bluetooth = BluetoothAdapter.getDefaultAdapter();
    private BluetoothSocket bSocket;
    protected DataOutputStream out;
    private DataInputStream in;

    public BlueSocksServer(BlueService context, BluetoothCallback callback, UUID uuid, String name) {
        this.callback = callback;
        this.contextRelay = context;
        this.uuid = uuid;
        this.name = name;
    }

    @Override
    public void run() {

        if(bluetooth != null)
        {
            try {
                socky = bluetooth.listenUsingInsecureRfcommWithServiceRecord(name, uuid);
                bSocket = socky.accept();
                in = new DataInputStream(bSocket.getInputStream());
                out = new DataOutputStream(bSocket.getOutputStream());
                STATUS = STARTED;
                while(true) {
                    String idd = in.readUTF();
                    int type = in.readInt();
                    byte[] data = new byte[type];
                    in.read(data);
                    for(BlueSocksServer dev : contextRelay.devices) {
                        if(!dev.uuid.equals(uuid) &&  dev.out != null) {
                            dev.out.writeUTF(idd);
                            dev.out.writeInt(type);
                            dev.out.write(data);
                            dev.out.flush();
                        }
                    }
                    contextRelay.out.writeUTF(idd);
                    contextRelay.out.write(data);
                    contextRelay.out.flush();
                    //contextRelay.sendData();
                }
            } catch (IOException e) {
                e.printStackTrace();
                STATUS = CLOSED;
            }
        }
    }

    public void close() throws IOException {
        if(socky != null) {
            socky.close();
        }
    }

    public interface BluetoothCallback {
    }
}
