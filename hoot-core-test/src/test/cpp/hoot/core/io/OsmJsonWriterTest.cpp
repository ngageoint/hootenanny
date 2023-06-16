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
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/io/OsmXmlReader.h>

namespace hoot
{

class OsmJsonWriterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmJsonWriterTest);
  CPPUNIT_TEST(runJsonInTagsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OsmJsonWriterTest()
    : HootTestFixture("test-files/io/Json/",
                      "test-output/io/Json/")
  {
  }

  void runJsonInTagsTest()
  {
    QString input = "JsonInTags.osm";
    QString output = "JsonInTags.json";
    OsmXmlReader reader;

    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + input, map);

    OsmJsonWriter writer;

    writer.open(_outputPath + output);

    writer.setIncludeCircularError(false);

    writer.write(map);
    HOOT_FILE_EQUALS( _inputPath + output,
                     _outputPath + output);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmJsonWriterTest, "quick");

}
