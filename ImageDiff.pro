QT       += core gui testlib multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    business/getimagesfromvideosinteractor.cpp \
    business/imageanalysis/comparisoninteractor.cpp \
    business/imageanalysis/comporators/colorssaturationcomporator.cpp \
    business/imageanalysis/comporators/contrastcomporator.cpp \
    business/imageanalysis/comporators/differenceinpixelvaluesasimage.cpp \
    business/imageanalysis/comporators/imageproximitytoorigincomparator.cpp \
    business/imageanalysis/comporators/pixelsabsolutevaluecomparator.cpp \
    business/imageanalysis/comporators/pixelsbrightnesscomparator.cpp \
    business/imageanalysis/comporators/sharpnesscomparator.cpp \
    business/imageanalysis/filters/grayscalefilter.cpp \
    business/imageanalysis/filters/rgbfilter.cpp \
    business/imageanalysis/imageprocessorsmanager.cpp \
    business/imageanalysis/runallcomparatorsinteractor.cpp \
    business/imageanalysis/setprocessorpropertiesinteractor.cpp \
    business/recentfilesmanager.cpp \
    domain/valueobjects/comparableimage.cpp \
    domain/valueobjects/comparisonresultvariant.cpp \
    domain/valueobjects/property.cpp \
    main.cpp \
    presentation/dialogs/aboutdialog.cpp \
    presentation/dialogs/colorinfopanel.cpp \
    presentation/dialogs/getimagesfromvideosdialog.cpp \
    presentation/dialogs/propertyeditordialog.cpp \
    presentation/dialogs/rgbtrackinghelper.cpp \
    presentation/mainwindow.cpp \
    presentation/mainwindowbuildmenudelegate.cpp \
    presentation/presenters/htmlreportpresenter.cpp \
    presentation/presenters/recentfilespresenter.cpp \
    presentation/views/imageviewer.cpp \
    presentation/views/videoplayerwidget.cpp \
    tests/comparableimagetests.cpp \
    tests/comparisoninteractortests.cpp \
    tests/comparisonresultvarianttests.cpp \
    tests/imageprocessorsmanagertests.cpp \
    tests/propertytests.cpp \
    tests/recentfilemanagertest.cpp \
    tests/recentfilespathformatertest.cpp \
    tests/saveimageinfotests.cpp \
    tests/setpropertiesinteractortests.cpp

HEADERS += \
    business/getimagesfromvideosinteractor.h \
    business/imageanalysis/comparisoninteractor.h \
    business/imageanalysis/comporators/colorssaturationcomporator.h \
    business/imageanalysis/comporators/contrastcomporator.h \
    business/imageanalysis/comporators/differenceinpixelvaluesasimage.h \
    business/imageanalysis/comporators/imageproximitytoorigincomparator.h \
    business/imageanalysis/comporators/pixelsabsolutevaluecomparator.h \
    business/imageanalysis/comporators/pixelsbrightnesscomparator.h \
    business/imageanalysis/comporators/sharpnesscomparator.h \
    business/imageanalysis/filters/grayscalefilter.h \
    business/imageanalysis/filters/rgbfilter.h \
    business/imageanalysis/imageprocessorsmanager.h \
    business/imageanalysis/runallcomparatorsinteractor.h \
    business/imageanalysis/setprocessorpropertiesinteractor.h \
    business/recentfilesmanager.h \
    domain/interfaces/comporator.h \
    domain/interfaces/filter.h \
    domain/interfaces/imageprocessor.h \
    domain/interfaces/iprogressdialog.h \
    domain/interfaces/mainwindowcallbacks.h \
    domain/valueobjects/comparableimage.h \
    domain/valueobjects/comparisonresultvariant.h \
    domain/valueobjects/imageprocessorsinfo.h \
    domain/valueobjects/imageviewstate.h \
    domain/valueobjects/property.h \
    domain/valueobjects/rgbvalue.h \
    domain/valueobjects/savefileinfo.h \
    presentation/dialogs/aboutdialog.h \
    presentation/dialogs/colorinfopanel.h \
    presentation/dialogs/getimagesfromvideosdialog.h \
    presentation/dialogs/propertyeditordialog.h \
    presentation/dialogs/rgbtrackinghelper.h \
    presentation/mainwindow.h \
    presentation/mainwindowbuildmenudelegate.h \
    presentation/presenters/htmlreportpresenter.h \
    presentation/presenters/recentfilespresenter.h \
    presentation/views/imageviewer.h \
    presentation/views/videoplayerwidget.h \
    tests/comparableimagetests.h \
    tests/comparisoninteractortests.h \
    tests/comparisonresultvarianttests.h \
    tests/imageprocessorsmanagertests.h \
    tests/mocks/mockImageprocessor.h \
    tests/mocks/mockmainwindowcallbacks.h \
    tests/propertytests.h \
    tests/recentfilemanagertest.h \
    tests/recentfilespathformatertest.h \
    tests/saveimageinfotests.h \
    tests/setpropertiesinteractortests.h \
    tests/testutils.h

FORMS += \
    presentation/forms/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

ICON = Resources/app_icon.icns
