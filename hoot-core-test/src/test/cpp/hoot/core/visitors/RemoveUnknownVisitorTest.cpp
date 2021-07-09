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
 * @copyright Copyright (C) 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
// hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/visitors/RemoveUnknownVisitor.h>

namespace hoot
{

class RemoveUnknownVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RemoveUnknownVisitorTest);
  CPPUNIT_TEST(runUnknown1Test);
  CPPUNIT_TEST(runUnknown2Test);
  CPPUNIT_TEST_SUITE_END();

public:

  RemoveUnknownVisitorTest()
    : HootTestFixture("test-files/",
                      "test-output/visitors/RemoveUnknownVisitorTest/")
  {
  }

  OsmMapPtr loadMap()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);
    //  Load up ToyTestA as Unknown1
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "ToyTestA.osm", map);
    //  Load up ToyTestB as Unknown2
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(_inputPath + "ToyTestB.osm", map);

    return map;
  }

  void runUnknown1Test()
  {
    OsmMapPtr map = loadMap();
    RemoveUnknown1Visitor visitor;
    map->visitRw(visitor);

    OsmXmlWriter writer;
    writer.write(map, _outputPath + "RemoveUnknown1VisitorOutput.osm");
    HOOT_FILE_EQUALS( _inputPath + "ToyTestB.osm",
                     _outputPath + "RemoveUnknown1VisitorOutput.osm");
  }

  void runUnknown2Test()
  {
    OsmMapPtr map = loadMap();
    RemoveUnknown2Visitor visitor;
    map->visitRw(visitor);

    OsmXmlWriter writer;
    writer.write(map, _outputPath + "RemoveUnknown2VisitorOutput.osm");
    HOOT_FILE_EQUALS( _inputPath + "ToyTestA.osm",
                     _outputPath + "RemoveUnknown2VisitorOutput.osm");
  }

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RemoveUnknownVisitorTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RemoveUnknownVisitorTest, "quick");

}


