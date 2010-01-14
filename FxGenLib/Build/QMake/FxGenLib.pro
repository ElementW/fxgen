# -------------------------------------------------
# Project created by QtCreator 2009-10-26T13:09:50
# -------------------------------------------------
QT -= core \
    gui
TARGET = FxGenLib
TEMPLATE = lib
DEFINES += FXGENLIB_LIBRARY
SOURCES += ../../Source/RenderOpsFunc.cpp \
    ../../Source/pch.cpp \
    ../../Source/MemoryMgr.cpp \
    ../../Source/Maths.cpp \
    ../../Source/FilterOpsFunc.cpp \
    ../../Source/EngineOp.cpp \
    ../../Source/DistordOpsFunc.cpp \
    ../../Source/CompilerOp.cpp \
    ../../Source/CombineOpsFunc.cpp \
    ../../Source/Bitmap.cpp
HEADERS += ../../Source/RectangularArray.h \
    ../../Source/pch.h \
    ../../Source/MemoryMgr.h \
    ../../Source/Maths.h \
    ../../Source/EngineOp.h \
    ../../Source/CompilerOp.h \
    ../../Source/Bitmap.h \
    ../../Source/Templates.h \
    ../../Include/FxGenLib.h
DESTDIR=../../../Win32Bin_QT/debug
