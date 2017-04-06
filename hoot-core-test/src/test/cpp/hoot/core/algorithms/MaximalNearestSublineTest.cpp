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
 * @copyright Copyright (C) 2012, 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/conflate/Conflator.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/MaximalNearestSubline.h>
#include <hoot/core/algorithms/WayAverager.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>
#include <hoot/core/visitors/FindWaysVisitor.h>
using namespace hoot;


// Boost
using namespace boost;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>
#include <QDir>

// Standard
#include <sstream>
using namespace std;

#include "../TestUtils.h"

class MaximalNearestSublineTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(MaximalNearestSublineTest);
    CPPUNIT_TEST(runTest);
    CPPUNIT_TEST(funnyCurveTest);
    CPPUNIT_TEST(oneShortTest);
    CPPUNIT_TEST_SUITE_END();

public:

    void runTest()
    {
      OsmXmlReader reader;

      boost::shared_ptr<OsmMap> map(new OsmMap());
      reader.setDefaultStatus(Status::Unknown1);
      reader.setUseDataSourceIds(true);
      reader.read("test-files/MaximalNearestSubline.osm", map);

      boost::shared_ptr<OsmMap> map2(new OsmMap(map->getProjection()));

      boost::shared_ptr<OGRSpatialReference> srs =
          MapProjector::createAeacProjection(CalculateMapBoundsVisitor::getBounds(map));
      MapProjector::project(map, srs);

      stringstream ss;

      boost::shared_ptr<Way> w;
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

//      {
//        boost::shared_ptr<OsmMap> wgs84(new OsmMap(map2));
//        MapProjector::reprojectToWgs84(wgs84);
//        OsmXmlWriter writer;
//        QString fn = QString("test-output/algorithms/MaximalNearestSublineTestOutput.osm");
//        writer.write(wgs84, fn);
//      }

      QFile fp("test-files/algorithms/MaximalNearestSublineTest.txt");
      fp.open(QIODevice::ReadOnly);
      QString s = fp.readAll();
      CPPUNIT_ASSERT_EQUAL(s.toStdString(), ss.str());

    }

    void funnyCurveTest()
    {
      TestUtils::resetEnvironment();

      OsmXmlReader reader;

      boost::shared_ptr<OsmMap> map(new OsmMap());
      reader.setDefaultStatus(Status::Unknown1);
      reader.read("test-files/MaximalNearestSubline2.osm", map);

      MapProjector::projectToPlanar(map);

      long n1 = FindWaysVisitor::findWaysByTag(map, "note", "1")[0];
      long n2 = FindWaysVisitor::findWaysByTag(map, "note", "2")[0];
      boost::shared_ptr<Way> left = MaximalNearestSubline::getMaximalNearestSubline(map,
            map->getWay(n1),
            map->getWay(n2),
            10.0, 10.0);
      left->setStatus(Status::Conflated);
      left->setTag("name", "left");
      map->addWay(left);
      //cout << ElementConverter(map).convertToLineString(left)->toString() << endl;

      boost::shared_ptr<Way> right = MaximalNearestSubline::getMaximalNearestSubline(map,
            map->getWay(n2),
            map->getWay(n1),
            10.0, 10.0);
      right->setStatus(Status::Conflated);
      left->setTag("name", "right");
      map->addWay(right);
      //cout << ElementConverter(map).convertToLineString(right)->toString() << endl;

      boost::shared_ptr<Way> w = WayAverager::average(map, right, left);
      w->setStatus(Status::Conflated);
      w->setTag("name", "average");
      map->addWay(w);
      //map->removeWay(n1);
      //map->removeWay(n2);
      QDir().mkpath("test-output/algorithms/");

      {
        boost::shared_ptr<OsmMap> wgs84(new OsmMap(map));
        MapProjector::projectToWgs84(wgs84);
        OsmXmlWriter writer;
        writer.setIncludeCompatibilityTags(false);
        writer.setIncludeHootInfo(false);
        writer.setIncludeIds(false);
        QString fn = QString("test-output/algorithms/MaximalNearestSubline2TestOutput.osm");
        writer.write(wgs84, fn);
      }

      HOOT_FILE_EQUALS("test-files/algorithms/MaximalNearestSubline2TestOutput.osm",
                       "test-output/algorithms/MaximalNearestSubline2TestOutput.osm");
    }


    void oneShortTest()
    {
      OsmXmlReader reader;

      OsmMap::resetCounters();

      boost::shared_ptr<OsmMap> map(new OsmMap());
      reader.setDefaultStatus(Status::Unknown1);
      reader.read("test-files/MaximalNearestSubline2.osm", map);

      MapProjector::projectToPlanar(map);

      long n3 = FindWaysVisitor::findWaysByTag(map, "note", "3")[0];
      long n4 = FindWaysVisitor::findWaysByTag(map, "note", "4")[0];
      boost::shared_ptr<Way> left = MaximalNearestSubline::getMaximalNearestSubline(
            map,
            map->getWay(n3),
            map->getWay(n4),
            10.0, 10.0);
      left->setStatus(Status::Conflated);
      left->setTag("name", "left");
      map->addWay(left);
      //cout << ElementConverter(map).convertToLineString(left)->toString() << endl;

      boost::shared_ptr<Way> right = MaximalNearestSubline::getMaximalNearestSubline(
            map,
            map->getWay(n4),
            map->getWay(n3),
            10.0, 10.0);
      right->setStatus(Status::Conflated);
      right->setTag("name", "right");
      map->addWay(right);
      //cout << ElementConverter(map).convertToLineString(right)->toString() << endl;

      boost::shared_ptr<Way> w = WayAverager::average(map, right, left);
      w->setStatus(Status::Conflated);
      w->setTag("name", "average");
      map->addWay(w);
      //map->removeWay(n1);
      //map->removeWay(n2);

      QDir().mkpath("test-output/algorithms/");

      {
        boost::shared_ptr<OsmMap> wgs84(new OsmMap(map));
        MapProjector::projectToWgs84(wgs84);
        OsmXmlWriter writer;
        QString fn = QString("test-output/algorithms/MaximalNearestSublineOneShortTestOutput.osm");
        writer.write(wgs84, fn);
      }

//      QFile fp("test-files/algorithms/MaximalNearestSublineOneShortTestOutput.osm");
//      fp.open(QIODevice::ReadOnly);
//      QString s1 = fp.readAll();

//      QFile fp2("test-output/algorithms/MaximalNearestSublineOneShortTestOutput.osm");
//      fp2.open(QIODevice::ReadOnly);
//      QString s2 = fp2.readAll();

//      CPPUNIT_ASSERT_EQUAL(s1.toStdString(), s2.toStdString());

    }
};


//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MaximalNearestSublineTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MaximalNearestSublineTest, "quick");

