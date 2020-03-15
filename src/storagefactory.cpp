#include <QFileInfo>
#include <QDir>
#include <QStorageInfo>
#include <QTextStream>

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#endif

#include "storagefactory.hpp"
#include "sizeinfo.hpp"

#ifdef Q_OS_ANDROID
#include "storagenotificationreciever.hpp"
#endif


StorageFactory::StorageFactory(QObject *parent)
    : StorageFactoryWorker(parent)
{
    m_isWorking = true;
}

StorageDevice StorageFactory::getPrimaryDevice()
{
    QList<StorageDevice>::iterator it = m_storages.begin();
    StorageDevice dev;

    while(it != m_storages.end())
    {
        if((*it).isPrimary())
        {
            dev = (*it);
        }
        it++;
    }

    return dev;
}

QList<StorageDevice> StorageFactory::getAllStoragesData()
{
    return m_storages;
}

StorageDevice StorageFactory::getStorageDeviceByFolder(const QString &folderPath)
{
    QList<StorageDevice>::iterator it = m_storages.begin();
    StorageDevice dev;

    while(it != m_storages.end())
    {
        if(folderPath.startsWith((*it).path()))
        {
            dev = (*it);
            if((*it).type() != StorageDevice::ROOT)
                break;
        }
        it++;
    }

    return dev;
}

bool StorageFactory::isWorking()
{
    return m_isWorking;
}

void StorageFactory::initialize(const bool &doInBackground)
{
    if(doInBackground)
    {
        m_currentTask = INITIALIZE_STORAGE_DEVICES;
        start();
    }
    else
    {
        initialize_bg();
    }
}

void StorageFactory::update(const bool &doInBackground)
{
    if(doInBackground)
    {
        m_currentTask = UPDATE_STORAGE_DEVICES;
        start();
    }
    else
    {
        update_bg();
    }
}

quint32 StorageFactory::storagesCount()
{
    return static_cast<quint32>(m_storages.size());
}

StorageDevice StorageFactory::storageAt(const quint32 &index)
{
    StorageDevice item = m_storages.at(static_cast<qint32>(index));
    return item;
}

void StorageFactory::execute()
{
    switch(m_currentTask)
    {
    case INITIALIZE_STORAGE_DEVICES:
        initialize_bg();
        break;
    case UPDATE_STORAGE_DEVICES:
        update_bg();
        break;
    }
}

void StorageFactory::setIsWorking(const bool &isWorking)
{
    if(m_isWorking != isWorking)
    {
        m_isWorking = isWorking;
        emit isWorkingChanged(isWorking);
    }
}

#if defined (Q_OS_ANDROID)
void StorageFactory::startNotificationListener()
{
    QtAndroid::androidActivity().callMethod<void>("startStorageNotificationListener", "()V");
}
#endif

