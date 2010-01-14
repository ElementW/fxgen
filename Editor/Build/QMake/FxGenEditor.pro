DEFINES-=UNICODE

QT -= core \
    gui
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

#DEPENDPATH += ../../../FxGenLib/Build/QMake/debug \

DEPENDPATH +=  ../../../Externals/FreeType/Lib \
    ../../../Win32Bin_QT/debug

win32 {
    DEPENDPATH += ../../../Externals/SFML-1.5/lib/mingw
}

linux {
    DEPENDPATH += ../../../Externals/SFML-1.5/lib
}

INCLUDEPATH += ../../../Core/Include \
    ../../../Externals/SFML-1.5/include \
    ../../../Externals/FreeType/Include

#    ../../../FxGenLib/Include \

DEFINES += _CRT_SECURE_NO_WARNINGS
LIBS += -L../../../Win32Bin_QT/debug \
    -lCoreLib

#LIBS += -L../../../FxGenLib/Build/QMake/debug \
#    -lFxGenLib

LIBS += -L../../../Externals/FreeType/Lib \
    -lfreetype

win32 {
    LIBS += -lOpenGL32
    LIBS += -L../../../Externals/SFML-1.5/lib/mingw \
        -lsfml-window-d
    LIBS += -L../../../Externals/SFML-1.5/lib/mingw \
        -lsfml-system-d
}

linux {
    LIBS += -lGL
    LIBS += -L../../../Externals/SFML-1.5/lib \
        -lsfml-window
    LIBS += -L../../../Externals/SFML-1.5/lib \
        -lsfml-system
}

SOURCES += ../../Source/ViewportsWnd.cpp \
    ../../Source/ViewportsCtrl.cpp \
    ../../Source/TreeNodeCtrl.cpp \
    ../../Source/OperatorsCtrl.cpp \
    ../../Source/PropertyItems.cpp \
    ../../Source/FileChooserDialog.cpp \
    ../../Source/PropertiesWnd.cpp \
    ../../Source/PropertiesCtrl.cpp \
    ../../Source/ProjectWnd.cpp \
    ../../Source/OperatorsWnd.cpp \
    ../../Source/Operator.cpp \
    ../../Source/Main.cpp \
    ../../Source/EditorGUI.cpp \
    ../../Source/EditorApp.cpp
HEADERS += ../../Source/*.h \
    ../../Source/CoreLibPkg.h
DESTDIR=../../../Win32Bin_QT/debug
