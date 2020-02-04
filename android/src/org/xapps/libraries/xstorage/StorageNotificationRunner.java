package org.xapps.libraries.xstorage;

import android.app.Activity;
import android.content.Intent;
import android.content.IntentFilter;

public class StorageNotificationRunner implements Runnable
{
    private Activity m_activity;
    public StorageNotificationRunner(Activity activity) {
        m_activity = activity;
    }

    @Override
    public void run() {
        IntentFilter filter= new IntentFilter();
        filter.addAction(Intent.ACTION_MEDIA_MOUNTED);
        filter.addAction(Intent.ACTION_MEDIA_UNMOUNTED);
        filter.addDataScheme("file");

        m_activity.registerReceiver(new StorageNotificationReciever(), filter);
    }
}
