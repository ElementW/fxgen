# -------------------------------------------------
# Project created by QtCreator 2009-11-30T16:43:08
# -------------------------------------------------
DEFINES-=UNICODE

QT -= core \
    gui
TARGET = CoreLib
TEMPLATE = lib
DEFINES += CORELIB_LIBRARY

INCLUDEPATH += ../../../Externals/SFML-1.5/include \
    ../../../Externals/FreeType/Include

LIBS += -L../../../Externals/FreeType/Lib \
    -lfreetype

win32 {
    LIBS += -lOpenGL32 #Win32
  LIBS += -L../../../Externals/SFML-1.5/lib/mingw \
      -lsfml-system-d
  LIBS += -L../../../Externals/SFML-1.5/lib/mingw \
      -lsfml-window-d
}

linux {
    LIBS += -lGL  #Linux
}

SOURCES += ../../Source/Types.cpp \
    ../../Source/StripBarCtrl.cpp \
    ../../Source/Stream.cpp \
    ../../Source/StandardsCtrl.cpp \
    ../../Source/MenuCtrl.cpp \
    ../../Source/MemoryMgr.cpp \
    ../../Source/Maths.cpp \
    ../../Source/Layout.cpp \
    ../../Source/GUI.cpp \
    ../../Source/Font.cpp \
    ../../Source/EventsMgr.cpp \
    ../../Source/Core.cpp \
    ../../Source/Archive.cpp
HEADERS += ../../Source/WinMain.h \
    ../../Source/Types.h \
    ../../Source/Templates.h \
    ../../Source/StripBarCtrl.h \
    ../../Source/Stream.h \
    ../../Source/StandardsCtrl.h \
    ../../Source/MenuCtrl.h \
    ../../Source/MemoryMgr.h \
    ../../Source/Maths.h \
    ../../Source/LinuxMain.h \
    ../../Source/Layout.h \
    ../../Source/Keyboard.h \
    ../../Source/GUI.h \
    ../../Source/Font.h \
    ../../Source/EventsMgr.h \
    ../../Source/Core.h \
    ../../Source/Archive.h \
    ../../Include/CoreLib.h
DESTDIR=../../../Win32Bin_QT/debug
