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
 * @copyright Copyright (C) 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmReader.h>
#include <hoot/core/ops/CalculateStatsOp.h>
#include <hoot/core/util/ConfigOptions.h>

// CPP Unit
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include "../TestUtils.h"

namespace hoot
{

class CalculateStatsOpTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(CalculateStatsOpTest);
  CPPUNIT_TEST(runStatsNumTest);
  CPPUNIT_TEST(runStatsTest);
  CPPUNIT_TEST(runStatsTestWithReviews);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp()
  {
    TestUtils::resetEnvironment();
    conf().set(ConfigOptions::getStatsTranslateScriptKey(), "${HOOT_HOME}/translations/HootTest.js");
  }

  void tearDown()
  {
    TestUtils::resetEnvironment();
  }

  //this is here just to prevent someone from adding a stat that doesn't get tested in this test
  void runStatsNumTest()
  {
    shared_ptr<CalculateStatsOp> calcStatsOp =
      _calcStats("test-files/ops/CalculateStatsOp/all-data-types.osm");
    CPPUNIT_ASSERT_EQUAL(85, calcStatsOp->getStats().size());
  }

  void runStatsTest()
  {
    shared_ptr<CalculateStatsOp> calcStatsOp =
      _calcStats("test-files/ops/CalculateStatsOp/all-data-types.osm");

    CPPUNIT_ASSERT_EQUAL(201.0, calcStatsOp->getSingleStat("Node Count"));
    CPPUNIT_ASSERT_EQUAL(22.0, calcStatsOp->getSingleStat("Way Count"));
    CPPUNIT_ASSERT_EQUAL(1.0, calcStatsOp->getSingleStat("Relation Count"));

    CPPUNIT_ASSERT_EQUAL(-427.0, calcStatsOp->getSingleStat("Minimum Node ID"));
    CPPUNIT_ASSERT_EQUAL(-1.0, calcStatsOp->getSingleStat("Maximum Node ID"));
    CPPUNIT_ASSERT_EQUAL(-283.0, calcStatsOp->getSingleStat("Minimum Way ID"));
    CPPUNIT_ASSERT_EQUAL(-1.0, calcStatsOp->getSingleStat("Maximum Way ID"));
    CPPUNIT_ASSERT_EQUAL(-1.0, calcStatsOp->getSingleStat("Minimum Relation ID"));
    CPPUNIT_ASSERT_EQUAL(-1.0, calcStatsOp->getSingleStat("Maximum Relation ID"));

    CPPUNIT_ASSERT_EQUAL(91.0, calcStatsOp->getSingleStat("Total Feature Tags"));
    CPPUNIT_ASSERT_EQUAL(91.0, calcStatsOp->getSingleStat("Total Feature Information Tags"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Total Feature Metadata Tags"));

    CPPUNIT_ASSERT_EQUAL(19.0, calcStatsOp->getSingleStat("Unique Names"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      1267.47, calcStatsOp->getSingleStat("Meters of Linear Features"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      25331.21, calcStatsOp->getSingleStat("Meters Squared of Area Features"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      1267.47, calcStatsOp->getSingleStat("Meters of Highway"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Highway Unique Name Count"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      25331.2, calcStatsOp->getSingleStat("Meters Squared of Buildings"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(13.0, calcStatsOp->getSingleStat("Building Unique Name Count"));

    CPPUNIT_ASSERT_EQUAL(31.0, calcStatsOp->getSingleStat("Total Feature Count"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Total Conflatable Features"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Untagged Feature Count"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Total Unconflatable Features"));
    CPPUNIT_ASSERT_EQUAL(12.0, calcStatsOp->getSingleStat("Total Conflated Features"));
    CPPUNIT_ASSERT_EQUAL(19.0, calcStatsOp->getSingleStat("Total Unmatched Features"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Total Features Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Total Number of Reviews to be Made"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Total Features Conflatable"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Total Features Unconflatable"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      38.71, calcStatsOp->getSingleStat("Percentage of Total Features Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Total Features Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      61.29, calcStatsOp->getSingleStat("Percentage of Total Features Unmatched"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(5.0, calcStatsOp->getSingleStat("Number of Match Creators"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Features Conflatable by: hoot::BuildingMatchCreator"));

    CPPUNIT_ASSERT_EQUAL(8.0, calcStatsOp->getSingleStat("POI Count"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable POIs"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflated POIs"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("POIs Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Number of POI Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(8.0, calcStatsOp->getSingleStat("Unmatched POIs"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, calcStatsOp->getSingleStat("Percentage of POIs Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, calcStatsOp->getSingleStat("Percentage of POIs Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, calcStatsOp->getSingleStat("Percentage of Unmatched POIs"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(9.0, calcStatsOp->getSingleStat("Highway Count"));
    //this seems a little funky to have no conflatable highways but see conflated highways in the
    //output, but since the conflated highways are already present in this file, its actually valid.
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable Highways"));
    CPPUNIT_ASSERT_EQUAL(7.0, calcStatsOp->getSingleStat("Conflated Highways"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Highways Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Number of Highway Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(2.0, calcStatsOp->getSingleStat("Unmatched Highways"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      1119.92, calcStatsOp->getSingleStat("Meters of Highway Processed by Conflation"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(77.78, calcStatsOp->getSingleStat("Percentage of Highways Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, calcStatsOp->getSingleStat("Percentage of Highways Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(22.22, calcStatsOp->getSingleStat("Percentage of Unmatched Highways"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(11.0, calcStatsOp->getSingleStat("Building Count"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable Buildings"));
    CPPUNIT_ASSERT_EQUAL(4.0, calcStatsOp->getSingleStat("Conflated Buildings"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Buildings Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Number of Building Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(7.0, calcStatsOp->getSingleStat("Unmatched Buildings"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      14100.9, calcStatsOp->getSingleStat("Meters Squared of Buildings Processed by Conflation"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(36.36, calcStatsOp->getSingleStat("Percentage of Buildings Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, calcStatsOp->getSingleStat("Percentage of Buildings Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(63.63, calcStatsOp->getSingleStat("Percentage of Unmatched Buildings"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Waterway Count"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable Waterways"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflated Waterways"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Waterways Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Number of Waterway Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Unmatched Waterways"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Meters of Waterway Processed by Conflation"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, calcStatsOp->getSingleStat("Percentage of Waterways Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, calcStatsOp->getSingleStat("Percentage of Waterways Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, calcStatsOp->getSingleStat("Percentage of Unmatched Waterways"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(52.0, calcStatsOp->getSingleStat("Longest Tag"));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.6058823, calcStatsOp->getSingleStat("Translated Populated Tag Percent"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(103.0, calcStatsOp->getSingleStat("Translated Populated Tag Count"));
    CPPUNIT_ASSERT_EQUAL(67.0, calcStatsOp->getSingleStat("Translated Default Tag Count"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Translated Null Tag Count"));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.5138888, calcStatsOp->getSingleStat("Building Translated Populated Tag Percent"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.6481481, calcStatsOp->getSingleStat("Highway Translated Populated Tag Percent"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.704545, calcStatsOp->getSingleStat("POI Translated Populated Tag Percent"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(
      isnan(calcStatsOp->getSingleStat("Waterway Translated Populated Tag Percent")), 1);
  }

  void runStatsTestWithReviews()
  {
    shared_ptr<CalculateStatsOp> calcStatsOp =
      _calcStats("test-files/ops/CalculateStatsOp/all-data-types-with-reviews.osm");

    CPPUNIT_ASSERT_EQUAL(201.0, calcStatsOp->getSingleStat("Node Count"));
    CPPUNIT_ASSERT_EQUAL(21.0, calcStatsOp->getSingleStat("Way Count"));
    CPPUNIT_ASSERT_EQUAL(2.0, calcStatsOp->getSingleStat("Relation Count"));

    CPPUNIT_ASSERT_EQUAL(-3961929.0, calcStatsOp->getSingleStat("Minimum Node ID"));
    CPPUNIT_ASSERT_EQUAL(-3961529.0, calcStatsOp->getSingleStat("Maximum Node ID"));
    CPPUNIT_ASSERT_EQUAL(-3961971.0, calcStatsOp->getSingleStat("Minimum Way ID"));
    CPPUNIT_ASSERT_EQUAL(-3961931.0, calcStatsOp->getSingleStat("Maximum Way ID"));
    CPPUNIT_ASSERT_EQUAL(-3961988.0, calcStatsOp->getSingleStat("Minimum Relation ID"));
    CPPUNIT_ASSERT_EQUAL(-3961981.0,
      calcStatsOp->getSingleStat("Maximum Relation ID"));

    CPPUNIT_ASSERT_EQUAL(89.0, calcStatsOp->getSingleStat("Total Feature Tags"));
    CPPUNIT_ASSERT_EQUAL(87.0, calcStatsOp->getSingleStat("Total Feature Information Tags"));
    CPPUNIT_ASSERT_EQUAL(2.0, calcStatsOp->getSingleStat("Total Feature Metadata Tags"));

    CPPUNIT_ASSERT_EQUAL(19.0, calcStatsOp->getSingleStat("Unique Names"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      1267.47, calcStatsOp->getSingleStat("Meters of Linear Features"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      25331.2, calcStatsOp->getSingleStat("Meters Squared of Area Features"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1267.47, calcStatsOp->getSingleStat("Meters of Highway"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Highway Unique Name Count"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      25331.2, calcStatsOp->getSingleStat("Meters Squared of Buildings"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(13.0, calcStatsOp->getSingleStat("Building Unique Name Count"));

    CPPUNIT_ASSERT_EQUAL(29.0, calcStatsOp->getSingleStat("Total Feature Count"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Total Conflatable Features"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Untagged Feature Count"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Total Unconflatable Features"));
    CPPUNIT_ASSERT_EQUAL(4.0, calcStatsOp->getSingleStat("Total Features Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(11.0, calcStatsOp->getSingleStat("Total Conflated Features"));
    CPPUNIT_ASSERT_EQUAL(18.0, calcStatsOp->getSingleStat("Total Unmatched Features"));
    CPPUNIT_ASSERT_EQUAL(2.0, calcStatsOp->getSingleStat("Total Number of Reviews to be Made"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Total Features Conflatable"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Total Features Unconflatable"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      37.93, calcStatsOp->getSingleStat("Percentage of Total Features Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      13.8, calcStatsOp->getSingleStat("Percentage of Total Features Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      62.07, calcStatsOp->getSingleStat("Percentage of Total Features Unmatched"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(5.0, calcStatsOp->getSingleStat("Number of Match Creators"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Features Conflatable by: hoot::BuildingMatchCreator"));

    CPPUNIT_ASSERT_EQUAL(8.0, calcStatsOp->getSingleStat("POI Count"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable POIs"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflated POIs"));
    CPPUNIT_ASSERT_EQUAL(2.0, calcStatsOp->getSingleStat("POIs Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(1.0, calcStatsOp->getSingleStat("Number of POI Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(8.0, calcStatsOp->getSingleStat("Unmatched POIs"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, calcStatsOp->getSingleStat("Percentage of POIs Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(25.0, calcStatsOp->getSingleStat("Percentage of POIs Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, calcStatsOp->getSingleStat("Percentage of Unmatched POIs"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(9.0, calcStatsOp->getSingleStat("Highway Count"));
    //see comment in corresponding section of code in runStatsTest
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable Highways"));
    CPPUNIT_ASSERT_EQUAL(7.0, calcStatsOp->getSingleStat("Conflated Highways"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Highways Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Number of Highway Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(2.0, calcStatsOp->getSingleStat("Unmatched Highways"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      1119.92, calcStatsOp->getSingleStat("Meters of Highway Processed by Conflation"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(77.78, calcStatsOp->getSingleStat("Percentage of Highways Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, calcStatsOp->getSingleStat("Percentage of Highways Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(22.22, calcStatsOp->getSingleStat("Percentage of Unmatched Highways"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(12.0, calcStatsOp->getSingleStat("Building Count"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable Buildings"));
    CPPUNIT_ASSERT_EQUAL(4.0, calcStatsOp->getSingleStat("Conflated Buildings"));
    CPPUNIT_ASSERT_EQUAL(2.0, calcStatsOp->getSingleStat("Buildings Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(1.0, calcStatsOp->getSingleStat("Number of Building Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(8.0, calcStatsOp->getSingleStat("Unmatched Buildings"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      13693.3, calcStatsOp->getSingleStat("Meters Squared of Buildings Processed by Conflation"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(33.33, calcStatsOp->getSingleStat("Percentage of Buildings Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(16.67, calcStatsOp->getSingleStat("Percentage of Buildings Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(66.67, calcStatsOp->getSingleStat("Percentage of Unmatched Buildings"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Waterway Count"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable Waterways"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflated Waterways"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Waterways Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Number of Waterway Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Unmatched Waterways"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Meters of Waterway Processed by Conflation"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, calcStatsOp->getSingleStat("Percentage of Waterways Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, calcStatsOp->getSingleStat("Percentage of Waterways Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, calcStatsOp->getSingleStat("Percentage of Unmatched Waterways"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(52.0, calcStatsOp->getSingleStat("Longest Tag"));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.6058823, calcStatsOp->getSingleStat("Translated Populated Tag Percent"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(103.0, calcStatsOp->getSingleStat("Translated Populated Tag Count"));
    CPPUNIT_ASSERT_EQUAL(67.0, calcStatsOp->getSingleStat("Translated Default Tag Count"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Translated Null Tag Count"));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.5138888, calcStatsOp->getSingleStat("Building Translated Populated Tag Percent"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.6481481, calcStatsOp->getSingleStat("Highway Translated Populated Tag Percent"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.704545, calcStatsOp->getSingleStat("POI Translated Populated Tag Percent"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(
      isnan(calcStatsOp->getSingleStat("Waterway Translated Populated Tag Percent")), 1);
  }

private:

  shared_ptr<CalculateStatsOp> _calcStats(const QString& inputFile)
  {
    OsmReader reader;
    shared_ptr<OsmMap> map(new OsmMap());
    OsmMap::resetCounters();
    reader.setDefaultStatus(Status::Unknown1);
    reader.setUseStatusFromFile(true);
    reader.setUseDataSourceIds(true);
    reader.read(inputFile, map);

    shared_ptr<CalculateStatsOp> calcStatsOp(new CalculateStatsOp());
    //If we figure out the error messages logged by the script translator related stats are
    //invalid and fix them, then this log disablement can be removed.
    {
      DisableLog dl(Log::Fatal);
      calcStatsOp->apply(map);
    }
    //calcStatsOp->printStats();
    return calcStatsOp;
  }

};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::CalculateStatsOpTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::CalculateStatsOpTest, "current");


