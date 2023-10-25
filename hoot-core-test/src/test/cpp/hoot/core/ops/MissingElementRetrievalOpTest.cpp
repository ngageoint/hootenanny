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
 * @copyright Copyright (C) 2023 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/MissingElementRetrievalOp.h>

namespace hoot
{

class MissingElementRetrievalOpTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MissingElementRetrievalOpTest);
  CPPUNIT_TEST(runMissingElementOsmApiTest);
  CPPUNIT_TEST(runMissingElementOverpassTest);
  CPPUNIT_TEST_SUITE_END();

public:

  MissingElementRetrievalOpTest()
    : HootTestFixture("test-files/ops/MissingElementRetrievalOpTest/",
                      "test-output/ops/MissingElementRetrievalOpTest/")
  {
  }

  const QString overpass_url = "https://overpass-api.de/api/interpreter";
  const QString osm_api_url = "https://api.openstreetmap.org/api/0.6/map";

  /** NOTE: This test reaches out to the public OSM API to retrieve all nodes and ways for a single relation */
  void runMissingElementOsmApiTest()
  {
    conf().set(ConfigOptions::getMissingElementRetrievalUrlKey(), osm_api_url);

    MissingElementRetrievalOp uut;
    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();

    reader.setDefaultStatus(Status::Unknown1);
    reader.setUseDataSourceIds(true);
    reader.read(_inputPath + "MissingInput.osm", map);

    uut.apply(map);

    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.write(map, _outputPath + "MissingOutputOsmApi.osm");

    HOOT_FILE_EQUALS( _inputPath + "MissingExpected.osm",
                     _outputPath + "MissingOutputOsmApi.osm");
  }

  /** NOTE: This test reaches out to the public Overpass to retrieve all nodes and ways for a single relation */
  void runMissingElementOverpassTest()
  {
    conf().set(ConfigOptions::getMissingElementRetrievalUrlKey(), overpass_url);

    MissingElementRetrievalOp uut;
    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();

    reader.setDefaultStatus(Status::Unknown1);
    reader.setUseDataSourceIds(true);
    reader.read(_inputPath + "MissingInput.osm", map);

    uut.apply(map);

    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.write(map, _outputPath + "MissingOutputOverpass.osm");

    HOOT_FILE_EQUALS( _inputPath + "MissingExpected.osm",
                     _outputPath + "MissingOutputOverpass.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MissingElementRetrievalOpTest, "quick");

}
