TEMPLATE = lib
CONFIG += dll
CONFIG -= flat

#
# Include the shared settings.
#
exists(../../LocalConfig.pri) {
  include(../../LocalConfig.pri)
} else:exists(../../Common/LocalConfig.pri) {
  include(../../Common/LocalConfig.pri)
} else {
  error("You must have a LocalConfig.pri")
}

#CONFIG -= qt

INCLUDEPATH += $${PWD}/../../
INCLUDEPATH += $${CPPUNIT_INC_PATH}

MAKEFILE = TgsTest$${BRANCH_APPEND}
debug:TARGET = $${MAKEFILE}d
release:TARGET = $${MAKEFILE}

DEFINES += TGS_BUILD_DLL

PRECOMPILED_HEADER = TgsTestStable.h

MISC_FILES += \
	TgsTest.pro \
	plugin.conf \

include (TgsTestFiles.pri)

HEADERS += \
	PluginFactory.h \

SOURCES += \
	PluginFactory.cpp \

INCLUDEPATH += $${PWD}/../../Tools/Windows/
INCLUDEPATH += $${THIRDPARTY_PATH}/cppunit/include/
INCLUDEPATH += $${THIRDPARTY_PATH}/boost/include/

INCLUDEPATH += $${THIRDPARTY_PATH}/boost/include
win32:INCLUDEPATH += $${THIRDPARTY_PATH}/NewMat/include
unix:INCLUDEPATH += $${THIRDPARTY_PATH}/newmat/

release {
  win32:LIBS += $${THIRDPARTY_PATH}/cppunit/lib/cppunit_dll.lib
  win32:LIBS += -L../Release -lTgs$${BRANCH_APPEND}
  win32::LIBS += $${THIRDPARTY_PATH}/Newmat/Lib/newmat.lib
  win32:LIBS += $${THIRDPARTY_PATH}/boost/lib/libboost_thread-vc80-mt-1_34_1.lib
  unix:LIBS += -lboost_thread
} else {
  win32:LIBS += $${THIRDPARTY_PATH}/cppunit/lib/cppunitd_dll.lib
  win32:LIBS += -L../Debug -lTgs$${BRANCH_APPEND}d
  win32:LIBS += $${THIRDPARTY_PATH}/Newmat/Lib/newmatd.lib
  win32:LIBS += $${THIRDPARTY_PATH}/boost/lib/libboost_thread-vc80-mt-gd-1_34_1.lib
  unix:LIBS += -lboost_thread
}

