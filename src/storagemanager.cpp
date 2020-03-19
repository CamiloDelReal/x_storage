#include <QTimer>
#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif

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
    connect(StorageNotificationReciever::getInstance(), &StorageNotificationReciever::deviceMounted, this, &StorageManager::scheduleUpdate);
    connect(StorageNotificationReciever::getInstance(), &StorageNotificationReciever::deviceUnmounted, this, &StorageManager::scheduleUpdate);
#endif
}

bool StorageManager::requestPermission()
{
    bool granted = true;

#if defined (Q_OS_ANDROID)
    QStringList permissions;
    permissions << "android.permission.READ_EXTERNAL_STORAGE";
    permissions << "android.permission.WRITE_EXTERNAL_STORAGE";
    QtAndroid::PermissionResultMap result = QtAndroid::requestPermissionsSync(permissions);
    QHash<QString, QtAndroid::PermissionResult>::Iterator it = result.begin();
    while(granted && it != result.end())
    {
        granted = (it.value() == QtAndroid::PermissionResult::Granted);
        it++;
    }
#elif defined (Q_OS_IOS)
#elif defined (Q_OS_WIN)
    granted = true;
#elif defined (Q_OS_LINUX)
    granted = true
#endif

    return granted;
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

void StorageManager::scheduleUpdate()
{
    StorageManager *receiver = this;
    QTimer::singleShot(2000, this, [receiver]() {
        receiver->update();
    });
}

void StorageManager::update()
{
    //In background
    m_storageFactory.update(true);
}

