exists ("../../Configure.pri") {
    include("../../Configure.pri")
}
else {
    include("../Configure.pri")
}

TEMPLATE = lib

CONFIG += rtti \
    debug

QT += testlib \
    xml \


TARGET = PrettyPipesExample

DEPENDPATH += \
  ../pp-lib/src/main/cpp/ \

INCLUDEPATH += \
  ${HADOOP_HOME}/c++/Linux-amd64-64/include/ \
  ${HADOOP_HOME}/src/c++/libhdfs/ \
  ${JAVA_HOME}/include \
  ${JAVA_HOME}/include/linux \
  $${DEPENDPATH}


UI_DIR = tmp/ui
MOC_DIR = tmp/moc
release:OBJECTS_DIR = tmp/release
else:OBJECTS_DIR = tmp/debug
DESTDIR = ../../lib/

DEFINES += QT_NO_EMIT

LIBS += -L${HADOOP_HOME}/c++/Linux-amd64-64/lib -lhadooppipes \
    -L../../lib/ -lPrettyPipes \
    -L${JAVA_HOME}/jre/lib/amd64/server/ -ljvm \

HEADERS += \
  src/main/cpp/pp/example/WordCountMapper.h \
  src/main/cpp/pp/example/WordCountReducer.h \
  src/main/cpp/pp/example/Plugin.h

SOURCES += \
  src/main/cpp/pp/example/WordCountMapper.cpp \
  src/main/cpp/pp/example/WordCountReducer.cpp \
  src/main/cpp/pp/example/Plugin.cpp \

cppunit {
SOURCES += \
  src/main/cpp/pp/example/WordCountTest.cpp
}
