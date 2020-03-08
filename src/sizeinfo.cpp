#include <QObject>

#if defined(Q_OS_WIN)
#include <windows.h>
#include <QDir>
#elif defined(Q_OS_ANDROID) || defined(Q_OS_LINUX)
#include <sys/vfs.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#endif

#include "sizeinfo.hpp"


SizeInfo::SizeInfo(QObject *parent)
    : QObject(parent)
{
    m_total = 0;
    m_free = 0;
    m_used = 0;
}

SizeInfo::SizeInfo(const quint64 &total, const quint64 &free, const quint64 &used, QObject *parent)
    : QObject(parent)
{
    m_total = total;
    m_free = free;
    m_used = used;
}

SizeInfo::SizeInfo(const SizeInfo &other)
    : QObject(other.parent())
{
    m_total = other.m_total;
    m_free = other.m_free;
    m_used = other.m_used;
}

SizeInfo &SizeInfo::operator=(const SizeInfo &other)
{
    m_total = other.m_total;
    m_free = other.m_free;
    m_used = other.m_used;

    setParent(other.parent());

    return *this;
}

SizeInfo SizeInfo::calculate(const QString &devPath, QObject *parent)
{
    quint64 totalSize = 0;
    quint64 freeSize = 0;

#if defined(Q_OS_WIN)

    QString sCurDir = QDir::currentPath();
    QDir::setCurrent(devPath);

    ULARGE_INTEGER free,total;
    bool bRes = ::GetDiskFreeSpaceExA( 0 , &free , &total , nullptr );
    if (bRes)
    {

        freeSize = static_cast<quint64>(static_cast<__int64>(free.QuadPart));
        totalSize = static_cast<quint64>(static_cast<__int64>(total.QuadPart));
    }
    QDir::setCurrent(sCurDir);

#elif defined(Q_OS_ANDROID) || defined(Q_OS_LINUX)

    struct stat stst;
    struct statfs stfs;

    if(::stat(devPath.toLocal8Bit(),&stst) != -1 && ::statfs(devPath.toLocal8Bit(),&stfs) != -1)
    {
        freeSize = stfs.f_bavail * (stst.st_blksize);
        totalSize = stfs.f_blocks * (stst.st_blksize);
    }

#elif defined(Q_OS_IOS)
    // not yet
#endif

    return SizeInfo(totalSize, freeSize, totalSize - freeSize, parent);
}

quint64 SizeInfo::total() const
{
    return m_total;
}

quint64 SizeInfo::free() const
{
    return m_free;
}

quint64 SizeInfo::used() const
{
    return m_used;
}

void SizeInfo::setTotal(const quint64 &total)
{
    if (m_total != total)
    {
        m_total = total;
        emit totalChanged(m_total);
    }
}

void SizeInfo::setFree(const quint64 &free)
{
    if (m_free != free)
    {
        m_free = free;
        emit freeChanged(m_free);
    }
}

void SizeInfo::setUsed(const quint64 &used)
{
    if (m_used != used)
    {
        m_used = used;
        emit usedChanged(m_used);
    }
}
