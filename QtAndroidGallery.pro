
QT       += core gui androidextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtAndroidGallery
TEMPLATE = app


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    clipscene.cpp \
    rotate.cpp \
    zoom.cpp \
    select.cpp

HEADERS  += mainwindow.h \
    clipscene.h \
    rotate.h \
    zoom.h \
    select.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

DISTFILES += \
    android-sources/AndroidManifest.xml \
    android-sources/src/com/HCI/QtAndroidGallery/QtAndroidGallery.java \
    icons/rotateRight.png \
    icons/rotateLeft.png \
    icons/select.png \
    icons/save.png \
    icons/open.png \
    icons/crop.png \
    icons/zoomOut.png \
    icons/zoomIn.png \
    icons/reset.png

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources

RESOURCES += \
    images.qrc
