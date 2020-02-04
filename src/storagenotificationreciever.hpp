#ifndef STORAGENOTIFICATIONRECIEVER_HPP
#define STORAGENOTIFICATIONRECIEVER_HPP

#include <QObject>

class StorageNotificationReciever : public QObject
{
    Q_OBJECT
public:
    static StorageNotificationReciever *getInstance();

public slots:
    void processDeviceMounted();
    void processDeviceUnmounted();

signals:
    void deviceMounted();
    void deviceUnmounted();

private:
    explicit StorageNotificationReciever();
};

#endif // STORAGENOTIFICATIONRECIEVER_HPP
