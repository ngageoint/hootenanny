
CONFIG += debug

exists ("../../Configure.pri") {
    include("../../Configure.pri")
}
else {
    include("../Configure.pri")
}

QT += testlib \
    xml \

TEMPLATE = lib

CONFIG += rtti \

TARGET = PrettyPipes

DEPENDPATH += \
  src/main/cpp/ \

INCLUDEPATH += . \
  $${DEPENDPATH} \

UI_DIR = tmp/ui
MOC_DIR = tmp/moc
release:OBJECTS_DIR = tmp/release
else:OBJECTS_DIR = tmp/debug
DESTDIR = ../../lib/

# This is needed to make the swig code happy.
QMAKE_CXXFLAGS += -fno-strict-aliasing

# Hadoop Pipes can be a bit obnoxious and specifying this with pragmas doesn't work on RHEL 5.8
QMAKE_CXXFLAGS += -Wno-unused-parameter -Wno-strict-aliasing

# This avoids a conflict with an "emit" function defined in Hadoop Pipes.
DEFINES += QT_NO_EMIT

SWIG_FILES = src/main/cpp/pp/swig.i

swig.files=src/main/cpp/pp/
swig.commands=swig -c++ -java -package pp -outdir src/main/java/pp/ -o tmp/swig/HootSwig.cxx src/main/cpp/pp/swig.i
swig.input=SWIG_FILES
swig.variable_out=dum
swig.output=tmp/swig/HootSwig.cxx
swig.name=swig

QMAKE_EXTRA_COMPILERS += swig

cppunit {
HEADERS += \

SOURCES += \
    src/test/cpp/pp/HdfsTest.cpp \
    src/test/cpp/pp/TestPlugin.cpp \
    src/test/cpp/pp/DataInputStreamTest.cpp \
    src/test/cpp/pp/io/CppSeqFileTest.cpp \
    src/test/cpp/pp/io/CppSeqFileRecordReaderTest.cpp \
    src/test/cpp/pp/io/CppSeqFileRecordWriterTest.cpp \
    src/test/cpp/pp/io/LineRecordReaderTest.cpp \
    src/test/cpp/pp/io/LineRecordWriterTest.cpp \

}

HEADERS += \
    src/main/cpp/pp/Hdfs.h \
    src/main/cpp/pp/HdfsDevice.hpp \
    src/main/cpp/pp/InputFormat.h \
    src/main/cpp/pp/InputSplit.h \
    src/main/cpp/pp/Factory.h \
    src/main/cpp/pp/Exception.h \
    src/main/cpp/pp/Strings.h \
    src/main/cpp/pp/mapreduce/Job.h \
    src/main/cpp/pp/conf/Configuration.h \
    src/main/cpp/pp/DataInputStream.h \
    src/main/cpp/pp/DataOutputStream.h \
    src/main/cpp/pp/JobConfConsumer.h \
    src/main/cpp/pp/MapContextConsumer.h \
    src/main/cpp/pp/io/CppSeqFile.h \
    src/main/cpp/pp/io/FileInputSplit.h \
    src/main/cpp/pp/io/FileInputFormat.h \
    src/main/cpp/pp/io/CppSeqFileRecordReader.h \
    src/main/cpp/pp/io/CppSeqFileRecordWriter.h \
    src/main/cpp/pp/io/RecordWriterProvider.h \
    src/main/cpp/pp/ReduceContextConsumer.h \
    src/main/cpp/pp/JobConfThief.h \
    src/main/cpp/pp/Plugins.h \
    src/main/cpp/pp/HadoopPipesUtils.h \
    src/main/cpp/pp/io/RecordWriter.h \
    src/main/cpp/pp/util/Iterator.h \
    src/main/cpp/pp/RecordReader.h \
    src/main/cpp/pp/HdfsConnection.h \
    src/main/cpp/pp/io/LineRecordReader.h \
    src/main/cpp/pp/io/LineRecordWriter.h \
    src/main/cpp/pp/mapreduce/LocalJobRunner.h \
    src/main/cpp/pp/io/RecordReaderProvider.h \
    src/main/cpp/pp/mapreduce/NullMapper.h \
    src/main/cpp/pp/mapreduce/NullReducer.h \
    src/main/cpp/pp/mapreduce/Mapper.h \
    src/main/cpp/pp/mapreduce/Reducer.h \
    src/main/cpp/pp/mapreduce/RecordWriter.h

SOURCES += \
    tmp/swig/HootSwig.cxx \
    src/main/cpp/pp/Hdfs.cpp \
    src/main/cpp/pp/InputFormat.cpp \
    src/main/cpp/pp/InputSplit.cpp \
    src/main/cpp/pp/Factory.cpp \
    src/main/cpp/pp/Exception.cpp \
    src/main/cpp/pp/mapreduce/Job.cpp \
    src/main/cpp/pp/conf/Configuration.cpp \
    src/main/cpp/pp/DataInputStream.cpp \
    src/main/cpp/pp/DataOutputStream.cpp \
    src/main/cpp/pp/io/CppSeqFile.cpp \
    src/main/cpp/pp/io/FileInputSplit.cpp \
    src/main/cpp/pp/io/FileInputFormat.cpp \
    src/main/cpp/pp/io/CppSeqFileRecordReader.cpp \
    src/main/cpp/pp/io/CppSeqFileRecordWriter.cpp \
    src/main/cpp/pp/Plugins.cpp \
    src/main/cpp/pp/HadoopPipesUtils.cpp \
    src/main/cpp/pp/RecordReader.cpp \
    src/main/cpp/pp/HdfsConnection.cpp \
    src/main/cpp/pp/io/LineRecordReader.cpp \
    src/main/cpp/pp/io/LineRecordWriter.cpp \
    src/main/cpp/pp/mapreduce/LocalJobRunner.cpp \
    src/main/cpp/pp/mapreduce/NullMapper.cpp \
    src/main/cpp/pp/mapreduce/NullReducer.cpp \
    src/main/cpp/pp/mapreduce/Mapper.cpp \
    src/main/cpp/pp/mapreduce/Reducer.cpp \
    src/main/cpp/pp/mapreduce/RecordWriter.cpp

OTHER_FILES += $$files(src/*.i, true)
