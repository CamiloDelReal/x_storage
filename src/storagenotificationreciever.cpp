#include "storagenotificationreciever.hpp"


StorageNotificationReciever *StorageNotificationReciever::getInstance()
{
    static StorageNotificationReciever *uniqueInstance = nullptr;

    if(uniqueInstance == nullptr)
        uniqueInstance = new StorageNotificationReciever();

    return uniqueInstance;
}

void StorageNotificationReciever::processDeviceMounted()
{
    emit deviceMounted();
}

void StorageNotificationReciever::processDeviceUnmounted()
{
    emit deviceUnmounted();
}

StorageNotificationReciever::StorageNotificationReciever()
    : QObject(0)
{
}
