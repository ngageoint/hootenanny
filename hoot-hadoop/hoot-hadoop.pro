CONFIG += debug

include(../Configure.pri)

QT += testlib \
    xml \

TEMPLATE = lib

CONFIG += rtti \
    qtestlib \

TARGET = HootHadoop

DEPENDPATH += \
  ../hoot-core/src/main/cpp \
  ../hoot-core/src/main/cpp/hoot/core/ \
  ../hoot-core-test/src/test/cpp/ \
  ../tgs/src/main/cpp \
  ../tgs/src/main/cpp/tgs \
  src/main/cpp/ \
  ../pretty-pipes/pp-lib/src/main/cpp \

INCLUDEPATH += \
  $${DEPENDPATH} \

UI_DIR = tmp/ui
MOC_DIR = tmp/moc
release:OBJECTS_DIR = tmp/release
else:OBJECTS_DIR = tmp/debug
DESTDIR = ../lib/

LIBS += -L../lib/ -lHootCore \
  -L../pretty-pipes/lib/ -lPrettyPipes \

# This avoids a conflict with an "emit" function defined in Hadoop Pipes.
DEFINES += QT_NO_EMIT

cppunit {
HEADERS += \
    src/test/cpp/hoot/hadoop/MapReduceTestFixture.h \

SOURCES += \
    src/test/cpp/hoot/hadoop/TestPlugin.cpp \
    src/test/cpp/hoot/hadoop/pbf/PbfInputFormatTest.cpp \
    src/test/cpp/hoot/hadoop/pbf/PbfRecordWriterTest.cpp \
    src/test/cpp/hoot/hadoop/way-join/WayJoin1Test.cpp \
    src/test/cpp/hoot/hadoop/way-join/WayJoin2InputFormatTest.cpp \
    src/test/cpp/hoot/hadoop/way-join/WayJoinDriverTest.cpp \
    src/test/cpp/hoot/hadoop/conflate/ConflateDriverTest.cpp \
    src/test/cpp/hoot/hadoop/paint-nodes/PaintNodesDriverTest.cpp \
    src/test/cpp/hoot/hadoop/HadoopTileWorkerTest.cpp \
    src/test/cpp/hoot/hadoop/stats/MapStatsDriverTest.cpp \
    src/test/cpp/hoot/hadoop/HadoopTileWorker2Test.cpp \
    src/test/cpp/hoot/hadoop/stats/MapStatsTest.cpp
}

HEADERS += \
    src/main/cpp/hoot/hadoop/pbf/PbfInputFormat.h \
    src/main/cpp/hoot/hadoop/pbf/PbfInputSplit.h \
    src/main/cpp/hoot/hadoop/pbf/PbfRecordReader.h \
    src/main/cpp/hoot/hadoop/paint-nodes/PaintNodesMapper.h \
    src/main/cpp/hoot/hadoop/MatrixRecordWriter.h \
    src/main/cpp/hoot/hadoop/OsmMapMapper.h \
    src/main/cpp/hoot/hadoop/paint-nodes/PaintNodesReducer.h \
    src/main/cpp/hoot/hadoop/NodeDensity.h \
    src/main/cpp/hoot/hadoop/paint-nodes/PaintNodesDriver.h \
    src/main/cpp/hoot/hadoop/way-join/WayJoinDriver.h \
    src/main/cpp/hoot/hadoop/cmd/WayJoinCmd.h \
    src/main/cpp/hoot/hadoop/pbf/PbfRecordWriter.h \
    src/main/cpp/hoot/hadoop/way-join/WayJoin1Mapper.h \
    src/main/cpp/hoot/hadoop/way-join/WayJoin1Reducer.h \
    src/main/cpp/hoot/hadoop/OsmMapReducer.h \
    src/main/cpp/hoot/hadoop/way-join/WayJoin2Mapper.h \
    src/main/cpp/hoot/hadoop/way-join/WayJoin2Reducer.h \
    src/main/cpp/hoot/hadoop/way-join/WayJoin2InputFormat.h \
    src/main/cpp/hoot/hadoop/way-join/WayJoin2InputSplit.h \
    src/main/cpp/hoot/hadoop/way-join/WayJoin2RecordReader.h \
    src/main/cpp/hoot/hadoop/HadoopIdGenerator.h \
    src/main/cpp/hoot/hadoop/stats/MapStats.h \
    src/main/cpp/hoot/hadoop/HadoopTileWorker.h \
    src/main/cpp/hoot/hadoop/conflate/ConflateMapper.h \
    src/main/cpp/hoot/hadoop/conflate/ConflateReducer.h \
    src/main/cpp/hoot/hadoop/conflate/ConflateDriver.h \
    src/main/cpp/hoot/hadoop/stats/MapStatsMapper.h \
    src/main/cpp/hoot/hadoop/stats/MapStatsDriver.h \
    src/main/cpp/hoot/hadoop/Debug.h \
    src/main/cpp/hoot/hadoop/HadoopTileWorker2.h \
    src/main/cpp/hoot/hadoop/fourpass/TileOpDriver.h \
    src/main/cpp/hoot/hadoop/fourpass/TileOpMapper.h \
    src/main/cpp/hoot/hadoop/fourpass/TileOpReducer.h \
    src/main/cpp/hoot/hadoop/Driver.h

