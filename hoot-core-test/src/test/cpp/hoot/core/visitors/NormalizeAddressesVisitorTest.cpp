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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/visitors/NormalizeAddressesVisitor.h>

namespace hoot
{

class NormalizeAddressesVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(NormalizeAddressesVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  NormalizeAddressesVisitorTest()
    : HootTestFixture("test-files/visitors/NormalizeAddressesVisitorTest/",
                      "test-output/visitors/NormalizeAddressesVisitorTest/")
  {
    setResetType(ResetBasic);
  }

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm",
      false,
      Status::Unknown1);

    NormalizeAddressesVisitor uut;
    map->visitRw(uut);

    const QString outputFile = _outputPath + "out.osm";
    OsmMapWriterFactory::write(map, outputFile);

    CPPUNIT_ASSERT_EQUAL(21, uut._addressNormalizer.getNumNormalized());
    HOOT_FILE_EQUALS(_inputPath + "gold.osm", outputFile);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NormalizeAddressesVisitorTest, "slow");

}


