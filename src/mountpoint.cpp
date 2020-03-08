#include "mountpoint.hpp"


MountPoint::MountPoint(QObject *parent)
    : QObject(parent)
{

}

MountPoint::MountPoint(const QString &path, QObject *parent)
    : QObject(parent)
{
    m_path = path;
}

MountPoint::MountPoint(const QString &path, const QString &device,
                       const bool &allowReadWrite, const QString &fileSystem,
                       QObject *parent)
    : QObject(parent)
{
    m_path = path;
    m_device = device;
    m_allowReadWrite = allowReadWrite;
    m_fileSystem = fileSystem;
}

MountPoint::MountPoint(const MountPoint &other)
    : QObject(other.parent())
{
    m_path = other.m_path;
    m_device = other.m_device;
    m_allowReadWrite = other.m_allowReadWrite;
    m_fileSystem = other.m_fileSystem;
}

MountPoint &MountPoint::operator=(const MountPoint &other)
{
    m_path = other.m_path;
    m_device = other.m_device;
    m_allowReadWrite = other.m_allowReadWrite;
    m_fileSystem = other.m_fileSystem;

    setParent(other.parent());

    return *this;
}

QString MountPoint::path() const
{
    return m_path;
}

QString MountPoint::device() const
{
    return m_device;
}

bool MountPoint::allowReadWrite() const
{
    return m_allowReadWrite;
}

QString MountPoint::fileSystem() const
{
    return m_fileSystem;
}

void MountPoint::setPath(const QString &path)
{
    if (m_path != path)
    {
        m_path = path;
        emit pathChanged(m_path);
    }
}

void MountPoint::setDevice(const QString &device)
{
    if (m_device != device)
    {
        m_device = device;
        emit deviceChanged(m_device);
    }
}

void MountPoint::setAllowReadWrite(const bool &allowReadWrite)
{
    if (m_allowReadWrite != allowReadWrite)
    {
        m_allowReadWrite = allowReadWrite;
        emit allowReadWriteChanged(m_allowReadWrite);
    }
}

void MountPoint::setFileSystem(const QString &fileSystem)
{
    if (m_fileSystem != fileSystem)
    {
        m_fileSystem = fileSystem;
        emit fileSystemChanged(m_fileSystem);
    }
}
