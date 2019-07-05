/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/subline-matching/MaximalNearestSubline.h>
#include <hoot/core/algorithms/WayAverager.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Standard
#include <sstream>
using namespace std;

namespace hoot
{

class MaximalNearestSublineTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MaximalNearestSublineTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(funnyCurveTest);
  CPPUNIT_TEST(oneShortTest);
  CPPUNIT_TEST_SUITE_END();

public:

  MaximalNearestSublineTest()
    : HootTestFixture("test-files/algorithms/subline-matching/",
                      "test-output/algorithms/subline-matching/")
  {
    setResetType(ResetAll);
  }

  void runTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.setUseDataSourceIds(true);
    reader.read(_inputPath + "MaximalNearestSubline.osm", map);

    OsmMapPtr map2(new OsmMap(map->getProjection()));

    std::shared_ptr<OGRSpatialReference> srs =
        MapProjector::createAeacProjection(CalculateMapBoundsVisitor::getBounds(map));
    MapProjector::project(map, srs);

    stringstream ss;

    WayPtr w;
    w = MaximalNearestSubline::getMaximalNearestSubline(map, map->getWay(-353),
                                                        map->getWay(-313),
      10.0, 10.0);
    w->setStatus(Status::Conflated);
    ss << ElementConverter(map).convertToLineString(w)->toString() << endl;

    w = MaximalNearestSubline::getMaximalNearestSubline(map, map->getWay(-313),
                                                        map->getWay(-353),
                                                        10.0, 10.0);
    w->setStatus(Status::Conflated);
    ss << ElementConverter(map).convertToLineString(w)->toString() << endl;

    w = MaximalNearestSubline::getMaximalNearestSubline(map, map->getWay(-260),
                                                        map->getWay(-247),
                                                        10.0, 10.0);
    w->setStatus(Status::Conflated);
    ss << ElementConverter(map).convertToLineString(w)->toString() << endl;

    w = MaximalNearestSubline::getMaximalNearestSubline(map, map->getWay(-247),
                                                        map->getWay(-260),
                                                        10.0, 10.0);
    w->setStatus(Status::Conflated);
    ss << ElementConverter(map).convertToLineString(w)->toString() << endl;

    QFile fp(_inputPath + "MaximalNearestSublineTest.txt");
    fp.open(QIODevice::ReadOnly);
    QString s = fp.readAll();
    CPPUNIT_ASSERT_EQUAL(s.toStdString(), ss.str());

  }

  void funnyCurveTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "MaximalNearestSubline2.osm", map);

    MapProjector::projectToPlanar(map);

    long n1 = ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "1")[0];
    long n2 = ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "2")[0];
    WayPtr left = MaximalNearestSubline::getMaximalNearestSubline(map,
          map->getWay(n1),
          map->getWay(n2),
          10.0, 10.0);
    left->setStatus(Status::Conflated);
    left->setTag("name", "left");
    map->addWay(left);

    WayPtr right = MaximalNearestSubline::getMaximalNearestSubline(map,
          map->getWay(n2),
          map->getWay(n1),
          10.0, 10.0);
    right->setStatus(Status::Conflated);
    left->setTag("name", "right");
    map->addWay(right);

    WayPtr w = WayAverager::average(map, right, left);
    w->setStatus(Status::Conflated);
    w->setTag("name", "average");
    map->addWay(w);

    {
      OsmMapPtr wgs84(new OsmMap(map));
      MapProjector::projectToWgs84(wgs84);
      OsmXmlWriter writer;
      writer.setIncludeCompatibilityTags(false);
      writer.setIncludeHootInfo(false);
      writer.setIncludeIds(false);
      writer.write(wgs84, _outputPath + "MaximalNearestSubline2TestOutput.osm");
    }

    HOOT_FILE_EQUALS( _inputPath + "MaximalNearestSubline2TestOutput.osm",
                     _outputPath + "MaximalNearestSubline2TestOutput.osm");
  }


  void oneShortTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "MaximalNearestSubline2.osm", map);

    MapProjector::projectToPlanar(map);

    long n3 = ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "3")[0];
    long n4 = ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "4")[0];
    WayPtr left = MaximalNearestSubline::getMaximalNearestSubline(
          map,
          map->getWay(n3),
          map->getWay(n4),
          10.0, 10.0);
    left->setStatus(Status::Conflated);
    left->setTag("name", "left");
    map->addWay(left);

    WayPtr right = MaximalNearestSubline::getMaximalNearestSubline(
          map,
          map->getWay(n4),
          map->getWay(n3),
          10.0, 10.0);
    right->setStatus(Status::Conflated);
    right->setTag("name", "right");
    map->addWay(right);

    WayPtr w = WayAverager::average(map, right, left);
    w->setStatus(Status::Conflated);
    w->setTag("name", "average");
    map->addWay(w);

    {
      OsmMapPtr wgs84(new OsmMap(map));
      MapProjector::projectToWgs84(wgs84);
      OsmXmlWriter writer;
      QString fn = QString(_outputPath + "MaximalNearestSublineOneShortTestOutput.osm");
      writer.write(wgs84, fn);
    }

//    QFile fp("test-files/algorithms/MaximalNearestSublineOneShortTestOutput.osm");
//    fp.open(QIODevice::ReadOnly);
//    QString s1 = fp.readAll();

//    QFile fp2("test-output/algorithms/MaximalNearestSublineOneShortTestOutput.osm");
//    fp2.open(QIODevice::ReadOnly);
//    QString s2 = fp2.readAll();

//    CPPUNIT_ASSERT_EQUAL(s1.toStdString(), s2.toStdString());

  }
};


//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MaximalNearestSublineTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MaximalNearestSublineTest, "quick");

}
