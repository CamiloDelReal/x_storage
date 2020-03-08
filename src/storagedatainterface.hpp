#ifndef STORAGEDATAINTERFACE_HPP
#define STORAGEDATAINTERFACE_HPP

#include <QObject>

#include "storagedevice.hpp"


class StorageDataInterface : public QObject
{
    Q_OBJECT
public:
    explicit StorageDataInterface(QObject *parent = nullptr);

    virtual StorageDevice getPrimaryDevice() = 0;
    virtual QList<StorageDevice> getAllStoragesData() = 0;
    virtual StorageDevice getStorageDeviceByFolder(const QString &folderPath) = 0;
};

#endif // STORAGEDATAINTERFACE_HPP
