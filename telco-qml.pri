QMLPATHS += $$top_builddir/qml

win32 {
    LIBS_PRIVATE += "$${TELCO_CORE_DEVKIT}/telco-core.lib"
}
unix {
    LIBS_PRIVATE += "-L$${TELCO_CORE_DEVKIT}" -ltelco-core
}

win32 {
    QMAKE_LFLAGS += /INCLUDE:?qml_register_types_Telco@@YAXXZ
}
macx {
    QMAKE_LFLAGS += -Wl,-u,__Z24qml_register_types_Telcov
    LIBS_PRIVATE += -Wl,-framework,AppKit -lbsm -lresolv
}
linux {
    QMAKE_LFLAGS += -Wl,--require-defined=_Z24qml_register_types_Telcov
    LIBS_PRIVATE += -ldl -lresolv -lrt -pthread
}
