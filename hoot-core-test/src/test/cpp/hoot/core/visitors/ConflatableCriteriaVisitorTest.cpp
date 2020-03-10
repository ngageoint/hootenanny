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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/visitors/ConflatableCriteriaVisitor.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

namespace hoot
{

class ConflatableCriteriaVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ConflatableCriteriaVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ConflatableCriteriaVisitorTest() :
    HootTestFixture(
      "test-files/visitors/ConflatableCriteriaVisitorTest/",
      "test-output/visitors/ConflatableCriteriaVisitorTest/")
  {
    setResetType(ResetBasic);
  }

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/conflate/unified/AllDataTypesA.osm", false, Status::Unknown1);

    ConflatableCriteriaVisitor uut;
    uut.setOsmMap(map.get());
    map->visitRw(uut);

    const QString outFileName = "ConflatableCriteriaVisitorTest-runBasicTest.osm";
    OsmMapWriterFactory::write(map, _outputPath + outFileName);

    HOOT_FILE_EQUALS(_inputPath + outFileName, _outputPath + outFileName);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConflatableCriteriaVisitorTest, "quick");

}


