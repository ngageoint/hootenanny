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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/ops/SmallHighwayMerger.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/elements/MapProjector.h>

namespace hoot
{

class SmallHighwayMergerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(SmallHighwayMergerTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  SmallHighwayMergerTest()
    : HootTestFixture("test-files/ops/SmallHighwayMerger/",
                      "test-output/ops/SmallHighwayMerger/")
  {
    setResetType(ResetBasic);
  }

  void runBasicTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "SmallHighwayMergerInput1.osm", map);

    MapProjector::projectToPlanar(map);
    SmallHighwayMerger::mergeWays(map, 15.0);
    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.write(map, _outputPath + "SmallHighwayMergerOutput1.osm");
    HOOT_FILE_EQUALS( _inputPath + "SmallHighwayMergerOutput1.osm",
                     _outputPath + "SmallHighwayMergerOutput1.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SmallHighwayMergerTest, "quick");

}
