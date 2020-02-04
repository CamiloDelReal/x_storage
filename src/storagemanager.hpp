#ifndef STORAGEMANAGER_HPP
#define STORAGEMANAGER_HPP

#include <QObject>

#include "storagedevice.hpp"
#include "storagefactory.hpp"
#include "storagemodel.hpp"
#include "storagedatainterface.hpp"

class StorageManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* model READ model NOTIFY modelChanged)
    Q_PROPERTY(bool isWorking READ isWorking NOTIFY isWorkingChanged)
public:
    explicit StorageManager(QObject *parent = nullptr);

    QObject *model();
    bool isWorking();

    StorageDataInterface *getInterface();

    void initialize();

public slots:
    void update();

signals:
    void modelChanged(QObject* model);
    void isWorkingChanged(const bool &isWorking);

private:
    StorageFactory m_storageFactory;
    StorageModel m_storageModel;
};

#endif // STORAGEMANAGER_HPP
