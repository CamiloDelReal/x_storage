#ifndef MOUNTPOINT_HPP
#define MOUNTPOINT_HPP

#include <QObject>


class MountPoint : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString device READ device WRITE setDevice NOTIFY deviceChanged)
    Q_PROPERTY(bool allowReadWrite READ allowReadWrite WRITE setAllowReadWrite NOTIFY allowReadWriteChanged)
    Q_PROPERTY(QString fileSystem READ fileSystem WRITE setFileSystem NOTIFY fileSystemChanged)
public:
    explicit MountPoint(QObject *parent = nullptr);
    MountPoint(const QString &path, const QString &device,
               const bool &allowReadWrite, const QString &fileSystem,
               QObject *parent = nullptr);
    MountPoint(const MountPoint &other);

    MountPoint &operator=(const MountPoint &other);

    QString path() const;
    QString device() const;
    bool allowReadWrite() const;
    QString fileSystem() const;

public slots:
    void setPath(const QString &path);
    void setDevice(const QString &device);
    void setAllowReadWrite(const bool &allowReadWrite);
    void setFileSystem(const QString &fileSystem);

signals:
    void pathChanged(const QString &path);
    void deviceChanged(const QString &device);
    void allowReadWriteChanged(const bool &allowReadWrite);
    void fileSystemChanged(const QString &fileSystem);

protected:
    QString m_path;
    QString m_device;
    bool m_allowReadWrite;
    QString m_fileSystem;
};

#endif // MOUNTPOINT_HPP
