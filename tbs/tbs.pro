##########################
# General statistics and machine learning utilities
##########################

TEMPLATE = lib
win32:CONFIG += dll

TARGET=tbs

CONFIG -= flat

CONFIG += rtti \
    debug

DESTDIR = ../lib/

# Include the settings of our parent project.
exists ("../Configure.pri") {
    include("../Configure.pri")
}

LIBS -= -lhdfs

unix:QMAKE_CXXFLAGS += -Wno-deprecated
unix:QT -= gui
UI_DIR = tmp/ui
MOC_DIR = tmp/moc
release:OBJECTS_DIR = tmp/release
else:OBJECTS_DIR = tmp/debug
DEPENDPATH += ./src/main/cpp/
INCLUDEPATH += $${DEPENDPATH}
MISC_FILES += tbs.pro

cppunit {

# When this list of classes is updated, also update the in docs/CoreAPI.dox. Maybe eventually we can
# automate the process. Fortunately, what we use from tgs/tbs doesn't change very often.

SOURCES += \
    src/test/cpp/tbs/optimization/GoldenSectionSearchTest.cpp \
    src/test/cpp/tbs/stats/TDistributionTest.cpp
}

HEADERS += src/main/cpp/tbs/stats/TDistribution.h \
    src/main/cpp/tbs/optimization/GoldenSectionSearch.h \
    src/main/cpp/tbs/optimization/LineSearch.h \
    src/main/cpp/tbs/stats/SampleStats.h

SOURCES += src/main/cpp/tbs/stats/TDistribution.cpp \
    src/main/cpp/tbs/optimization/GoldenSectionSearch.cpp \
    src/main/cpp/tbs/optimization/LineSearch.cpp \
    src/main/cpp/tbs/stats/SampleStats.cpp

