# XStorage
Library to list storage devices<br/>
A demo app is [here](https://github.com/CamiloDelReal/xstorage_demo)

### Features
- Windows, Android
  * iOS missing
  * Linux needs improvements, only listing root and not mount points
- Storage device information
  * Name
  * Type
  * Primary
  * Removable
  * Sizes
  * Mount Point
    - Path
	- Device
	- Read/Write accessibility
	- File system
	
### Migration to Qt6 of the Android core is in TODO

### How to use for Android with device changes listener

#### Declaring intent listener

To use device mount/unmount listener you must inherit from QActivity and add the folliwings lines to your new Android Activity

	...
	import org.xapps.libraries.storage.StorageNotificationRunner;
	...
	public class ... extends QtActivity
	{
		...

		public void startStorageNotificationListener() {
			runOnUiThread(new StorageNotificationRunner(this));
		}

		...
	}

And of course, modify your AndroidManifest to use this activity

#### Compile the new Activity

To compile properly the java files in module, you need to add de java folder to variable java.srcDirs
in the build.gradle file:

	By default
		java.srcDirs = [qt5AndroidDir + '/src', 'src', 'java']
	New
		java.srcDirs = [qt5AndroidDir + '/src', 'src', 'java', <path_to_java_folder>]
	Example
		java.srcDirs = [qt5AndroidDir + '/src', 'src', 'java', '../libraries/xstorage/android/src']
		
#### Skip listener from Android changes

If you don't use storage mount/unmount listener, then add to your .pro file, before include storages.pri

    DEFINES += NO_STORAGE_NOTIFICATION_LISTENER
		
### Types from QML

To register enums types and use it from qml

    qmlRegisterUncreatableType<StorageDevice>("XApps.XStorage", 1, 0, "StorageType", "Can't register StorageDevice::StorageType");
	