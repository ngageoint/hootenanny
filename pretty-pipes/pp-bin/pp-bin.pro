exists ("../../Configure.pri") {
    include("../../Configure.pri")
}
else {
    include("../Configure.pri")
}

TEMPLATE = app

CONFIG += rtti \
    debug

TARGET = PrettyPipes

DEPENDPATH += \
  ../pp-lib/src/main/cpp/ \

INCLUDEPATH += \
  $${DEPENDPATH} \

UI_DIR = tmp/ui
MOC_DIR = tmp/moc
release:OBJECTS_DIR = tmp/release
else:OBJECTS_DIR = tmp/debug
DESTDIR = ../../bin/

LIBS += -L../../lib/ -lPrettyPipes \
    #setting this in ax_lib_hadoop.m4 causes a linking error 
    -lhadooputils \

HEADERS += \

SOURCES += src/main/cpp/pp/main.cpp \
