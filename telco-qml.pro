TEMPLATE = lib
CONFIG += qt no_keywords plugin qmltypes
QT += quick

QML_IMPORT_NAME = Telco
QML_IMPORT_MAJOR_VERSION = 1

!isEmpty(_QMAKE_CONF_) {
    DESTDIR = $$shadowed($$dirname(_QMAKE_CONF_))/qml/$$QML_IMPORT_NAME
} else {
    DESTDIR = $$OUT_PWD/qml/$$QML_IMPORT_NAME
}
TARGET = telco-qml
TARGETPATH = Telco
QMAKE_MOC_OPTIONS += -Muri=$$TARGETPATH

SOURCES = \
    src/plugin.cpp \
    src/device.cpp \
    src/application.cpp \
    src/process.cpp \
    src/maincontext.cpp \
    src/telco.cpp \
    src/spawnoptions.cpp \
    src/script.cpp \
    src/devicelistmodel.cpp \
    src/applicationlistmodel.cpp \
    src/processlistmodel.cpp \
    src/iconprovider.cpp \
    src/variant.cpp

HEADERS = \
    src/plugin.h \
    src/device.h \
    src/application.h \
    src/process.h \
    src/maincontext.h \
    src/telco.h \
    src/spawnoptions.h \
    src/script.h \
    src/devicelistmodel.h \
    src/applicationlistmodel.h \
    src/processlistmodel.h \
    src/iconprovider.h \
    src/variant.h

PLUGINFILES = \
    qmldir

OTHER_FILES += $$PLUGINFILES

INCLUDEPATH += $$PWD/src

!isEmpty(TELCO_CORE_DEVKIT) {
    INCLUDEPATH += "$${TELCO_CORE_DEVKIT}"

    win32 {
        LIBS_PRIVATE += "$${TELCO_CORE_DEVKIT}/telco-core.lib"
        QMAKE_LFLAGS += /IGNORE:4099
    }
    unix {
        LIBS_PRIVATE += "-L$${TELCO_CORE_DEVKIT}" -ltelco-core
    }

    macx {
        LIBS_PRIVATE += -Wl,-framework,AppKit -lbsm -lresolv
    }
    linux {
        LIBS_PRIVATE += -ldl -lresolv -lrt -pthread
    }

    install_path = $$[QT_INSTALL_QML]/$$QML_IMPORT_NAME
} else {
    TELCO = $$absolute_path("$$PWD/../")

    win32 {
        win32-msvc*:contains(QMAKE_TARGET.arch, x86_64): {
            TELCO_HOST = x64-Release
        } else {
            TELCO_HOST = Win32-Release
        }
    }
    macx {
        TELCO_BUILD = macos-x86_64
        TELCO_HOST = macos-x86_64
    }
    linux {
        TELCO_BUILD = linux-x86_64
        TELCO_HOST = linux-x86_64
    }

    win32 {
        TELCO_SDK_LIBS = \
            libpcre2-8.a \
            libffi.a \
            libz.a \
            libbrotlicommon.a libbrotlienc.a libbrotlidec.a \
            libglib-2.0.a libgmodule-2.0.a libgobject-2.0.a libgthread-2.0.a libgio-2.0.a \
            libnice.a \
            libusrsctp.a \
            libgioopenssl.a \
            libssl.a libcrypto.a \
            libgee-0.8.a \
            libjson-glib-1.0.a \
            libnghttp2.a \
            libsqlite3.a \
            libpsl.a \
            libsoup-3.0.a \
            libcapstone.a \
            libquickjs.a

        INCLUDEPATH += "$${TELCO}/build/sdk-windows/$${TELCO_HOST}/include/glib-2.0"
        INCLUDEPATH += "$${TELCO}/build/sdk-windows/$${TELCO_HOST}/lib/glib-2.0/include"
        INCLUDEPATH += "$${TELCO}/build/sdk-windows/$${TELCO_HOST}/include/gee-0.8"
        INCLUDEPATH += "$${TELCO}/build/sdk-windows/$${TELCO_HOST}/include/json-glib-1.0"
        INCLUDEPATH += "$${TELCO}/build/tmp-windows/$${TELCO_HOST}/telco-core/api"

        LIBS_PRIVATE += \
            advapi32.lib \
            crypt32.lib \
            dnsapi.lib \
            gdi32.lib \
            iphlpapi.lib \
            ole32.lib \
            psapi.lib \
            secur32.lib \
            shell32.lib \
            shlwapi.lib \
            user32.lib \
            winmm.lib \
            ws2_32.lib
        LIBS_PRIVATE += -L"$${TELCO}/build/sdk-windows/$${TELCO_HOST}/lib" $${TELCO_SDK_LIBS}
        LIBS_PRIVATE += -L"$${TELCO}/build/sdk-windows/$${TELCO_HOST}/lib/gio/modules"
        LIBS_PRIVATE += -L"$${TELCO}/build/tmp-windows/$${TELCO_HOST}/telco-core" telco-core.lib
    }

    unix {
        QT_CONFIG -= no-pkg-config
        CONFIG += link_pkgconfig
        PKG_CONFIG = PKG_CONFIG_PATH=$${TELCO}/build/sdk-$${TELCO_HOST}/lib/pkgconfig:$${TELCO}/build/telco-$${TELCO_HOST}/lib/pkgconfig $${TELCO}/build/toolchain-$${TELCO_BUILD}/bin/pkg-config --define-variable=telco_sdk_prefix=$${TELCO}/build/sdk-$${TELCO_HOST} --static
        PKGCONFIG += telco-core-1.0
    }

    win32:install_path = $${TELCO}/build/telco-windows/$${TELCO_HOST}/lib/qt5/qml/$$QML_IMPORT_NAME
    unix:install_path = $${TELCO}/build/telco-$${TELCO_HOST}/lib/qt5/qml/$$QML_IMPORT_NAME
}

!static {
    win32 {
        QMAKE_LFLAGS += /NODEFAULTLIB:libcmt.lib
    }

    macx {
        QMAKE_LFLAGS += -Wl,-exported_symbol,_qt_plugin_query_metadata -Wl,-exported_symbol,_qt_plugin_instance -Wl,-dead_strip
    }

    linux {
        QMAKE_LFLAGS += -Wl,--version-script -Wl,$${PWD}/telco-qml.version -Wl,--gc-sections -Wl,-z,noexecstack
    }
}

target.path = $$install_path

pluginfiles_install.files = $$PLUGINFILES $$OUT_PWD/plugins.qmltypes
pluginfiles_install.path = $$install_path
pluginfiles_install.CONFIG += no_check_exist

pluginfiles_copy.files = $$PLUGINFILES $$OUT_PWD/plugins.qmltypes
pluginfiles_copy.path = $$DESTDIR

QMLTYPES_INSTALL_DIR = $$install_path

INSTALLS += target pluginfiles_install
COPIES += pluginfiles_copy

static {
    telco_qml_resources.files = qmldir
    telco_qml_resources.prefix = /qt-project.org/imports/Telco
    RESOURCES += telco_qml_resources
}