void StorageFactory::readStoragesData()
{
    m_storages.clear();

#if defined (Q_OS_ANDROID)

    //Get all mount points
    QHash<QString, MountPoint> mountedPoints = getMountedPoints();

    //Get Root Storage Memory
    QString rootPath = "/";
    if(mountedPoints.contains(rootPath))
    {
        MountPoint rootPoint = mountedPoints.value(rootPath);
        SizeInfo rootSizes = SizeInfo::calculate(rootPath);

        m_storages << StorageDevice(rootPoint, "Root", StorageDevice::ROOT, false, false, rootSizes);
    }

    QAndroidJniObject appContext = QtAndroid::androidContext();
    int apiVersion = QtAndroid::androidSdkVersion();

    //Get storage info from Android Services
    QAndroidJniObject mountServiceObj;

    if(apiVersion < 24) // API 24 = Android 7.0
    {
        QAndroidJniObject serviceObj = QAndroidJniObject::callStaticObjectMethod("android/os/ServiceManager",
                                                                                 "getService",
                                                                                 "(Ljava/lang/String;)Landroid/os/IBinder;",
                                                                                 QAndroidJniObject::fromString("mount").object<jstring>());
        mountServiceObj = QAndroidJniObject::callStaticObjectMethod("android/os/storage/IMountService$Stub",
                                                                    "asInterface",
                                                                    "(Landroid/os/IBinder;)Landroid/os/storage/IMountService;",
                                                                    serviceObj.object<jobject>());
    }
    else
    {
        QAndroidJniObject storageServiceName = QAndroidJniObject::getStaticObjectField("android/content/Context", "STORAGE_SERVICE", "Ljava/lang/String;");
        mountServiceObj = appContext.callObjectMethod("getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;", storageServiceName.object());

    }

    QAndroidJniObject mountedDevicesObj;

    if(apiVersion < 18)         // API 18 = Android 4.3
    {
        mountedDevicesObj = mountServiceObj.callObjectMethod("getVolumeList", "()[Landroid/os/Parcelable;");
    }
    else if(apiVersion < 23)    // API 23 = Android 6.0
    {
        mountedDevicesObj = mountServiceObj.callObjectMethod("getVolumeList", "()[Landroid/os/storage/StorageVolume;");
    }
    else if(apiVersion < 24)    // API 24 = Android 7.0
    {
        QAndroidJniObject packageNameObj = appContext.callObjectMethod("getPackageName", "()Ljava/lang/String;");
        jint kernelUID = appContext
                .callObjectMethod("getPackageManager", "()Landroid/content/pm/PackageManager;")
                .callObjectMethod("getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;",
                                  packageNameObj.object<jstring>(), 0)
                .getObjectField("applicationInfo", "Landroid/content/pm/ApplicationInfo;")
                .getField<jint>("uid");
        mountedDevicesObj = mountServiceObj.callObjectMethod("getVolumeList",
                                                             "(ILjava/lang/String;I)[Landroid/os/storage/StorageVolume;",
                                                             kernelUID,
                                                             packageNameObj.object<jstring>(),
                                                             0);
    }
    else
    {
        QAndroidJniObject storageVolumesListObj = mountServiceObj.callObjectMethod("getStorageVolumes", "()Ljava/util/List;");
        mountedDevicesObj = storageVolumesListObj.callObjectMethod("toArray", "()[Ljava/lang/Object;");
    }

    jobjectArray devList = mountedDevicesObj.object<jobjectArray>();
    QAndroidJniEnvironment qjniEnv;
    const int devCount = qjniEnv->GetArrayLength(devList);

    for(int i = 0; i < devCount; i++)
    {
        QAndroidJniObject devItem(qjniEnv->GetObjectArrayElement(devList, i));
        if(devItem != nullptr)
        {
            QAndroidJniObject pathObj = devItem.callObjectMethod("getPath", "()Ljava/lang/String;");
            QString path = pathObj.toString();
            MountPoint devMountPoint;

            if(mountedPoints.contains(path))
            {
                devMountPoint = mountedPoints.value(path);
            }
            else
            {
                // Check if it is emulated
                QRegExp emulatedPattern(".*emulated\\/\\d+$");
                if(emulatedPattern.exactMatch(path))
                {
                    QRegExp emulatedPreffixRemover("\\/\\d+$");
                    QString mountedPath = path;
                    path.remove(emulatedPreffixRemover);
                    if(mountedPoints.contains(path))
                    {
                        devMountPoint = mountedPoints.value(path);
                        devMountPoint.setPath(mountedPath);
                    }
                    else
                    {
                        devMountPoint = MountPoint(path);
                    }
                }
                else
                {
                    devMountPoint = MountPoint(path);
                }

            }

            QAndroidJniObject nameObj = devItem.callObjectMethod("getDescription", "(Landroid/content/Context;)Ljava/lang/String;", appContext.object<jobject>());
            QString name = (nameObj != nullptr && nameObj.isValid() ? nameObj.toString() : "");
            jboolean isRemovable = devItem.callMethod<jboolean>("isRemovable", "()Z");
            SizeInfo devSizes = SizeInfo::calculate(path);

            jboolean isPrimary;

            if(apiVersion < 24)
            {
                //Get Primary Storage Path
                QAndroidJniObject primaryStorageFileObj = QAndroidJniObject::callStaticObjectMethod("android/os/Environment", "getExternalStorageDirectory", "()Ljava/io/File;");
                QAndroidJniObject primaryStorageStringObj = primaryStorageFileObj.callObjectMethod( "getAbsolutePath", "()Ljava/lang/String;" );
                QString primaryStoragePath = primaryStorageStringObj.toString();
                isPrimary = (primaryStoragePath == path);
            }
            else
            {
                isPrimary = devItem.callMethod<jboolean>("isPrimary", "()Z");
            }

            StorageDevice::StorageType type = StorageDevice::UNKNOWN;

            if(!isRemovable)
            {
                type = StorageDevice::INTERNAL;
            }
            else
            {
                QRegExp usbPattern(".*usb.*|.*media_rw.*");
                if(usbPattern.exactMatch(path))
                {
                    type = StorageDevice::USB;
                }
                else
                {
                    type = StorageDevice::SDCARD;
                }
            }

            m_storages << StorageDevice(devMountPoint, name, type, isPrimary, isRemovable, devSizes);
        }
    }

#elif defined (Q_OS_WIN)

    QFileInfoList listDrives = QDir::drives();
    foreach(QFileInfo info, listDrives)
    {
        QStorageInfo devInfo(info.absoluteFilePath());
        if(devInfo.isReady())
        {
            SizeInfo devSizes = SizeInfo::calculate(devInfo.rootPath());
            StorageDevice device(devInfo.rootPath(), QString::fromLocal8Bit(devInfo.device()),
                                 !devInfo.isReadOnly(), QString::fromLocal8Bit(devInfo.fileSystemType()),
                                 !devInfo.name().isEmpty() ? devInfo.name() : devInfo.displayName(),
                                 devInfo.isRoot() ? StorageDevice::ROOT : StorageDevice::HDD, devInfo.isRoot(),
                                 false, devSizes);

            m_storages.append(device);
        }
    }

#elif defined (Q_OS_LINUX)

    QStorageInfo rootDev("/");
    SizeInfo devSizes = SizeInfo::calculate(rootDev.rootPath());
    StorageDevice device(rootDev.rootPath(), QString::fromLocal8Bit(rootDev.device()),
                         !rootDev.isReadOnly(), QString::fromLocal8Bit(rootDev.fileSystemType()),
                         "Root",
                         StorageDevice::Root, rootDev.isRoot(),
                         false, devSizes);

    m_storages.append(device);

#endif
}

