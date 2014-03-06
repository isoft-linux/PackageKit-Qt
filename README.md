# PackageKit-Qt

PackageKit Qt Binding

## build && install

```
cd PackageKit-Qt/
mkdir -p build
cd build
cmake ..
make
sudo make install
```

## pkg-config

```
pkg-config --cflags --libs packagekit-qt5 
```

## migration

migrated from PackageKit-0.7.6/lib/packagekit-qt2 to Qt5.2.x
