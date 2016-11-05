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

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/splitter/IntersectionSplitter.h>
#include <hoot/core/io/OsmReader.h>
#include <hoot/core/io/OsmWriter.h>
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

#include "../../TestUtils.h"

class IntersectionSplitterTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(IntersectionSplitterTest);
    CPPUNIT_TEST(runTest);
    CPPUNIT_TEST(runTestSimple);
    CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    QDir().mkpath("test-output/conflate/splitter");

    OsmReader reader;

    OsmMap::resetCounters();

    shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/conflate/splitter/IntersectionSplitter.osm", map);

    IntersectionSplitter::splitIntersections(map);

    OsmWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, "test-output/conflate/splitter/IntersectionSplitterTest.osm");

    HOOT_FILE_EQUALS("test-output/conflate/splitter/IntersectionSplitterTest.osm",
                     "test-files/conflate/splitter/IntersectionSplitterOut.osm");
  }

  void runTestSimple()
  {
    QDir().mkpath("test-output/conflate/splitter");

    OsmReader reader;
    OsmMap::resetCounters();
    shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/conflate/splitter/SimpleSplitter.osm", map);

    IntersectionSplitter::splitIntersections(map);

    OsmWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, "test-output/conflate/splitter/SimpleSplitterOutput.osm");

    HOOT_FILE_EQUALS("test-output/conflate/splitter/SimpleSplitterOutput.osm",
                     "test-files/conflate/splitter/SimpleSplitterExpected.osm");
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(IntersectionSplitterTest);

