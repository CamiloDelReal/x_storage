#ifndef STORAGEDEVICE_HPP
#define STORAGEDEVICE_HPP

#include <QObject>

#include "mountpoint.hpp"
#include "sizeinfo.hpp"


class StorageDevice : public MountPoint
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(StorageType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(bool isPrimary READ isPrimary WRITE setIsPrimary NOTIFY isPrimaryChanged)
    Q_PROPERTY(bool isRemovable READ isRemovable WRITE setIsRemovable NOTIFY isRemovableChanged)
    Q_PROPERTY(quint64 totalSize READ totalSize WRITE setTotalSize NOTIFY totalSizeChanged)
    Q_PROPERTY(quint64 freeSize READ freeSize WRITE setFreeSize NOTIFY freeSizeChanged)

    //Extra properties, for easy use and/or avoid an UI overload
    Q_PROPERTY(QString totalSizeStr READ totalSizeStr WRITE setTotalSizeStr NOTIFY totalSizeStrChanged)
    Q_PROPERTY(QString freeSizeStr READ freeSizeStr WRITE setFreeSizeStr NOTIFY freeSizeStrChanged)
    Q_PROPERTY(quint32 usedPercent READ usedPercent WRITE setUsedPercent NOTIFY usedPercentChanged)
    Q_PROPERTY(QString usedPercentStr READ usedPercentStr WRITE setUsedPercentStr NOTIFY usedPercentStrChanged)

public:
    enum StorageType
    {
        Unknown = 0,
        Root,
        Internal,
        SdCard,
        Usb,
        Hdd
    };
    Q_ENUM(StorageType)

    explicit StorageDevice(QObject *parent = nullptr);
    StorageDevice(const QString &path, const QString &device,
                  const bool &allowReadWrite, const QString &fileSystem,
                  const QString &name, const StorageType &type,
                  const bool &isPrimary, const bool &isRemovable,
                  const SizeInfo &sizes, QObject *parent = nullptr);
    StorageDevice(const MountPoint &mountPoint, const QString &name,
                  const StorageType &type, const bool &isPrimary,
                  const bool &isRemovable, const SizeInfo &sizes);
    StorageDevice(const StorageDevice &other);

    StorageDevice &operator=(const StorageDevice &other);

    QString name() const;
    StorageType type() const;
    bool isPrimary() const;
    bool isRemovable() const;
    quint64 totalSize() const;
    quint64 freeSize() const;

    QString totalSizeStr() const;
    QString freeSizeStr() const;
    quint32 usedPercent() const;    
    QString usedPercentStr() const;

public slots:
    void setName(const QString &name);
    void setType(const StorageType &type);
    void setIsPrimary(const bool &isPrimary);
    void setIsRemovable(const bool &isRemovable);
    void setTotalSize(const quint64 &totalSize);
    void setFreeSize(const quint64 &freeSize);
    void setUsedPercentStr(const QString &usedPercentStr);

private slots:
    void setTotalSizeStr(const QString &totalSizeStr);
    void setFreeSizeStr(const QString &freeSizeStr);
    void setUsedPercent(const quint32 &usedPercent);

signals:
    void nameChanged(const QString &name);
    void typeChanged(const StorageType &type);
    void isPrimaryChanged(const bool &isPrimary);
    void isRemovableChanged(const bool &isRemovable);
    void totalSizeChanged(const quint64 &totalSize);
    void freeSizeChanged(const quint64 &freeSize);

    void totalSizeStrChanged(const QString &totalSizeStr);
    void freeSizeStrChanged(const QString &freeSizeStr);
    void usedPercentChanged(const quint32 &usedPercent);
    void usedPercentStrChanged(const QString &usedPercentStr);

private:
    QString m_name;
    StorageType m_type;
    bool m_isPrimary;
    bool m_isRemovable;
    SizeInfo m_sizes;

    QString m_totalSizeStr;
    QString m_freeSizeStr;
    quint32 m_usedPercent;
    QString m_usedPercentStr;

    void updateSizesStr();
};

#endif // STORAGEDEVICE_HPP
