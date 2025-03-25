QT += testlib core

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
    tests/main.cpp \
    tests/mocks/mockrecentfilesmanager.cpp \
    tests/tst_recentfilesmanager.cpp \
    tests/tst_testrecentfilesinteractor.cpp \
    tests/tst_imagevalidationrules.cpp \

SOURCES += \
    business/recentfilesmanager.cpp \
    business/recentfilesinteractor.cpp \
    business/validation/imagevalidationrules.cpp \
    domain/valueobjects/images.cpp

HEADERS += \
    business/recentfilesmanager.h \
    business/recentfilesinteractor.h \
    tests/mocks/mockrecentfilesmanager.h \
    business/validation/imagevalidationrules.h \
    domain/valueobjects/images.h \
    tests/tst_imagevalidationrules.h \
    tests/tst_recentfilesmanager.h \
    tests/tst_testrecentfilesinteractor.h
