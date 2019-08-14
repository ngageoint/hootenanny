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
 * @copyright Copyright (C) 2014, 2016, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/rnd/visitors/RemoveRef2VisitorMultipleCriterion.h>
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/util/MapProjector.h>

// TGS
#include <tgs/Statistics/Random.h>
using namespace Tgs;

namespace hoot
{

class RemoveRef2VisitorMultipleCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RemoveRef2VisitorMultipleCriterionTest);
  CPPUNIT_TEST(runToyTest1);
  CPPUNIT_TEST(runToyTest2);
  CPPUNIT_TEST_SUITE_END();

public:

  RemoveRef2VisitorMultipleCriterionTest()
    : HootTestFixture("test-files/rnd/visitors/RemoveRef2VisitorMultipleCriterionTest/",
                      "test-output/rnd/visitors/RemoveRef2VisitorMultipleCriterionTest/")
  {
    setResetType(ResetAll);
  }

  void runToyTest1()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "RemoveRef2VisitorMultipleCriterionInput.osm");

    ElementCriterionPtr poiCrit(new PoiCriterion);
    ElementCriterionPtr buildingCrit(new BuildingCriterion(map));
    ElementCriterionPtr chainCrit(new ChainCriterion(poiCrit, buildingCrit));
    RemoveRef2VisitorMultipleCriterion v;
    v.addCriterion(chainCrit);
    map->visitRw(v);

    const QString testFileName = "runToyTest1.json";
    OsmJsonWriter writer(8);
    writer.setIncludeCompatibilityTags(false);
    writer.open(_outputPath + testFileName);
    MapProjector::projectToWgs84(map);
    writer.write(map);
    writer.close();
    HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
  }

  void runToyTest2()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "RemoveRef2VisitorMultipleCriterionInput.osm");

    ElementCriterionPtr poiCrit(new PoiCriterion);
    ElementCriterionPtr buildingCrit(new BuildingCriterion(map));
    ElementCriterionPtr chainCrit(new ChainCriterion(buildingCrit, poiCrit));
    RemoveRef2VisitorMultipleCriterion v;
    v.addCriterion(chainCrit);
    map->visitRw(v);

    const QString testFileName = "runToyTest2.json";
    OsmJsonWriter writer(8);
    writer.setIncludeCompatibilityTags(false);
    writer.open(_outputPath + testFileName);
    MapProjector::projectToWgs84(map);
    writer.write(map);
    writer.close();
    HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RemoveRef2VisitorMultipleCriterionTest, "quick");

}


