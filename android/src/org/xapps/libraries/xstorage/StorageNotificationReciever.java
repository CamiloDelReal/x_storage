package org.xapps.libraries.xstorage;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.hardware.usb.UsbManager;
import android.util.Log;


public class StorageNotificationReciever extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        if (intent.getAction().equals(Intent.ACTION_MEDIA_MOUNTED)
            || intent.getAction().equals(UsbManager.ACTION_USB_DEVICE_ATTACHED)) {
            StorageNotificationNative.deviceMounted();
        }
        else if (intent.getAction().equals(Intent.ACTION_MEDIA_UNMOUNTED)
            || intent.getAction().equals(UsbManager.ACTION_USB_DEVICE_DETACHED)) {
            StorageNotificationNative.deviceUnmounted();
        }
    }
}
