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
 * @copyright Copyright (C) 2014, 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include "../TestUtils.h"
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/ops/CalculateStatsOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>

// CPP Unit
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <qnumeric.h>

namespace hoot
{

class CalculateStatsOpTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(CalculateStatsOpTest);
  CPPUNIT_TEST(runStatsNumTest);
  CPPUNIT_TEST(runStatsTest);
  CPPUNIT_TEST(runStatsTestWithReviews);
  CPPUNIT_TEST_SUITE_END();

public:


  CalculateStatsOpTest()
    : HootTestFixture("test-files/ops/CalculateStatsOp/",
                      UNUSED_PATH)
  {
    setResetType(ResetAll);
  }

  virtual void setUp()
  {
    HootTestFixture::setUp();
    conf().set(ConfigOptions::getStatsTranslateScriptKey(), "${HOOT_HOME}/translations/HootTest.js");
  }

  virtual void takeDown()
  {
    conf().set(
      ConfigOptions::getStatsTranslateScriptKey(),
      ConfigOptions::getStatsTranslateScriptDefaultValue());
  }

  // This is here to prevent inadvertant removal of stats and to make sure any new stats get added
  // to this test.
  void runStatsNumTest()
  {
    std::shared_ptr<CalculateStatsOp> calcStatsOp = _calcStats(_inputPath + "all-data-types.osm");
    CPPUNIT_ASSERT_EQUAL(194, calcStatsOp->getStats().size());
  }

