#include "storagedevice.hpp"
#include "storageutils.hpp"


StorageDevice::StorageDevice(QObject *parent)
    : MountPoint(parent)
{
    m_type = Unknown;
    m_isPrimary = false;
    updateSizesStr();

    connect(&m_sizes, &SizeInfo::totalChanged, this, &StorageDevice::totalSizeChanged);
    connect(&m_sizes, &SizeInfo::freeChanged, this, &StorageDevice::freeSizeChanged);
}

StorageDevice::StorageDevice(const QString &path, const QString &device,
                             const bool &allowReadWrite, const QString &fileSystem,
                             const QString &name, const StorageType &type,
                             const bool &isPrimary, const bool &isRemovable,
                             const SizeInfo &sizes, QObject *parent)
    : MountPoint(path, device, allowReadWrite, fileSystem, parent)
{
    m_name = name;
    m_type = type;
    m_isPrimary = isPrimary;
    m_isRemovable = isRemovable;
    m_sizes = sizes;
    updateSizesStr();

    connect(&m_sizes, &SizeInfo::totalChanged, this, &StorageDevice::totalSizeChanged);
    connect(&m_sizes, &SizeInfo::freeChanged, this, &StorageDevice::freeSizeChanged);
}

StorageDevice::StorageDevice(const MountPoint &mountPoint, const QString &name,
                             const StorageType &type, const bool &isPrimary,
                             const bool &isRemovable, const SizeInfo &sizes)
    : MountPoint(mountPoint)
{
    m_name = name;
    m_type = type;
    m_isPrimary = isPrimary;
    m_isRemovable = isRemovable;
    m_sizes = sizes;
    updateSizesStr();

    connect(&m_sizes, &SizeInfo::totalChanged, this, &StorageDevice::totalSizeChanged);
    connect(&m_sizes, &SizeInfo::freeChanged, this, &StorageDevice::freeSizeChanged);
}

StorageDevice::StorageDevice(const StorageDevice &other)
    : MountPoint(other)
{
    m_name = other.m_name;
    m_type = other.m_type;
    m_isPrimary = other.m_isPrimary;
    m_isRemovable = other.m_isRemovable;
    m_sizes = other.m_sizes;

    m_totalSizeStr = other.m_totalSizeStr;
    m_freeSizeStr = other.m_freeSizeStr;
    m_usedPercent = other.m_usedPercent;

    connect(&m_sizes, &SizeInfo::totalChanged, this, &StorageDevice::totalSizeChanged);
    connect(&m_sizes, &SizeInfo::freeChanged, this, &StorageDevice::freeSizeChanged);
}

StorageDevice &StorageDevice::operator=(const StorageDevice &other)
{
    MountPoint::operator=(other);
    m_name = other.m_name;
    m_type = other.m_type;
    m_isPrimary = other.m_isPrimary;
    m_isRemovable = other.m_isRemovable;
    m_sizes = other.m_sizes;

    m_totalSizeStr = other.m_totalSizeStr;
    m_freeSizeStr = other.m_freeSizeStr;
    m_usedPercent = other.m_usedPercent;

    setParent(other.parent());

    return *this;
}

QString StorageDevice::name() const
{
    return m_name;
}

StorageDevice::StorageType StorageDevice::type() const
{
    return m_type;
}

bool StorageDevice::isPrimary() const
{
    return m_isPrimary;
}

bool StorageDevice::isRemovable() const
{
    return m_isRemovable;
}

quint64 StorageDevice::totalSize() const
{
    return m_sizes.total();
}

quint64 StorageDevice::freeSize() const
{
    return m_sizes.free();
}

QString StorageDevice::totalSizeStr() const
{
    return m_totalSizeStr;
}

QString StorageDevice::freeSizeStr() const
{
    return m_freeSizeStr;
}

quint32 StorageDevice::usedPercent() const
{
    return m_usedPercent;
}

void StorageDevice::setName(const QString &name)
{
    if (m_name != name)
    {
        m_name = name;
        emit nameChanged(m_name);
    }
}

void StorageDevice::setType(const StorageType &type)
{
    if(m_type != type)
    {
        m_type = type;
        emit typeChanged(m_type);
    }
}

void StorageDevice::setIsPrimary(const bool &isPrimary)
{
    if (m_isPrimary != isPrimary)
    {
        m_isPrimary = isPrimary;
        emit isPrimaryChanged(m_isPrimary);
    }
}

void StorageDevice::setIsRemovable(const bool &isRemovable)
{
    if (m_isRemovable != isRemovable)
    {
        m_isRemovable = isRemovable;
        emit isRemovableChanged(m_isRemovable);
    }
}

void StorageDevice::setTotalSize(const quint64 &totalSize)
{
    if(m_sizes.total() != totalSize)
    {
        m_sizes.setTotal(totalSize);
        setTotalSizeStr(StorageUtils::size2str(m_sizes.total()));

        if(m_sizes.total() > 0)
        {
            if(m_sizes.free() == 0)
                setUsedPercent(100);
            else
                setUsedPercent(static_cast<quint32>(100 - ((m_sizes.free() * 100) / m_sizes.total())));
        }
        else
        {
            setUsedPercent(0);
        }
    }
}

void StorageDevice::setFreeSize(const quint64 &freeSize)
{
    if(m_sizes.free() != freeSize)
    {
        m_sizes.setFree(freeSize);
        setFreeSizeStr(StorageUtils::size2str(m_sizes.free()));

        if(m_sizes.total() > 0)
        {
            if(m_sizes.free() == 0)
                setUsedPercent(100);
            else
                setUsedPercent(static_cast<quint32>(100 - ((m_sizes.free() * 100) / m_sizes.total())));
        }
        else
        {
            setUsedPercent(0);
        }
    }
}

void StorageDevice::setTotalSizeStr(const QString &totalSizeStr)
{
    if(m_totalSizeStr != totalSizeStr)
    {
        m_totalSizeStr = totalSizeStr;
        emit totalSizeStrChanged(m_totalSizeStr);
    }
}

void StorageDevice::setFreeSizeStr(const QString &freeSizeStr)
{
    if(m_freeSizeStr != freeSizeStr)
    {
        m_freeSizeStr = freeSizeStr;
        emit freeSizeStrChanged(m_freeSizeStr);
    }
}

void StorageDevice::setUsedPercent(const quint32 &usedPercent)
{
    if (m_usedPercent != usedPercent)
    {
        m_usedPercent = usedPercent;
        emit usedPercentChanged(m_usedPercent);
    }
}

void StorageDevice::updateSizesStr()
{
    setTotalSizeStr(StorageUtils::size2str(m_sizes.total()));
    setFreeSizeStr(StorageUtils::size2str(m_sizes.free()));

    if(m_sizes.total() > 0)
    {
        if(m_sizes.free() == 0)
            setUsedPercent(100);
        else
            setUsedPercent(static_cast<quint32>(100 - ((m_sizes.free() * 100) / m_sizes.total())));
    }
    else
    {
        setUsedPercent(0);
    }
}
