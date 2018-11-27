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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/visitors/NormalizePhoneNumbersVisitor.h>

namespace hoot
{

static const QString outputRoot = "test-output/visitors/NormalizePhoneNumbersVisitorTest";

class NormalizePhoneNumbersVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(NormalizePhoneNumbersVisitorTest);
  //TODO: add test with non-US region code
  //TODO: add alt formats test
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  NormalizePhoneNumbersVisitorTest()
  {
    setResetType(ResetBasic);
    TestUtils::mkpath(outputRoot);
  }

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm",
      false,
      Status::Unknown1);

    NormalizePhoneNumbersVisitor uut;
    uut._phoneNumberNormalizer.setRegionCode("US");
    map->visitRw(uut);

    const QString outputFile = outputRoot + "/out.osm";
    OsmMapWriterFactory::getInstance().write(map, outputFile);

    CPPUNIT_ASSERT_EQUAL(12, uut._phoneNumberNormalizer.getNumNormalized());
    HOOT_FILE_EQUALS("test-files/visitors/NormalizePhoneNumbersVisitorTest/gold.osm", outputFile);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NormalizePhoneNumbersVisitorTest, "quick");

}


