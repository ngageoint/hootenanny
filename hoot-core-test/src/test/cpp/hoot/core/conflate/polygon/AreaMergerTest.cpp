// Hoot
#include "../../TestUtils.h"
#include <hoot/core/conflate/polygon/AreaMerger.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

// Qt
#include <QDir>

namespace hoot
{

class AreaMergerTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(AreaMergerTest);
  CPPUNIT_TEST(merge2Ways);
  CPPUNIT_TEST(merge2Relations);
  CPPUNIT_TEST(mergeRefWaySecRelation);
  CPPUNIT_TEST(mergeRefRelationSecWay);
  CPPUNIT_TEST(merge2Refs);
  CPPUNIT_TEST(merge2Secs);
  CPPUNIT_TEST(mergeEmptyMap);
  CPPUNIT_TEST(mergeTooManyElements);
  CPPUNIT_TEST(mergeInvalidStatus);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp()
  {
    TestUtils::mkpath("test-output/conflate/polygon/AreaMergerTest");
  }

  void merge2Ways()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/conflate/polygon/merge2Ways-in.osm", false, Status::Unknown1);

    const ElementId refId = AreaMerger::merge(map);
    CPPUNIT_ASSERT_EQUAL((long)-1, refId.getId());
    CPPUNIT_ASSERT_EQUAL(ElementType::Relation, refId.getType().getEnum());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(
      map, "test-output/conflate/polygon/merge2Ways-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/conflate/polygon/merge2Ways-out.osm",
      "test-output/conflate/polygon/merge2Ways-out.osm");
  }

  void merge2Relations()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/conflate/polygon/merge2Relations-in.osm", false, Status::Unknown1);

    const ElementId refId = AreaMerger::merge(map);
    CPPUNIT_ASSERT_EQUAL((long)-1, refId.getId());
    CPPUNIT_ASSERT_EQUAL(ElementType::Relation, refId.getType().getEnum());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(
      map, "test-output/conflate/polygon/merge2Relations-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/conflate/polygon/merge2Relations-out.osm",
      "test-output/conflate/polygon/merge2Relations-out.osm");
  }

  void mergeRefWaySecRelation()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/conflate/polygon/mergeRefWaySecRelation-in.osm", false, Status::Unknown1);

    const ElementId refId = AreaMerger::merge(map);
    CPPUNIT_ASSERT_EQUAL((long)-1, refId.getId());
    CPPUNIT_ASSERT_EQUAL(ElementType::Relation, refId.getType().getEnum());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(
      map, "test-output/conflate/polygon/mergeRefWaySecRelation-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/conflate/polygon/mergeRefWaySecRelation-out.osm",
      "test-output/conflate/polygon/mergeRefWaySecRelation-out.osm");
  }

  void mergeRefRelationSecWay()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/conflate/polygon/mergeRefRelationSecWay-in.osm", false, Status::Unknown1);

    const ElementId refId = AreaMerger::merge(map);
    CPPUNIT_ASSERT_EQUAL((long)-1, refId.getId());
    CPPUNIT_ASSERT_EQUAL(ElementType::Relation, refId.getType().getEnum());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(
      map, "test-output/conflate/polygon/mergeRefRelationSecWay-in.osm");

    HOOT_FILE_EQUALS(
      "test-files/conflate/polygon/mergeRefRelationSecWay-out.osm",
      "test-output/conflate/polygon/mergeRefRelationSecWay-out.osm");
  }

  void merge2Refs()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map, "test-files/conflate/polygon/merge2Refs-in.osm", false, Status::Unknown1);

      AreaMerger::merge(map);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(
      "No secondary area was passed to the Area Merger.", exceptionMsg.toStdString());
  }

  void merge2Secs()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map, "test-files/conflate/polygon/merge2Secs-in.osm", false, Status::Unknown1);

      AreaMerger::merge(map);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(
      "No reference area was passed to the Area Merger.", exceptionMsg.toStdString());
  }

  void mergeEmptyMap()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map, "test-files/conflate/polygon/mergeEmptyMap-in.osm", false, Status::Unknown1);

      AreaMerger::merge(map);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(
      "The Area Merger expects two areas passed to it.  Number passed: 0",
      exceptionMsg.toStdString());
  }

  void mergeTooManyElements()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map, "test-files/conflate/polygon/mergeTooManyElements-in.osm", false, Status::Unknown1);

      AreaMerger::merge(map);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(
      "The Area Merger expects two areas passed to it.  Number passed: 3",
      exceptionMsg.toStdString());
  }

  void mergeInvalidStatus()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map, "test-files/conflate/polygon/mergeInvalidStatus-in.osm", false, Status::Unknown1);

      AreaMerger::merge(map);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(
      "Passed a way with an invalid status to the Area Merger: Invalid",
      exceptionMsg.toStdString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(AreaMergerTest, "quick");

}
