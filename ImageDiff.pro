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
    business/plugins/imageprocessordeserializer.cpp \
    business/plugins/pluginsmanager.cpp \
    business/imageanalysis/runallcomparatorsinteractor.cpp \
    business/imageanalysis/setprocessorpropertiesinteractor.cpp \
    business/plugins/pythonscriptcomparator.cpp \
    business/plugins/pythonscriptfilter.cpp \
    business/pluginsettingsinteractor.cpp \
    business/recentfilesmanager.cpp \
    data/storage/getfileuserpathsservcie.cpp \
    data/storage/repositories/pluginsrepository.cpp \
    domain/valueobjects/comparableimage.cpp \
    domain/valueobjects/comparisonresultvariant.cpp \
    domain/valueobjects/property.cpp \
    main.cpp \
    presentation/colorpickercontroller.cpp \
    presentation/dialogs/aboutdialog.cpp \
    presentation/dialogs/colorpickerpanel.cpp \
    presentation/dialogs/comparatorresultdialog.cpp \
    presentation/dialogs/getimagesfromvideosdialog.cpp \
    presentation/dialogs/helpdialog.cpp \
    presentation/dialogs/pluginssettingsdialog.cpp \
    presentation/dialogs/propertyeditordialog.cpp \
    presentation/imageprocessorsmenucontroller.cpp \
    presentation/mainwindow.cpp \
    presentation/presenters/htmlimageprocessorshelppresenter.cpp \
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
    tests/setpropertiesinteractortests.cpp \
    domain/valueobjects/autocomparisonreportentry.cpp \

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
    business/plugins/imageprocessordeserializer.h \
    business/plugins/pluginsmanager.h \
    business/imageanalysis/runallcomparatorsinteractor.h \
    business/imageanalysis/setprocessorpropertiesinteractor.h \
    business/plugins/pythonscriptcomparator.h \
    business/plugins/pythonscriptfilter.h \
    business/pluginsettingsinteractor.h \
    business/recentfilesmanager.h \
    data/storage/getfileuserpathsservcie.h \
    data/storage/repositories/pluginsrepository.h \
    domain/interfaces/comparator.h \
    domain/interfaces/filter.h \
    domain/interfaces/imageprocessor.h \
    domain/interfaces/iprogressdialog.h \
    domain/interfaces/mainwindowcallbacks.h \
    domain/valueobjects/autocomparisonreportentry.h \
    domain/valueobjects/comparableimage.h \
    domain/valueobjects/comparisonresultvariant.h \
    domain/valueobjects/imagepixelcolor.h \
    domain/valueobjects/imageprocessorsinfo.h \
    domain/valueobjects/pluginssettings.h \
    domain/valueobjects/property.h \
    domain/valueobjects/pyscriptinfo.h \
    domain/valueobjects/savefileinfo.h \
    presentation/colorpickercontroller.h \
    presentation/dialogs/aboutdialog.h \
    presentation/dialogs/colorpickerpanel.h \
    presentation/dialogs/comparatorresultdialog.h \
    presentation/dialogs/getimagesfromvideosdialog.h \
    presentation/dialogs/helpdialog.h \
    presentation/dialogs/pluginssettingsdialog.h \
    presentation/dialogs/propertyeditordialog.h \
    presentation/imageprocessorsmenucontroller.h \
    presentation/mainwindow.h \
    presentation/presenters/htmlimageprocessorshelppresenter.h \
    presentation/presenters/htmlreportpresenter.h \
    presentation/presenters/recentfilespresenter.h \
    presentation/valueobjects/rgbwidgets.h \
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

DISTFILES += \
    business/plugins/comparator_example.json \
    business/plugins/filter_example.json
