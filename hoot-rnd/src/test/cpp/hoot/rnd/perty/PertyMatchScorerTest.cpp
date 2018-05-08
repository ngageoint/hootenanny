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
 * @copyright Copyright (C) 2014, 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/rnd/perty/PertyMatchScorer.h>
#include <hoot/rnd/scoring/MapMatchScoringUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/visitors/TagKeyCountVisitor.h>

// Qt
#include <QString>
#include <QDir>

#include <hoot/core/TestUtils.h>

namespace hoot
{

class PertyMatchScorerTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PertyMatchScorerTest);
  CPPUNIT_TEST(runLoadReferenceMapTest);
  CPPUNIT_TEST(runLoadPerturbedMapTest);
  CPPUNIT_TEST(runCombineMapTest);
  CPPUNIT_TEST(runConflateTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp()
  {
    TestUtils::mkpath("test-output/perty/PertyMatchScorerTest");
  }

  void runLoadReferenceMapTest()
  {
    TestUtils::resetEnvironment();
    OsmMap::resetCounters();

    PertyMatchScorer matchScorer;
    matchScorer.setSearchDistance(15.0);
    matchScorer.setApplyRubberSheet(true);
    matchScorer._loadReferenceMap(
      "test-files/perty/PertyMatchScorerTest/PertyMatchScorerTest-reference-in-1.osm",
      "test-output/perty/PertyMatchScorerTest/PertyMatchScorerTest-reference-out-1.osm");
    HOOT_FILE_EQUALS(
      "test-files/perty/PertyMatchScorerTest/PertyMatchScorerTest-reference-out-1.osm",
      "test-output/perty/PertyMatchScorerTest/PertyMatchScorerTest-reference-out-1.osm");
  }

  void runLoadPerturbedMapTest()
  {
    TestUtils::resetEnvironment();
    Settings testSettings = conf();
    testSettings.set("perty.seed", QString::number(1));
    testSettings.set("perty.systematic.error.x", QString::number(1));
    testSettings.set("perty.systematic.error.y", QString::number(1));
    PertyMatchScorer matchScorer;
    matchScorer.setConfiguration(testSettings);
    matchScorer.setSearchDistance(15.0);
    matchScorer.setApplyRubberSheet(true);
    OsmMap::resetCounters();
    matchScorer._loadPerturbedMap(
      "test-files/perty/PertyMatchScorerTest/PertyMatchScorerTest-reference-out-1.osm",
      "test-output/perty/PertyMatchScorerTest/PertyMatchScorerTest-perturbed-out-1.osm");
    HOOT_FILE_EQUALS(
      "test-files/perty/PertyMatchScorerTest/PertyMatchScorerTest-perturbed-out-1.osm",
      "test-output/perty/PertyMatchScorerTest/PertyMatchScorerTest-perturbed-out-1.osm");
  }

  void runCombineMapTest()
  {
    TestUtils::resetEnvironment();
    OsmMap::resetCounters();
    OsmXmlReader reader;
    OsmMapPtr referenceMap(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.setUseDataSourceIds(true);
    reader.read(
      "test-files/perty/PertyMatchScorerTest/PertyMatchScorerTest-reference-out-1.osm", referenceMap);
    CPPUNIT_ASSERT_EQUAL(44, (int)referenceMap->getElementCount());

    PertyMatchScorer matchScorer;
    matchScorer.setSearchDistance(15.0);
    matchScorer.setApplyRubberSheet(false);
    OsmMapPtr combinedMap =
      matchScorer._combineMapsAndPrepareForConflation(
        referenceMap,
        "test-files/perty/PertyMatchScorerTest/PertyMatchScorerTest-perturbed-out-1.osm");

    //can't do a file comparison on the output here since the UUID's added to the file will be
    //different with each run
    CPPUNIT_ASSERT_EQUAL(100, (int)combinedMap->getElementCount());
    boost::shared_ptr<TagKeyCountVisitor> tagKeyCountVisitorRef1(new TagKeyCountVisitor(MetadataTags::Ref1()));
    combinedMap->visitRw(*tagKeyCountVisitorRef1);
    CPPUNIT_ASSERT_EQUAL(8, (int)tagKeyCountVisitorRef1->getStat());
    boost::shared_ptr<TagKeyCountVisitor> tagKeyCountVisitorRef2(new TagKeyCountVisitor(MetadataTags::Ref2()));
    combinedMap->visitRw(*tagKeyCountVisitorRef2);
    CPPUNIT_ASSERT_EQUAL(10, (int)tagKeyCountVisitorRef2->getStat());
  }

  void runConflateTest()
  {
    TestUtils::resetEnvironment();
    Settings testSettings = conf();
    testSettings.set("conflate.enable.old.roads", "false");

    OsmMap::resetCounters();
    OsmXmlReader reader;
    OsmMapPtr combinedMap(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.setUseDataSourceIds(true);
    reader.read(
      "test-files/perty/PertyMatchScorerTest/PertyMatchScorerTest-combined-out-1.osm", combinedMap);

    PertyMatchScorer matchScorer;
    matchScorer.setConfiguration(testSettings);
    matchScorer.setSearchDistance(15.0);
    matchScorer.setApplyRubberSheet(true);
    //matchScorer.setConfiguration(testSettings);
    /*boost::shared_ptr<MatchComparator> result =*/
    matchScorer._conflateAndScoreMatches(
      combinedMap,
      "test-output/perty/PertyMatchScorerTest/PertyMatchScorerTest-conflated-out-1.osm");
    //const double score = result->getPertyScore();

    //MatchComparator test already covers testing the scoring, so not doing that here.
    HOOT_FILE_EQUALS(
      "test-files/perty/PertyMatchScorerTest/PertyMatchScorerTest-conflated-out-1.osm",
      "test-output/perty/PertyMatchScorerTest/PertyMatchScorerTest-conflated-out-1.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PertyMatchScorerTest, "slow");

}
