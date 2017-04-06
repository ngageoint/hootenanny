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


// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/manipulators/DividedHighwayManipulation.h>
#include <hoot/core/visitors/FindWaysVisitor.h>
using namespace hoot;

// Qt
#include <QDebug>

// TGS
#include <tgs/StreamUtils.h>
using namespace Tgs;

namespace hoot
{

class DividedHighwayManipulatorTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(DividedHighwayManipulatorTest);
    CPPUNIT_TEST(individualManipulationsTest);
    CPPUNIT_TEST_SUITE_END();

public:

  void individualManipulationsTest()
  {
    OsmXmlReader reader;

    boost::shared_ptr<OsmMap> map(new OsmMap());
    OsmMap::resetCounters();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DividedHighway.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/UndividedHighway.osm", map);

    MapProjector::projectToOrthographic(map);

    long left  = FindWaysVisitor::findWaysByTag(map, "note", "0")[0];
    long right = FindWaysVisitor::findWaysByTag(map, "note", "1")[0];
    long mid   = FindWaysVisitor::findWaysByTag(map, "note", "2")[0];

    DividedHighwayManipulation uut(left, right, mid, map, 10.0);
    qDebug() << uut.getScoreEstimate();
    qDebug() << uut.calculateScore(map);
    set<ElementId> ignored1, ignored2;
    boost::shared_ptr<OsmMap> after(new OsmMap(map));
    uut.applyManipulation(after, ignored1, ignored2);

    left  = FindWaysVisitor::findWaysByTag(map, "note", "3")[0];
    right = FindWaysVisitor::findWaysByTag(map, "note", "4")[0];
    mid   = FindWaysVisitor::findWaysByTag(map, "note", "5")[0];

    DividedHighwayManipulation uut2(left, right, mid, after, 10.0);
    qDebug() << uut2.getScoreEstimate();
    qDebug() << uut2.calculateScore(after);
    uut2.applyManipulation(after, ignored1, ignored2);

    left  = FindWaysVisitor::findWaysByTag(map, "note", "6")[0];
    right = FindWaysVisitor::findWaysByTag(map, "note", "7")[0];
    mid   = FindWaysVisitor::findWaysByTag(map, "note", "8")[0];

    DividedHighwayManipulation uut3(left, right, mid, after, 10.0);
    qDebug() << uut3.getScoreEstimate();
    qDebug() << uut3.calculateScore(after);
    uut3.applyManipulation(after, ignored1, ignored2);

    MapProjector::projectToWgs84(after);

    OsmXmlWriter writer;
    writer.write(after, "test-output/DividedHighwayManipulatorTest.osm");
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(DividedHighwayManipulatorTest);

}
