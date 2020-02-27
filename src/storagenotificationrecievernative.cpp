#include <jni.h>
#include <QMetaObject>

#include "storagenotificationreciever.hpp"

//QUITAR
#include <QDebug>

#if (defined (Q_OS_ANDROID) && !defined(NO_STORAGE_NOTIFICATION_LISTENER))


static void deviceMounted(JNIEnv * /*env*/, jobject /*obj*/)
{
    QMetaObject::invokeMethod(StorageNotificationReciever::getInstance(),
                              "processDeviceMounted", Qt::QueuedConnection);
}

static void deviceUnmounted(JNIEnv * /*env*/, jobject /*obj*/)
{
    QMetaObject::invokeMethod(StorageNotificationReciever::getInstance(),
                              "processDeviceUnmounted", Qt::QueuedConnection);
}

static JNINativeMethod methods[] =
{
    { "deviceMounted", "()V", (void *)deviceMounted },
    { "deviceUnmounted", "()V", (void *)deviceUnmounted },
};

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK)
      return JNI_ERR;

    jclass javaClass = env->FindClass("org/xapps/libraries/xstorage/StorageNotificationNative");
    if (!javaClass)
    {
        return JNI_ERR;
    }

    if (env->RegisterNatives(javaClass, methods, sizeof(methods) / sizeof(methods[0])) < 0)
    {
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}

#endif
