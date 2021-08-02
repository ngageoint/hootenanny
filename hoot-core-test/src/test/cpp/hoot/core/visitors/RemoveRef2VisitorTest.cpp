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
 * @copyright Copyright (C) 2014, 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/visitors/RemoveRef2Visitor.h>
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/elements/MapProjector.h>

// TGS
#include <tgs/Statistics/Random.h>
using namespace Tgs;

namespace hoot
{

class RemoveRef2VisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RemoveRef2VisitorTest);
  CPPUNIT_TEST(runToyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  RemoveRef2VisitorTest()
    : HootTestFixture("test-files/visitors/RemoveRef2VisitorTest/",
                      "test-output/visitors/RemoveRef2VisitorTest/")
  {
    setResetType(ResetAll);
  }

  void runToyTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, _inputPath + "RemoveRef2VisitorInput.osm");

    ElementCriterionPtr c = std::make_shared<PoiCriterion>();
    RemoveRef2Visitor v;
    v.addCriterion(c);
    map->visitRw(v);

    const QString testFileName = "runToyTest.json";
    OsmJsonWriter writer(8);
    writer.setIncludeCompatibilityTags(false);
    writer.open(_outputPath + testFileName);
    MapProjector::projectToWgs84(map);
    writer.write(map);
    writer.close();
    HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RemoveRef2VisitorTest, "quick");

}


