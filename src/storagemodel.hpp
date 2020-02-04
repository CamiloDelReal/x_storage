#ifndef STORAGEMODEL_HPP
#define STORAGEMODEL_HPP

#include <QObject>
#include <QAbstractListModel>
#include <QVariant>

#include "storagefactory.hpp"

class StorageModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(quint32 count READ rowCount NOTIFY countChanged)
public:
    enum StorageModelRoles
    {
        PathRole = Qt::UserRole + 1,
        DeviceRole,
        AllowReadWriteRole,
        FileSystemRole,
        NameRole,
        TypeRole,
        IsPrimaryRole,
        IsRemovableRole,
        TotalSizeRole,
        FreeSizeRole,
        TotalSizeStrRole,
        FreeSizeStrRole,
        UsedPercentRole
    };

    explicit StorageModel(QObject *parent = nullptr);

    void setFactory(StorageFactory *factory);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE QVariant itemAt(const quint32 &index);
    Q_INVOKABLE QVariant primaryDevice();

private slots:
    void resetModel();

signals:
    void countChanged(const quint32 &count);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    StorageFactory *m_factory;
    StorageDevice *m_currentItem;
};

#endif // STORAGEMODEL_HPP