SOURCES += \
    src/main/cpp/hoot/hadoop/pbf/PbfInputFormat.cpp \
    src/main/cpp/hoot/hadoop/pbf/PbfInputSplit.cpp \
    src/main/cpp/hoot/hadoop/pbf/PbfRecordReader.cpp \
    src/main/cpp/hoot/hadoop/paint-nodes/PaintNodesMapper.cpp \
    src/main/cpp/hoot/hadoop/MatrixRecordWriter.cpp \
    src/main/cpp/hoot/hadoop/OsmMapMapper.cpp \
    src/main/cpp/hoot/hadoop/paint-nodes/PaintNodesReducer.cpp \
    src/main/cpp/hoot/hadoop/NodeDensity.cpp \
    src/main/cpp/hoot/hadoop/paint-nodes/PaintNodesDriver.cpp \
    src/main/cpp/hoot/hadoop/way-join/WayJoinDriver.cpp \
    src/main/cpp/hoot/hadoop/cmd/WayJoinCmd.cpp \
    src/main/cpp/hoot/hadoop/pbf/PbfRecordWriter.cpp \
    src/main/cpp/hoot/hadoop/way-join/WayJoin1Mapper.cpp \
    src/main/cpp/hoot/hadoop/way-join/WayJoin1Reducer.cpp \
    src/main/cpp/hoot/hadoop/OsmMapReducer.cpp \
    src/main/cpp/hoot/hadoop/way-join/WayJoin2Mapper.cpp \
    src/main/cpp/hoot/hadoop/way-join/WayJoin2Reducer.cpp \
    src/main/cpp/hoot/hadoop/way-join/WayJoin2InputFormat.cpp \
    src/main/cpp/hoot/hadoop/way-join/WayJoin2InputSplit.cpp \
    src/main/cpp/hoot/hadoop/way-join/WayJoin2RecordReader.cpp \
    src/main/cpp/hoot/hadoop/HadoopIdGenerator.cpp \
    src/main/cpp/hoot/hadoop/stats/MapStats.cpp \
    src/main/cpp/hoot/hadoop/HadoopTileWorker.cpp \
    src/main/cpp/hoot/hadoop/conflate/ConflateMapper.cpp \
    src/main/cpp/hoot/hadoop/conflate/ConflateReducer.cpp \
    src/main/cpp/hoot/hadoop/conflate/ConflateDriver.cpp \
    src/main/cpp/hoot/hadoop/stats/MapStatsMapper.cpp \
    src/main/cpp/hoot/hadoop/stats/MapStatsDriver.cpp \
    src/main/cpp/hoot/hadoop/cmd/BigConflateCmd.cpp \
    src/main/cpp/hoot/hadoop/Debug.cpp \
    src/main/cpp/hoot/hadoop/HadoopTileWorker2.cpp \
    src/main/cpp/hoot/hadoop/fourpass/TileOpDriver.cpp \
    src/main/cpp/hoot/hadoop/fourpass/TileOpMapper.cpp \
    src/main/cpp/hoot/hadoop/fourpass/TileOpReducer.cpp \
    src/main/cpp/hoot/hadoop/cmd/BigMergeNodesCmd.cpp \
    src/main/cpp/hoot/hadoop/cmd/BigCookieCutterCmd.cpp \
    src/main/cpp/hoot/hadoop/cmd/BigPertyCmd.cpp \
    src/main/cpp/hoot/hadoop/cmd/BigUnifyCmd.cpp \
    src/main/cpp/hoot/hadoop/cmd/BigPaintNodesCmd.cpp \
    src/main/cpp/hoot/hadoop/Driver.cpp
