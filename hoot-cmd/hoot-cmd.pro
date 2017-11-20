# -------------------------------------------------
# Project created by QtCreator 2011-09-29T12:27:47
# -------------------------------------------------

QT += testlib \
    xml \

TARGET = hoot.bin
TEMPLATE = app
DEPENDPATH += \
    ../tgs/src/main/cpp/ \
    ../tgs/src/main/cpp/tgs/ \
    ../hoot-core/src/main/cpp/hoot/core \
    ../hoot-core/src/main/cpp/ \
    ../hoot-hadoop/src/main/cpp/ \

INCLUDEPATH += \
    $${DEPENDPATH}

CONFIG += rtti \
    qtestlib \
    debug

include(../Configure.pri)

LIBS += -L../tgs/ -lTgs -L../lib/ -lHootCore -ltbs

hadoop {
  LIBS += -lHootHadoop -lhadooppipes -lhadooputils -lHootCoreTest
}

UI_DIR = tmp/ui
MOC_DIR = tmp/moc
release:OBJECTS_DIR = tmp/release
else:OBJECTS_DIR = tmp/debug
DESTDIR = ../bin/

SOURCES += src/main/cpp/hoot/cmd/main.cpp \
    src/main/cpp/hoot/cmd/HelpCmd.cpp

HEADERS += \

FORMS +=

