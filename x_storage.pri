#************  Sources  ***************************************************************************
HEADERS += \
    $$PWD/src/mountpoint.hpp \
    $$PWD/src/sizeinfo.hpp \
    $$PWD/src/storagedevice.hpp \
    $$PWD/src/storagefactory.hpp \
    $$PWD/src/storagefactoryworker.hpp \
    $$PWD/src/storagedatainterface.hpp \
    $$PWD/src/storagemanager.hpp \
    $$PWD/src/storagemodel.hpp \
    $$PWD/src/storageutils.hpp

SOURCES += \
    $$PWD/src/mountpoint.cpp \
    $$PWD/src/sizeinfo.cpp \
    $$PWD/src/storagedevice.cpp \
    $$PWD/src/storagefactory.cpp \
    $$PWD/src/storagefactoryworker.cpp \
    $$PWD/src/storagedatainterface.cpp \
    $$PWD/src/storagemanager.cpp \
    $$PWD/src/storagemodel.cpp \
    $$PWD/src/storageutils.cpp

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
        $$PWD/android/src/org/xapps/libraries/xstorage/StorageNotificationNative.java \
        $$PWD/android/src/org/xapps/libraries/xstorage/StorageNotificationReciever.java \
        $$PWD/android/src/org/xapps/libraries/xstorage/StorageNotificationRunner.java
}


#************  Other  *****************************************************************************
OTHER_FILES += \
    $$PWD/readme
