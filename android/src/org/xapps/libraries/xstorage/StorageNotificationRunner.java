package org.xapps.libraries.xstorage;

import android.app.Activity;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbManager;


public class StorageNotificationRunner implements Runnable
{
    private Activity m_activity;
    private StorageNotificationReciever m_mediaReceiver;
    private StorageNotificationReciever m_usbReceiver;

    public StorageNotificationRunner(Activity activity) {
        m_activity = activity;
    }

    @Override
    public void run() {
        IntentFilter mediaFilter = new IntentFilter();
        mediaFilter.addAction(Intent.ACTION_MEDIA_MOUNTED);
        mediaFilter.addAction(Intent.ACTION_MEDIA_UNMOUNTED);
        mediaFilter.addDataScheme("file");

        IntentFilter usbFilter = new IntentFilter();
        usbFilter.addAction(UsbManager.ACTION_USB_DEVICE_ATTACHED);
        usbFilter.addAction(UsbManager.ACTION_USB_DEVICE_DETACHED);

        m_mediaReceiver = new StorageNotificationReciever();
        m_usbReceiver = new StorageNotificationReciever();

        m_activity.registerReceiver(m_mediaReceiver, mediaFilter);
        m_activity.registerReceiver(m_usbReceiver, usbFilter);
    }

    public void clearReceiver() {
        m_activity.unregisterReceiver(m_mediaReceiver);
        m_activity.unregisterReceiver(m_usbReceiver);
    }
}
