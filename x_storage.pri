#************  Sources  ***************************************************************************
HEADERS += \
    $$PWD/src/mountpoint.hpp \
    $$PWD/src/sizeinfo.hpp \
    $$PWD/src/storagedevice.hpp \
    $$PWD/src/storagefactory.hpp \
    $$PWD/src/storagefactoryworker.hpp \
    $$PWD/src/storagedatainterface.hpp \
    $$PWD/src/storagemanager.hpp \
    $$PWD/src/storagemodel.hpp

SOURCES += \
    $$PWD/src/mountpoint.cpp \
    $$PWD/src/sizeinfo.cpp \
    $$PWD/src/storagedevice.cpp \
    $$PWD/src/storagefactory.cpp \
    $$PWD/src/storagefactoryworker.cpp \
    $$PWD/src/storagedatainterface.cpp \
    $$PWD/src/storagemanager.cpp \
    $$PWD/src/storagemodel.cpp

INCLUDEPATH += \
    $$PWD/src


#************  Android Configurations  ************************************************************
android {
    QT += androidextras

    HEADERS += \
        $$PWD/src/storagenotificationreciever.hpp

    SOURCES += \
        $$PWD/src/storagenotificationreciever.cpp \
        $$PWD/src/storagenotificationrecievernative.cpp

    DISTFILES += \
        $$PWD/android/src/org/xapps/modules/storage/StorageNotificationNative.java \
        $$PWD/android/src/org/xapps/modules/storage/StorageNotificationReciever.java \
        $$PWD/android/src/org/xapps/modules/storage/StorageNotificationRunner.java
}


#************  Other  *****************************************************************************
OTHER_FILES += \
    $$PWD/README
