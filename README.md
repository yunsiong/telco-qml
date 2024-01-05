# telco-qml

Telco QML plugin for Qt >= 5.15.

## Binaries

Grab one from Telco's [releases][], and extract it in your Qt installation's
`qml` directory.

## Building: standalone

First, download a telco-core devkit from Telco's [releases][], and extract it to
e.g. `/opt/telco-core-devkit`. Then:

    $ mkdir build-telco-qml
    $ cd build-telco-qml
    $ qmake TELCO_CORE_DEVKIT=/opt/telco-core-devkit ../telco-qml
    $ make -j16
    $ make install

## Building: together with Telco

E.g. on macOS/x86_64:

    $ make core-macos-thin
    $ cd build/tmp_thin-macos-x86_64
    $ mkdir telco-qml
    $ cd telco-qml
    $ qmake ../../../telco-qml
    $ make -j16
    $ make install


[releases]: https://github.com/telco/telco/releases
