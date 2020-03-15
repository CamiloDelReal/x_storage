#ifndef STORAGEFACTORY_HPP
#define STORAGEFACTORY_HPP

#include <QObject>

#include "storagefactoryworker.hpp"
#include "storagedevice.hpp"


class StorageFactory : public StorageFactoryWorker
{
    Q_OBJECT
public:
    explicit StorageFactory(QObject *parent = nullptr);

    StorageDevice getPrimaryDevice();
    QList<StorageDevice> getAllStoragesData();
    StorageDevice getStorageDeviceByFolder(const QString &folderPath);

    bool isWorking();

    void initialize(const bool &doInBackground);
    void update(const bool &doInBackground);

    quint32 storagesCount();
    StorageDevice storageAt(const quint32 &index);

signals:
    void requestStorageModelReset();

    void isWorkingChanged(const bool &isWorking);

private:
    enum StorageFactoryTask
    {
        INITIALIZE_STORAGE_DEVICES = 0,
        UPDATE_STORAGE_DEVICES
    };

    bool m_isWorking;
    StorageFactoryTask m_currentTask;
    QList<StorageDevice> m_storages;

    void execute();
    void setIsWorking(const bool &isWorking);

    void readStoragesData();
#ifdef Q_OS_ANDROID
    void startNotificationListener();
    QHash<QString, MountPoint> getMountedPoints();
#endif

    void initialize_bg();
    void update_bg();
};

#endif // STORAGEFACTORY_HPP
