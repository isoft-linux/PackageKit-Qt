# PackageKit-Qt

Qt5 bindings for PackageKit, but it still supports Qt4


## Build && Install

```
cd PackageKit-Qt/
mkdir -p build
cd build
cmake ..
'''but for Qt4 cmake -DUSE_QT5=OFF ..'''
make
sudo make install
```


## pkg-config

```
pkg-config --cflags --libs packagekit-qt5 
```


## migration

port PackageKit-0.7.6/lib/packagekit-qt2 to Qt5.2.x


## Upstream

https://gitorious.org/packagekit/packagekit-qt
