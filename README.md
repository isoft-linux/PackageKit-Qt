# PackageKit-Qt

Qt5 bindings for PackageKit, as an extension to the original PackageKit-Qt2 (actually using Qt4, don't be fooled 233)


## Build & Install

```
$ cd PackageKit-Qt/
$ mkdir -p build
$ cd build
$ cmake ..

(but for Qt4 version please use cmake -DUSE_QT5=OFF .. here)

$ make
#  make install
```


## To Get pkg-config Information

```
pkg-config --cflags --libs packagekit-qt5 
```


## Migration

A port PackageKit-0.7.6/lib/packagekit-qt2 to Qt version 5.2.x.


## Upstream URL

https://gitorious.org/packagekit/packagekit-qt
