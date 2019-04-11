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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

class OsmXmlWriterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmXmlWriterTest);
  CPPUNIT_TEST(runEncodeCharsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  const QString _inputPath = "test-files/io/OsmXmlWriterTest/";
  const QString _outputPath = "test-output/io/OsmXmlWriterTest/";

  OsmXmlWriterTest()
  {
    setResetType(ResetBasic);
    TestUtils::mkpath(_outputPath);
  }

  void runEncodeCharsTest()
  {
    OsmXmlWriter uut;

    OsmMapPtr map(new OsmMap());
    Tags tags1;
    tags1.set("note", "1 & 2");
    TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags1);
    Tags tags2;
    tags2.set("note", "\"3\"");
    TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags2);
    Tags tags3;
    tags3.set("note", "0\n");
    TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags3);
    Tags tags4;
    tags4.set("note", "<2>");
    TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags4);

    const QString output = _outputPath + "runEncodeCharsTest-out.osm";
    uut.write(map, output);
    HOOT_FILE_EQUALS(_inputPath + "runEncodeCharsTest.osm", output);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmXmlWriterTest, "quick");

}
