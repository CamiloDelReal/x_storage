package org.xapps.libraries.xstorage;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class StorageNotificationReciever extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        if (intent.getAction().equals(Intent.ACTION_MEDIA_MOUNTED)) {
            StorageNotificationNative.deviceMounted();
        }
        else if (intent.getAction().equals(Intent.ACTION_MEDIA_UNMOUNTED)) {
            StorageNotificationNative.deviceUnmounted();
        }
    }
}
