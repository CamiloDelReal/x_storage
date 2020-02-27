#include "storagemanager.hpp"


#if (defined (Q_OS_ANDROID) && !defined(NO_STORAGE_NOTIFICATION_LISTENER))
#include "storagenotificationreciever.hpp"
#endif

StorageManager::StorageManager(QObject *parent)
    : QObject(parent)
{
    m_storageModel.setFactory(&m_storageFactory);
    connect(&m_storageFactory, &StorageFactory::isWorkingChanged, this, &StorageManager::isWorkingChanged);

#if (defined (Q_OS_ANDROID) && !defined(NO_STORAGE_NOTIFICATION_LISTENER))
    connect(StorageNotificationReciever::getInstance(), &StorageNotificationReciever::deviceMounted, this, &StorageManager::update);
    connect(StorageNotificationReciever::getInstance(), &StorageNotificationReciever::deviceUnmounted, this, &StorageManager::update);
#endif
}

QObject *StorageManager::model()
{
    return dynamic_cast<QObject*>(&m_storageModel);
}

bool StorageManager::isWorking()
{
    return m_storageFactory.isWorking();
}

StorageDataInterface *StorageManager::getInterface()
{
    return dynamic_cast<StorageDataInterface*>(&m_storageFactory);
}

void StorageManager::initialize()
{
    //Not in background
    m_storageFactory.initialize(false);
}

void StorageManager::update()
{
    //In background
    m_storageFactory.update(true);
}

