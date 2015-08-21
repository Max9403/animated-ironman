package com.ers.blue;

import android.app.Activity;
import android.app.Fragment;
import android.app.FragmentTransaction;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

import java.util.ArrayList;

/**
 * Created by BClaa on 8/9/2015.
 */
public class BlueHelper {

    private static native void addDevice(int id, String name);
    private static native void clearDevices();

    private static ArrayList<BluetoothDevice> devices = new ArrayList<BluetoothDevice>();

    private static BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    // Create a BroadcastReceiver for ACTION_FOUND
    private static final BroadcastReceiver mReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            // When discovery finds a device
            if (BluetoothDevice.ACTION_FOUND.equals(intent.getAction())) {
                // Get the BluetoothDevice object from the Intent
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                devices.add(device);
                addDevice(devices.indexOf(device), device.getName());
            }
        }
    };

    static {
    }
    // Register the BroadcastReceiver
    public static boolean isSupported() {
        return mBluetoothAdapter != null;
    }

    public static boolean isEnabled() {
        return mBluetoothAdapter.isEnabled();
    }

    public static void enable(Context context) {
        if (!isEnabled()) {
            Intent biep = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            context.startActivity(biep);
            /*Fragment f = new Fragment() {

                private static final int ENABLE_BT = 23523423;

                @Override
                public void onAttach(Activity activity) {
                    super.onAttach(activity);
                    Intent biep = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                    startActivityForResult(biep, ENABLE_BT);
                }

                @Override
                public void onActivityResult(int requestCode, int resultCode,
                                             Intent data) {
                    if (requestCode == ENABLE_BT) {

                    }
                    super.onActivityResult(requestCode, resultCode, data);
                }

                //this is to verify the fragment has been removed.
                //you can log or put a breakpoint to verify
                @Override
                public void onDestroy() {
                    super.onDestroy();
                }
            };
            FragmentTransaction fragmentTransaction = ((Activity)context).getFragmentManager().beginTransaction();
            fragmentTransaction.add(f, "enable_bluetooth");
            fragmentTransaction.commit();*/
        }
    }

    public static void discover(Context context) {
        IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        context.registerReceiver(mReceiver, filter);
        Intent boop = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
        boop.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
        context.startActivity(boop);
        context.unregisterReceiver(mReceiver);
    }

}
