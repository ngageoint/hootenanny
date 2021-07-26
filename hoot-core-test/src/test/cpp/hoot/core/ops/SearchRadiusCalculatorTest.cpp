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
 * @copyright Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/ops/MapCleaner.h>
#include <hoot/core/ops/SearchRadiusCalculator.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QBuffer>
#include <QByteArray>

using namespace std;

namespace hoot
{

class SearchRadiusCalculatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(SearchRadiusCalculatorTest);
  CPPUNIT_TEST(runCalcResultTest);
  CPPUNIT_TEST(runNotEnoughTiePointsTest);
  CPPUNIT_TEST(runPreviouslyConflatedDataTest);
  CPPUNIT_TEST_SUITE_END();

public:

  SearchRadiusCalculatorTest()
    : HootTestFixture("test-files/ops/SearchRadiusCalculatorTest/", UNUSED_PATH)
  {
    setResetType(ResetBasic);
  }

  void runCalcResultTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "Haiti_CNIGS_Rivers_REF1-cropped-2.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(_inputPath + "Haiti_osm_waterway_ss_REF2-cropped-2.osm", map);

    MapCleaner().apply(map);

    Settings testSettings = conf();
    testSettings.set("rubber.sheet.ref", "true");
    testSettings.set("rubber.sheet.minimum.ties", "5");
    testSettings.set("rubber.sheet.fail.when.minimum.tie.points.not.found", "false");
    SearchRadiusCalculator searchRadiusCalculator;
    searchRadiusCalculator.setConfiguration(testSettings);

    searchRadiusCalculator.apply(map);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      34.334710, boost::any_cast<double>(searchRadiusCalculator.getResult()), 1e-6);
  }

  void runNotEnoughTiePointsTest()
  {
    DisableLog dl;

    vector<double> tiePointDistances;
    tiePointDistances.push_back(11.0);

    Settings testSettings = conf();
    testSettings.set("rubber.sheet.ref", "true");
    testSettings.set("rubber.sheet.minimum.ties", "5");
    testSettings.set("rubber.sheet.fail.when.minimum.tie.points.not.found", "false");
    SearchRadiusCalculator searchRadiusCalculator;
    searchRadiusCalculator.setConfiguration(testSettings);

    searchRadiusCalculator._calculateSearchRadius(tiePointDistances);
    //if the search radius calculator can't find enough tie points, it will use the specified
    //default search radius
    CPPUNIT_ASSERT_EQUAL(
      ConfigOptions().getCircularErrorDefaultValue(), searchRadiusCalculator._result);
  }

  void runPreviouslyConflatedDataTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "Haiti_CNIGS_Rivers_REF1-cropped-2.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(_inputPath + "Haiti_osm_waterway_ss_REF2-cropped-2.osm", map);

    MapCleaner().apply(map);

    //If any data in the dataset has already been conflated (or is invalid), the operation
    //shouldn't fail.  The data should just be skipped.
    map->getWay(
      ElementIdsVisitor::findElementsByTag(
        map, ElementType::Way, MetadataTags::Ref1(), "001952")[0])->setStatus(Status::Conflated);
    map->getWay(
      ElementIdsVisitor::findElementsByTag(
        map, ElementType::Way,  MetadataTags::Ref2(), "001f4b")[0])->setStatus(Status::Invalid);

    Settings testSettings = conf();
    testSettings.set("rubber.sheet.ref", "true");
    testSettings.set("rubber.sheet.minimum.ties", "5");
    testSettings.set("rubber.sheet.fail.when.minimum.tie.points.not.found", "false");
    SearchRadiusCalculator searchRadiusCalculator;
    searchRadiusCalculator.setConfiguration(testSettings);

    searchRadiusCalculator.apply(map);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      32.675054, boost::any_cast<double>(searchRadiusCalculator.getResult()), 1e-6);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SearchRadiusCalculatorTest, "quick");

}
