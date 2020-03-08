#include "storagemodel.hpp"

//QUITAR
#include <QDebug>


StorageModel::StorageModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_factory = nullptr;
    m_currentItem = nullptr;
}

void StorageModel::setFactory(StorageFactory *factory)
{
    if(m_factory != nullptr)
    {
        m_factory->disconnect(this);
        m_factory = nullptr;
    }

    if(factory != nullptr)
    {
        m_factory = factory;
        connect(m_factory, &StorageFactory::requestStorageModelReset, this, &StorageModel::resetModel);
    }
}

int StorageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    int countValue = 0;

    if(m_factory != nullptr && !m_factory->isWorking())
        countValue = static_cast<qint32>(m_factory->storagesCount());

    return countValue;
}

QVariant StorageModel::data(const QModelIndex &index, int role) const
{
    QVariant wrapper;

    if(m_factory != nullptr && !m_factory->isWorking()
            && index.row() >= 0 && static_cast<quint32>(index.row()) < m_factory->storagesCount())
    {
        const StorageDevice  &storage = m_factory->storageAt(static_cast<quint32>(index.row()));

        switch(role)
        {
        case PathRole:
            wrapper.setValue(storage.path());
            break;
        case DeviceRole:
            wrapper.setValue(storage.device());
            break;
        case AllowReadWriteRole:
            wrapper.setValue(storage.allowReadWrite());
            break;
        case FileSystemRole:
            wrapper.setValue(storage.fileSystem());
            break;
        case NameRole:
            wrapper.setValue(storage.name());
            break;
        case TypeRole:
            wrapper.setValue(storage.type());
            break;
        case IsPrimaryRole:
            wrapper.setValue(storage.isPrimary());
            break;
        case IsRemovableRole:
            wrapper.setValue(storage.isRemovable());
            break;
        case TotalSizeRole:
            wrapper.setValue(storage.totalSize());
            break;
        case FreeSizeRole:
            wrapper.setValue(storage.freeSize());
            break;
        case TotalSizeStrRole:
            wrapper.setValue(storage.totalSizeStr());
            break;
        case FreeSizeStrRole:
            wrapper.setValue(storage.freeSizeStr());
            break;
        case UsedPercentRole:
            wrapper.setValue(storage.usedPercent());
            break;
        case UsedPercentStrRole:
            wrapper.setValue(storage.usedPercentStr());
            break;
        }
    }

    return wrapper;
}

QVariant StorageModel::itemAt(const quint32 &index)
{
    QVariant wrapper;

    if(m_factory != nullptr && !m_factory->isWorking()
            && index < m_factory->storagesCount())
    {
        /*if(m_currentItem != nullptr)
            m_currentItem->deleteLater();*/
        m_currentItem = new StorageDevice(m_factory->storageAt(index));
        wrapper.setValue(dynamic_cast<QObject*>(m_currentItem));
    }

    return wrapper;
}

QVariant StorageModel::primaryDevice()
{
    QVariant wrapper;

    if(m_factory != nullptr && !m_factory->isWorking())
    {
        /*if(m_currentItem != nullptr)
            m_currentItem->deleteLater();*/
        m_currentItem = new StorageDevice(m_factory->getPrimaryDevice());
        wrapper.setValue(dynamic_cast<QObject*>(m_currentItem));
    }

    return wrapper;
}

void StorageModel::resetModel()
{
    beginResetModel();
    endResetModel();
    emit countChanged(static_cast<quint32>(rowCount()));
}

QHash<int, QByteArray> StorageModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PathRole] = QByteArrayLiteral("path");
    roles[DeviceRole] = QByteArrayLiteral("device");
    roles[AllowReadWriteRole] = QByteArrayLiteral("allowReadWrite");
    roles[FileSystemRole] = QByteArrayLiteral("fileSystem");
    roles[NameRole] = QByteArrayLiteral("name");
    roles[TypeRole] = QByteArrayLiteral("type");
    roles[IsPrimaryRole] = QByteArrayLiteral("isPrimary");
    roles[IsRemovableRole] = QByteArrayLiteral("isRemovable");
    roles[TotalSizeRole] = QByteArrayLiteral("totalSize");
    roles[FreeSizeRole] = QByteArrayLiteral("freeSize");
    roles[TotalSizeStrRole] = QByteArrayLiteral("totalSizeStr");
    roles[FreeSizeStrRole] = QByteArrayLiteral("freeSizeStr");
    roles[UsedPercentRole] = QByteArrayLiteral("usedPercent");
    roles[UsedPercentStrRole] = QByteArrayLiteral("usedPercentStr");
    return roles;
}
