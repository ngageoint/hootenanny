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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2017, 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/splitter/DualHighwaySplitter.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/elements/MapProjector.h>

// TGS
#include <tgs/StreamUtils.h>
using namespace Tgs;

namespace hoot
{

class DualHighwaySplitterTest : public HootTestFixture
{
    CPPUNIT_TEST_SUITE(DualHighwaySplitterTest);
    CPPUNIT_TEST(simpleTest);
    CPPUNIT_TEST(allTest);
    CPPUNIT_TEST_SUITE_END();

public:

  DualHighwaySplitterTest()
    : HootTestFixture("test-files/algorithms/splitter/",
                      "test-output/algorithms/splitter/")
  {
    setResetType(ResetBasic);
  }

  void simpleTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "DualHighwaySplitterSimpleInput.osm", map);

    MapProjector::projectToOrthographic(map);

    OsmMapPtr after = DualHighwaySplitter::splitAll(map, DualHighwaySplitter::Right, 10.0);

    MapProjector::projectToWgs84(after);

    OsmXmlWriter writer;
    writer.write(after, _outputPath + "DualHighwaySplitterSimpleOutput.osm");

    HOOT_FILE_EQUALS( _inputPath + "DualHighwaySplitterSimpleExpected.osm",
                     _outputPath + "DualHighwaySplitterSimpleOutput.osm");
  }

  void allTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/UndividedHighway.osm", map);

    MapProjector::projectToOrthographic(map);

    OsmMapPtr after = DualHighwaySplitter::splitAll(map, DualHighwaySplitter::Right, 10.0);

    MapProjector::projectToWgs84(after);

    OsmXmlWriter writer;
    writer.write(after, _outputPath + "DualHighwaySplitterTest.osm");

    HOOT_FILE_EQUALS( _inputPath + "DualHighwaySplitterTestExpected.osm",
                     _outputPath + "DualHighwaySplitterTest.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DualHighwaySplitterTest, "quick");

}
