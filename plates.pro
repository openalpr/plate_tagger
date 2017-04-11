TEMPLATE = app
TARGET = plates
TARGET_HUMAN = OpenALPR Training Utility

QT += core gui widgets 
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x050000

VERSION_MAJOR=1
VERSION_MINOR=0
VERSION_PATCH=0

VERSION=$$sprintf("%1.%2.%3", $$VERSION_MAJOR, $$VERSION_MINOR, $$VERSION_PATCH)

DEFINES += VERSION_MAJOR=$$VERSION_MAJOR
DEFINES += VERSION_MINOR=$$VERSION_MINOR
DEFINES += VERSION_PATCH=$$VERSION_PATCH
DEFINES += VERSION_STRING=$$sprintf("\"\\\"%1\\\"\"", $$VERSION)

DEFINES += TARGET=$$TARGET
DEFINES += TARGET_STRING=$$sprintf("\"\\\"%1\\\"\"", $$TARGET)
UP=$$sprintf("%1", $$TARGET)
DEFINES += TARGET_UPPER_STRING=$$sprintf("\"\\\"%1\\\"\"", $$upper($$UP))
DEFINES += TARGET_HUMAN_STRING=$$sprintf("\"\\\"%1\\\"\"", $$TARGET_HUMAN)

win32 {
    RC_FILE = $${TARGET}.rc
} else:macx {
    QMAKE_INFO_PLIST = Info.plist
    ICON = images/$${TARGET}.icns
}

########################################

INCLUDEPATH += . qprogressindicator
DEPENDPATH += . qprogressindicator

SOURCES += main.cpp \
        mainwindow.cpp \
    settings.cpp \
    utils.cpp \
    qprogressindicator/QProgressIndicator.cpp \
    directoryloader.cpp \
    about.cpp \
    imagefile.cpp \
    options.cpp \
    platefile.cpp \
    threadedimageloader.cpp \
    imageview.cpp \
    squeezedlabel.cpp \
    polygons.cpp \
    selection.cpp \
    imageviewerbase.cpp \
    imageviewerobserver.cpp \
    plateselector.cpp \
    imageviewerplateselector.cpp \
    dot.cpp

HEADERS  += mainwindow.h \
    settings.h \
    utils.h \
    qprogressindicator/QProgressIndicator.h \
    directoryloader.h \
    about.h \
    imagefile.h \
    options.h \
    platefile.h \
    threadedimageloader.h \
    imageview.h \
    squeezedlabel.h \
    polygons.h \
    selection.h \
    imageviewerbase.h \
    imageviewerobserver.h \
    plateselector.h \
    imageviewerplateselector.h \
    dot.h

FORMS    += mainwindow.ui \
    directoryloader.ui \
    about.ui \
    options.ui \
    imageview.ui \
    plateselector.ui

RESOURCES += \
    $${TARGET}.qrc

DISTFILES += \
    other/spec.txt \
    README.txt
