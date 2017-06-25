TEMPLATE = app
TARGET = plates
TARGET_HUMAN = OpenALPR Training Utility

QT += core gui widgets 
CONFIG += c++11 link_pkgconfig

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x050000

VERSION_MAJOR=1
VERSION_MINOR=1
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
    dot.h \
    plates-yaml.h

FORMS    += mainwindow.ui \
    directoryloader.ui \
    about.ui \
    options.ui \
    imageview.ui \
    plateselector.ui

# built-in yaml-cpp on all platforms.
# See https://bugs.launchpad.net/ubuntu/+source/yaml-cpp/+bug/1646914
INCLUDEPATH += extra/yaml-cpp/include
DEPENDPATH += extra/yaml-cpp/include

SOURCES += \
    extra/yaml-cpp/src/binary.cpp \
    extra/yaml-cpp/src/convert.cpp \
    extra/yaml-cpp/src/directives.cpp \
    extra/yaml-cpp/src/emit.cpp \
    extra/yaml-cpp/src/emitfromevents.cpp \
    extra/yaml-cpp/src/emitter.cpp \
    extra/yaml-cpp/src/emitterstate.cpp \
    extra/yaml-cpp/src/emitterutils.cpp \
    extra/yaml-cpp/src/exceptions.cpp \
    extra/yaml-cpp/src/exp.cpp \
    extra/yaml-cpp/src/memory.cpp \
    extra/yaml-cpp/src/node.cpp \
    extra/yaml-cpp/src/node_data.cpp \
    extra/yaml-cpp/src/nodebuilder.cpp \
    extra/yaml-cpp/src/nodeevents.cpp \
    extra/yaml-cpp/src/null.cpp \
    extra/yaml-cpp/src/ostream_wrapper.cpp \
    extra/yaml-cpp/src/parse.cpp \
    extra/yaml-cpp/src/parser.cpp \
    extra/yaml-cpp/src/regex_yaml.cpp \
    extra/yaml-cpp/src/scanner.cpp \
    extra/yaml-cpp/src/scanscalar.cpp \
    extra/yaml-cpp/src/scantag.cpp \
    extra/yaml-cpp/src/scantoken.cpp \
    extra/yaml-cpp/src/simplekey.cpp \
    extra/yaml-cpp/src/singledocparser.cpp \
    extra/yaml-cpp/src/stream.cpp \
    extra/yaml-cpp/src/tag.cpp \
    extra/yaml-cpp/src/contrib/graphbuilder.cpp \
    extra/yaml-cpp/src/contrib/graphbuilderadapter.cpp

HEADERS += \
    extra/yaml-cpp/include/yaml-cpp/anchor.h \
    extra/yaml-cpp/include/yaml-cpp/binary.h \
    extra/yaml-cpp/include/yaml-cpp/dll.h \
    extra/yaml-cpp/include/yaml-cpp/emitfromevents.h \
    extra/yaml-cpp/include/yaml-cpp/emitter.h \
    extra/yaml-cpp/include/yaml-cpp/emitterdef.h \
    extra/yaml-cpp/include/yaml-cpp/emittermanip.h \
    extra/yaml-cpp/include/yaml-cpp/emitterstyle.h \
    extra/yaml-cpp/include/yaml-cpp/eventhandler.h \
    extra/yaml-cpp/include/yaml-cpp/exceptions.h \
    extra/yaml-cpp/include/yaml-cpp/mark.h \
    extra/yaml-cpp/include/yaml-cpp/noncopyable.h \
    extra/yaml-cpp/include/yaml-cpp/null.h \
    extra/yaml-cpp/include/yaml-cpp/ostream_wrapper.h \
    extra/yaml-cpp/include/yaml-cpp/parser.h \
    extra/yaml-cpp/include/yaml-cpp/stlemitter.h \
    extra/yaml-cpp/include/yaml-cpp/traits.h \
    extra/yaml-cpp/include/yaml-cpp/yaml.h \
    extra/yaml-cpp/include/yaml-cpp/contrib/anchordict.h \
    extra/yaml-cpp/include/yaml-cpp/contrib/graphbuilder.h \
    extra/yaml-cpp/include/yaml-cpp/node/convert.h \
    extra/yaml-cpp/include/yaml-cpp/node/emit.h \
    extra/yaml-cpp/include/yaml-cpp/node/impl.h \
    extra/yaml-cpp/include/yaml-cpp/node/iterator.h \
    extra/yaml-cpp/include/yaml-cpp/node/node.h \
    extra/yaml-cpp/include/yaml-cpp/node/parse.h \
    extra/yaml-cpp/include/yaml-cpp/node/ptr.h \
    extra/yaml-cpp/include/yaml-cpp/node/type.h \
    extra/yaml-cpp/include/yaml-cpp/node/detail/bool_type.h \
    extra/yaml-cpp/include/yaml-cpp/node/detail/impl.h \
    extra/yaml-cpp/include/yaml-cpp/node/detail/iterator.h \
    extra/yaml-cpp/include/yaml-cpp/node/detail/iterator_fwd.h \
    extra/yaml-cpp/include/yaml-cpp/node/detail/memory.h \
    extra/yaml-cpp/include/yaml-cpp/node/detail/node.h \
    extra/yaml-cpp/include/yaml-cpp/node/detail/node_data.h \
    extra/yaml-cpp/include/yaml-cpp/node/detail/node_iterator.h \
    extra/yaml-cpp/include/yaml-cpp/node/detail/node_ref.h

RESOURCES += \
    $${TARGET}.qrc

DISTFILES += \
    other/spec.txt \
    README.txt