  void runStatsTest()
  {
    std::shared_ptr<CalculateStatsOp> calcStatsOp = _calcStats(_inputPath + "all-data-types.osm");

    CPPUNIT_ASSERT_EQUAL(201.0, calcStatsOp->getSingleStat("Nodes"));
    CPPUNIT_ASSERT_EQUAL(22.0, calcStatsOp->getSingleStat("Ways"));
    CPPUNIT_ASSERT_EQUAL(1.0, calcStatsOp->getSingleStat("Relations"));

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
      1267.47, calcStatsOp->getSingleStat("Meters of Roads"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Unique Road Names"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      25331.2, calcStatsOp->getSingleStat("Meters Squared of Buildings"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(13.0, calcStatsOp->getSingleStat("Unique Building Names"));

    CPPUNIT_ASSERT_EQUAL(31.0, calcStatsOp->getSingleStat("Total Features"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Total Conflatable Features"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Untagged Features"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Total Unconflatable Features"));
    CPPUNIT_ASSERT_EQUAL(12.0, calcStatsOp->getSingleStat("Total Conflated Features"));
    CPPUNIT_ASSERT_EQUAL(19.0, calcStatsOp->getSingleStat("Total Unmatched Features"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Total Features Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Total Reviews to be Made"));
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

    CPPUNIT_ASSERT_EQUAL(12.0, calcStatsOp->getSingleStat("Match Creators"));
    //we're parsing a conflated output map here, so all conflatable counts are equal to zero;
    //counts for PoiPolygon are broken up by type in addition to being shown as total
    CPPUNIT_ASSERT_EQUAL(
      0.0, calcStatsOp->getSingleStat("Features Conflatable by: hoot::BuildingMatchCreator"));
    CPPUNIT_ASSERT_EQUAL(
      0.0, calcStatsOp->getSingleStat("Features Conflatable by: hoot::HighwayMatchCreator"));
    CPPUNIT_ASSERT_EQUAL(
      0.0, calcStatsOp->getSingleStat("Polygons Conflatable by: hoot::PoiPolygonMatchCreator"));
    CPPUNIT_ASSERT_EQUAL(
      0.0, calcStatsOp->getSingleStat("POIs Conflatable by: hoot::PoiPolygonMatchCreator"));
    CPPUNIT_ASSERT_EQUAL(
      0.0, calcStatsOp->getSingleStat("Features Conflatable by: hoot::PoiPolygonMatchCreator"));
    CPPUNIT_ASSERT_EQUAL(
      0.0,
      calcStatsOp->getSingleStat(
        "Features Conflatable by: hoot::ScriptMatchCreator,Poi.js"));

    CPPUNIT_ASSERT_EQUAL(8.0, calcStatsOp->getSingleStat("POIs"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable POIs"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflated POIs"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("POIs Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("POI Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(8.0, calcStatsOp->getSingleStat("Unmatched POIs"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of POIs Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of POIs Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      100.0, calcStatsOp->getSingleStat("Percentage of Unmatched POIs"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(9.0, calcStatsOp->getSingleStat("Roads"));
    //this seems a little funky to have no conflatable highways but see conflated highways in the
    //output, but since the conflated highways are already present in this file, its actually valid.
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable Roads"));
    CPPUNIT_ASSERT_EQUAL(7.0, calcStatsOp->getSingleStat("Conflated Roads"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Roads Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Road Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(2.0, calcStatsOp->getSingleStat("Unmatched Roads"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      1119.92, calcStatsOp->getSingleStat("Meters of Road Processed by Conflation"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      77.78, calcStatsOp->getSingleStat("Percentage of Roads Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Roads Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      22.22, calcStatsOp->getSingleStat("Percentage of Unmatched Roads"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(11.0, calcStatsOp->getSingleStat("Buildings"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable Buildings"));
    CPPUNIT_ASSERT_EQUAL(4.0, calcStatsOp->getSingleStat("Conflated Buildings"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Buildings Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Building Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(7.0, calcStatsOp->getSingleStat("Unmatched Buildings"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      14100.9,
      calcStatsOp->getSingleStat("Meters Squared of Buildings Processed by Conflation"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      36.36, calcStatsOp->getSingleStat("Percentage of Buildings Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Buildings Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      63.63, calcStatsOp->getSingleStat("Percentage of Unmatched Buildings"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Waterways"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable Waterways"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflated Waterways"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Waterways Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Waterway Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Unmatched Waterways"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Meters of Waterway Processed by Conflation"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Waterways Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Waterways Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Unmatched Waterways"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Power Lines"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable Power Lines"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflated Power Lines"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Power Lines Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Power Line Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Unmatched Power Lines"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Meters of Power Line Processed by Conflation"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Power Lines Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Power Lines Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Unmatched Power Lines"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(6.0, calcStatsOp->getSingleStat("Polygon Conflatable POIs"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable Polygon Conflatable POIs"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflated Polygon Conflatable POIs"));
    CPPUNIT_ASSERT_EQUAL(
      0.0, calcStatsOp->getSingleStat("Polygon Conflatable POIs Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(
      0.0, calcStatsOp->getSingleStat("Polygon Conflatable POI Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(6.0, calcStatsOp->getSingleStat("Unmatched Polygon Conflatable POIs"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Polygon Conflatable POIs Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0,
      calcStatsOp->getSingleStat("Percentage of Polygon Conflatable POIs Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      100.0, calcStatsOp->getSingleStat("Percentage of Unmatched Polygon Conflatable POIs"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(12.0, calcStatsOp->getSingleStat("Polygons"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable Polygons"));
    CPPUNIT_ASSERT_EQUAL(5.0, calcStatsOp->getSingleStat("Conflated Polygons"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Polygons Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Polygon Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(7.0, calcStatsOp->getSingleStat("Unmatched Polygons"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      14810.8,
      calcStatsOp->getSingleStat("Meters Squared of Polygons Processed by Conflation"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      41.67, calcStatsOp->getSingleStat("Percentage of Polygons Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Polygons Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      58.33, calcStatsOp->getSingleStat("Percentage of Unmatched Polygons"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(2.0, calcStatsOp->getSingleStat("Areas"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable Areas"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflated Areas"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Areas Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Area Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(2.0, calcStatsOp->getSingleStat("Unmatched Areas"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Meters Squared of Areas Processed by Conflation"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Areas Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Areas Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      100.0, calcStatsOp->getSingleStat("Percentage of Unmatched Areas"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(52.0, calcStatsOp->getSingleStat("Longest Tag"));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.6058823, calcStatsOp->getSingleStat("Translated Populated Tag Percent"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(103.0, calcStatsOp->getSingleStat("Translated Populated Tags"));
    CPPUNIT_ASSERT_EQUAL(67.0, calcStatsOp->getSingleStat("Translated Default Tags"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Translated Null Tags"));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.5138888, calcStatsOp->getSingleStat("Building Translated Populated Tag Percent"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.6481481, calcStatsOp->getSingleStat("Road Translated Populated Tag Percent"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.704545, calcStatsOp->getSingleStat("POI Translated Populated Tag Percent"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(
      int(::qIsNaN(calcStatsOp->getSingleStat("Waterway Translated Populated Tag Percent"))), 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.704545,
      calcStatsOp->getSingleStat("Polygon Conflatable POI Translated Populated Tag Percent"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.513888, calcStatsOp->getSingleStat("Polygon Translated Populated Tag Percent"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(3.0, calcStatsOp->getSingleStat("Least Nodes in a Way"));
    CPPUNIT_ASSERT_EQUAL(59.0, calcStatsOp->getSingleStat("Most Nodes in a Way"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0, calcStatsOp->getSingleStat("Average Nodes Per Way"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(226.0, calcStatsOp->getSingleStat("Total Way Nodes"));

    CPPUNIT_ASSERT_EQUAL(3.0, calcStatsOp->getSingleStat("Least Members in a Relation"));
    CPPUNIT_ASSERT_EQUAL(3.0, calcStatsOp->getSingleStat("Most Members in a Relation"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      3.0, calcStatsOp->getSingleStat("Average Members Per Relation"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(3.0, calcStatsOp->getSingleStat("Total Relation Members"));

    CPPUNIT_ASSERT_EQUAL(1.0, calcStatsOp->getSingleStat("Least Tags on a Feature"));
    CPPUNIT_ASSERT_EQUAL(6.0, calcStatsOp->getSingleStat("Most Tags on a Feature"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, calcStatsOp->getSingleStat("Average Tags Per Feature"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(1.0, calcStatsOp->getSingleStat("Least Information Tags on a Feature"));
    CPPUNIT_ASSERT_EQUAL(6.0, calcStatsOp->getSingleStat("Most Information Tags on a Feature"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      2.94, calcStatsOp->getSingleStat("Average Information Tags Per Feature"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(19.0, calcStatsOp->getSingleStat("Features with Names"));

    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Tunnels"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("One-Way Streets"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Road Roundabouts"));

    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Multi-Use Buildings"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Buildings With Height Info"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Shortest Building Height"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Tallest Building Height"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Average Height Per Building"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Buildings With Level Info"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Least Levels in a Building"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Most Levels in a Building"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Average Levels Per Building"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(2.0, calcStatsOp->getSingleStat("Non-Building Areas"));

    CPPUNIT_ASSERT_EQUAL(1.0, calcStatsOp->getSingleStat("Features with Addresses"));
    CPPUNIT_ASSERT_EQUAL(1.0, calcStatsOp->getSingleStat("Total Addresses"));

    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Features with Phone Numbers"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Total Phone Numbers"));
  }

  void runStatsTestWithReviews()
  {
    std::shared_ptr<CalculateStatsOp> calcStatsOp = _calcStats(_inputPath + "all-data-types-with-reviews.osm");

    CPPUNIT_ASSERT_EQUAL(201.0, calcStatsOp->getSingleStat("Nodes"));
    CPPUNIT_ASSERT_EQUAL(21.0, calcStatsOp->getSingleStat("Ways"));
    CPPUNIT_ASSERT_EQUAL(2.0, calcStatsOp->getSingleStat("Relations"));

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
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1267.47, calcStatsOp->getSingleStat("Meters of Roads"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Unique Road Names"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      25331.2, calcStatsOp->getSingleStat("Meters Squared of Buildings"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(13.0, calcStatsOp->getSingleStat("Unique Building Names"));

    CPPUNIT_ASSERT_EQUAL(29.0, calcStatsOp->getSingleStat("Total Features"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Total Conflatable Features"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Untagged Features"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Total Unconflatable Features"));
    CPPUNIT_ASSERT_EQUAL(4.0, calcStatsOp->getSingleStat("Total Features Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(11.0, calcStatsOp->getSingleStat("Total Conflated Features"));
    CPPUNIT_ASSERT_EQUAL(18.0, calcStatsOp->getSingleStat("Total Unmatched Features"));
    CPPUNIT_ASSERT_EQUAL(2.0, calcStatsOp->getSingleStat("Total Reviews to be Made"));
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

    CPPUNIT_ASSERT_EQUAL(12.0, calcStatsOp->getSingleStat("Match Creators"));
    //we're parsing a conflated output map here, so all conflatable counts are equal to zero;
    //counts for PoiPolygon are broken up by type in addition to being shown as total
    CPPUNIT_ASSERT_EQUAL(
      0.0, calcStatsOp->getSingleStat("Features Conflatable by: hoot::BuildingMatchCreator"));
    CPPUNIT_ASSERT_EQUAL(
      0.0, calcStatsOp->getSingleStat("Features Conflatable by: hoot::HighwayMatchCreator"));
    CPPUNIT_ASSERT_EQUAL(
      0.0, calcStatsOp->getSingleStat("Polygons Conflatable by: hoot::PoiPolygonMatchCreator"));
    CPPUNIT_ASSERT_EQUAL(
      0.0, calcStatsOp->getSingleStat("POIs Conflatable by: hoot::PoiPolygonMatchCreator"));
    CPPUNIT_ASSERT_EQUAL(
      0.0, calcStatsOp->getSingleStat("Features Conflatable by: hoot::PoiPolygonMatchCreator"));
    CPPUNIT_ASSERT_EQUAL(
      0.0,
      calcStatsOp->getSingleStat(
        "Features Conflatable by: hoot::ScriptMatchCreator,Poi.js"));

    CPPUNIT_ASSERT_EQUAL(8.0, calcStatsOp->getSingleStat("POIs"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable POIs"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflated POIs"));
    CPPUNIT_ASSERT_EQUAL(2.0, calcStatsOp->getSingleStat("POIs Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(1.0, calcStatsOp->getSingleStat("POI Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(8.0, calcStatsOp->getSingleStat("Unmatched POIs"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of POIs Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      25.0, calcStatsOp->getSingleStat("Percentage of POIs Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      100.0, calcStatsOp->getSingleStat("Percentage of Unmatched POIs"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(9.0, calcStatsOp->getSingleStat("Roads"));
    //see comment in corresponding section of code in runStatsTest
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable Roads"));
    CPPUNIT_ASSERT_EQUAL(7.0, calcStatsOp->getSingleStat("Conflated Roads"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Roads Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Road Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(2.0, calcStatsOp->getSingleStat("Unmatched Roads"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      1119.92, calcStatsOp->getSingleStat("Meters of Road Processed by Conflation"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      77.78, calcStatsOp->getSingleStat("Percentage of Roads Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Roads Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      22.22, calcStatsOp->getSingleStat("Percentage of Unmatched Roads"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(12.0, calcStatsOp->getSingleStat("Buildings"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable Buildings"));
    CPPUNIT_ASSERT_EQUAL(4.0, calcStatsOp->getSingleStat("Conflated Buildings"));
    CPPUNIT_ASSERT_EQUAL(2.0, calcStatsOp->getSingleStat("Buildings Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(1.0, calcStatsOp->getSingleStat("Building Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(8.0, calcStatsOp->getSingleStat("Unmatched Buildings"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      13693.3,
      calcStatsOp->getSingleStat("Meters Squared of Buildings Processed by Conflation"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      33.33, calcStatsOp->getSingleStat("Percentage of Buildings Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      16.67, calcStatsOp->getSingleStat("Percentage of Buildings Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      66.67, calcStatsOp->getSingleStat("Percentage of Unmatched Buildings"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Waterways"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable Waterways"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflated Waterways"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Waterways Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Waterway Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Unmatched Waterways"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Meters of Waterway Processed by Conflation"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Waterways Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Waterways Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Unmatched Waterways"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Power Lines"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable Power Lines"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflated Power Lines"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Power Lines Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(
      0.0, calcStatsOp->getSingleStat("Power Line Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Unmatched Power Lines"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Meters of Power Line Processed by Conflation"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Power Lines Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Power Lines Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Unmatched Power Lines"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(6.0, calcStatsOp->getSingleStat("Polygon Conflatable POIs"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable Polygon Conflatable POIs"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflated Polygon Conflatable POIs"));
    CPPUNIT_ASSERT_EQUAL(
      1.0, calcStatsOp->getSingleStat("Polygon Conflatable POIs Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(
      1.0, calcStatsOp->getSingleStat("Polygon Conflatable POI Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(6.0, calcStatsOp->getSingleStat("Unmatched Polygon Conflatable POIs"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Percentage of Polygon Conflatable POIs Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      16.67,
      calcStatsOp->getSingleStat("Percentage of Polygon Conflatable POIs Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      100.0, calcStatsOp->getSingleStat("Percentage of Unmatched Polygon Conflatable POIs"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(12.0, calcStatsOp->getSingleStat("Polygons"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Conflatable Polygons"));
    CPPUNIT_ASSERT_EQUAL(4.0, calcStatsOp->getSingleStat("Conflated Polygons"));
    CPPUNIT_ASSERT_EQUAL(2.0, calcStatsOp->getSingleStat("Polygons Marked for Review"));
    CPPUNIT_ASSERT_EQUAL(1.0, calcStatsOp->getSingleStat("Polygon Reviews to be Made"));
    CPPUNIT_ASSERT_EQUAL(8.0, calcStatsOp->getSingleStat("Unmatched Polygons"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      13693.36,
      calcStatsOp->getSingleStat("Meters Squared of Polygons Processed by Conflation"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      33.33, calcStatsOp->getSingleStat("Percentage of Polygons Conflated"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      16.7, calcStatsOp->getSingleStat("Percentage of Polygons Marked for Review"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      66.7, calcStatsOp->getSingleStat("Percentage of Unmatched Polygons"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(52.0, calcStatsOp->getSingleStat("Longest Tag"));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.6058823, calcStatsOp->getSingleStat("Translated Populated Tag Percent"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(103.0, calcStatsOp->getSingleStat("Translated Populated Tags"));
    CPPUNIT_ASSERT_EQUAL(67.0, calcStatsOp->getSingleStat("Translated Default Tags"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Translated Null Tags"));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.5138888, calcStatsOp->getSingleStat("Building Translated Populated Tag Percent"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.6481481, calcStatsOp->getSingleStat("Road Translated Populated Tag Percent"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.704545, calcStatsOp->getSingleStat("POI Translated Populated Tag Percent"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(
      int(::qIsNaN(calcStatsOp->getSingleStat("Waterway Translated Populated Tag Percent"))), 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.704545,
      calcStatsOp->getSingleStat("Polygon Conflatable POI Translated Populated Tag Percent"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.513888, calcStatsOp->getSingleStat("Polygon Translated Populated Tag Percent"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(3.0, calcStatsOp->getSingleStat("Least Nodes in a Way"));
    CPPUNIT_ASSERT_EQUAL(59.0, calcStatsOp->getSingleStat("Most Nodes in a Way"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.0, calcStatsOp->getSingleStat("Average Nodes Per Way"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(209.0, calcStatsOp->getSingleStat("Total Way Nodes"));

    CPPUNIT_ASSERT_EQUAL(2.0, calcStatsOp->getSingleStat("Least Members in a Relation"));
    CPPUNIT_ASSERT_EQUAL(2.0, calcStatsOp->getSingleStat("Most Members in a Relation"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      2.0, calcStatsOp->getSingleStat("Average Members Per Relation"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(4.0, calcStatsOp->getSingleStat("Total Relation Members"));

    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Least Tags on a Feature"));
    CPPUNIT_ASSERT_EQUAL(6.0, calcStatsOp->getSingleStat("Most Tags on a Feature"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, calcStatsOp->getSingleStat("Average Tags Per Feature"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Least Information Tags on a Feature"));
    CPPUNIT_ASSERT_EQUAL(6.0, calcStatsOp->getSingleStat("Most Information Tags on a Feature"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      2.94, calcStatsOp->getSingleStat("Average Information Tags Per Feature"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(19.0, calcStatsOp->getSingleStat("Features with Names"));

    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Tunnels"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("One-Way Streets"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Road Roundabouts"));

    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Multi-Use Buildings"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Buildings With Height Info"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Shortest Building Height"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Tallest Building Height"), 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Average Height Per Building"), 1e-1);
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Buildings With Level Info"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Least Levels in a Building"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Most Levels in a Building"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, calcStatsOp->getSingleStat("Average Levels Per Building"), 1e-1);

    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Non-Building Areas"));

    CPPUNIT_ASSERT_EQUAL(1.0, calcStatsOp->getSingleStat("Features with Addresses"));
    CPPUNIT_ASSERT_EQUAL(1.0, calcStatsOp->getSingleStat("Total Addresses"));

    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Features with Phone Numbers"));
    CPPUNIT_ASSERT_EQUAL(0.0, calcStatsOp->getSingleStat("Total Phone Numbers"));
  }

private:

  std::shared_ptr<CalculateStatsOp> _calcStats(const QString& inputFile)
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.setUseFileStatus(true);
    reader.setUseDataSourceIds(true);
    reader.read(inputFile, map);

    std::shared_ptr<CalculateStatsOp> calcStatsOp(new CalculateStatsOp());
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

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::CalculateStatsOpTest, "slow");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::CalculateStatsOpTest, "current");

}
