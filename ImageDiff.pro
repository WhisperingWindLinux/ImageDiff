QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    colorssaturationcomporator.cpp \
    comparisionmanager.cpp \
    comparisionresultvariant.cpp \
    contrastcomporator.cpp \
    imagecomparator.cpp \
    imageviewer.cpp \
    interactors/comparisioninteractor.cpp \
    main.cpp \
    mainwindow.cpp \
    pixelsabsolutevaluecomparator.cpp \
    pixelsbrightnesscomparator.cpp

HEADERS += \
    colorssaturationcomporator.h \
    comparisionmanager.h \
    comparisionresultvariant.h \
    contrastcomporator.h \
    imagecomparator.h \
    imageviewer.h \
    interactors/comparisioninteractor.h \
    interfaces/acomporator.h \
    interfaces/mainwindowcallbacks.h \
    mainwindow.h \
    pixelsabsolutevaluecomparator.h \
    pixelsbrightnesscomparator.h \
    savefileinfo.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