#ifdef Q_OS_ANDROID
QHash<QString, MountPoint> StorageFactory::getMountedPoints()
{
    QFile mountsFile("/proc/mounts");
    bool fileOpened = false;
    if(!mountsFile.exists())
    {
        mountsFile.setFileName("/proc/self/mounts");
        if(mountsFile.exists())
            fileOpened = mountsFile.open(QFile::ReadOnly | QFile::Text);
    }
    else
    {
        fileOpened = mountsFile.open(QFile::ReadOnly | QFile::Text);
    }

    QHash<QString, MountPoint> points;
    if(fileOpened)
    {
        QString line;
        QTextStream mountsFileStream(&mountsFile);
        int jump;
        QStringList lineSections;
        QString path;
        QString device;
        QString allowReadWriteStr;
        QString fileSystem;
        while(mountsFileStream.readLineInto(&line))
        {
            lineSections = line.split(" ");
            if(lineSections.size() >= 5)
            {
                device = lineSections.at(0);
                jump = 1;
                if(lineSections.at(jump) == "on")
                    jump++;
                path = lineSections.at(jump);
                jump++;
                if(lineSections.at(jump) == "type")
                    jump++;
                fileSystem = lineSections.at(jump);
                allowReadWriteStr = lineSections.at(jump + 1);
                if(allowReadWriteStr.at(0) == '(')
                    allowReadWriteStr = allowReadWriteStr.right(1);
                points.insert(path, MountPoint(path, device, allowReadWriteStr.startsWith("rw"), fileSystem));
            }
        }

        mountsFile.close();
    }

    return points;
}
#endif

void StorageFactory::initialize_bg()
{
    setIsWorking(true);

#if (defined (Q_OS_ANDROID) && !defined(NO_STORAGE_NOTIFICATION_LISTENER))
    startNotificationListener();
#endif

    readStoragesData();

    setIsWorking(false);
    emit requestStorageModelReset();
}

void StorageFactory::update_bg()
{
    setIsWorking(true);

    readStoragesData();

    setIsWorking(false);
    emit requestStorageModelReset();
}
