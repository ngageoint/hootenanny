# -------------------------------------------------
# Main project for hoot production code
# -------------------------------------------------

QT += \
    sql \
    testlib \
    xml \
    network \

TARGET = HootCore
TEMPLATE = lib

DEPENDPATH += \
    ../tbs/src/main/cpp/ \
    ../tgs/src/main/cpp/ \
    src/main/cpp/ \

INCLUDEPATH += \
  $${DEPENDPATH} \
  ../local/include/ \

CONFIG += rtti \
    debug \
    precompile_header

OTHER_FILES = \
    $$files(../plugins/*.js, true) \
    $$files(../plugins/*.py, true) \
    $$files(../translations/*.js, true) \
    $$files(../translations/*.py, true) \
    $$files(../plugins-local/*.js, true) \
    $$files(../plugins-local/*.py, true) \
    $$files(../translations-local/*.js, true) \
    $$files(../translations-local/*.py, true) \
    $$files(../rules/*.js, true) \
    $$files(../conf/schema/*.json, true) \
    $$files(../docs/commands/*.asciidoc, true) \
    ../conf/core/ConfigOptions.asciidoc \

include(../Configure.pri)

LIBS += -L../lib/ -lTgs -ltbs
LIBS -= -lhdfs

UI_DIR = tmp/ui
MOC_DIR = tmp/moc
release:OBJECTS_DIR = tmp/release
else:OBJECTS_DIR = tmp/debug
DESTDIR = ../lib/

PROTOPATH = src/main/cpp/hoot/core/proto/
PROTOS = $${PROTOPATH}/OsmFormat.proto $${PROTOPATH}/FileFormat.proto
PROTO_TMP=src/main/cpp/hoot/core/proto/

include(protobuf.pri)

PRECOMPILED_HEADER = src/main/cpp/hoot/core/HootCoreStable.h

SOURCES += $$files(src/*.cpp, true)
HEADERS += $$files(src/*.h, true)

log4cxx {
  SOURCES -= src/main/cpp/hoot/core/util/LogGeneric.cpp
  HEADERS -= src/main/cpp/hoot/core/util/LogGeneric.h
} else {
  SOURCES -= src/main/cpp/hoot/core/util/LogLog4Cxx.cpp
  HEADERS -= src/main/cpp/hoot/core/util/LogLog4Cxx.h
}
