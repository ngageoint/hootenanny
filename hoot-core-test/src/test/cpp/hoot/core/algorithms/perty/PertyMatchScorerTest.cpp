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
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/visitors/TagKeyCountVisitor.h>
#include <hoot/core/algorithms/perty/PertyMatchScorer.h>
#include <hoot/core/scoring/MapMatchScoringUtils.h>

namespace hoot
{

class PertyMatchScorerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(PertyMatchScorerTest);
  CPPUNIT_TEST(runLoadReferenceMapTest);
  CPPUNIT_TEST(runLoadPerturbedMapTest);
  CPPUNIT_TEST(runCombineMapTest);
  CPPUNIT_TEST(runConflateTest);
  CPPUNIT_TEST_SUITE_END();

public:

  PertyMatchScorerTest()
    : HootTestFixture("test-files/algorithms/perty/PertyMatchScorerTest/",
                      "test-output/algorithms/perty/PertyMatchScorerTest/")
  {
    setResetType(ResetAll);
  }

  void runLoadReferenceMapTest()
  {
    PertyMatchScorer matchScorer;
    matchScorer.setSearchDistance(15.0);
    matchScorer._loadReferenceMap(
      _inputPath + "PertyMatchScorerTest-reference-in-1.osm",
      _outputPath + "PertyMatchScorerTest-reference-out-1.osm");
    HOOT_FILE_EQUALS(
      _inputPath + "PertyMatchScorerTest-reference-out-1.osm",
      _outputPath + "PertyMatchScorerTest-reference-out-1.osm");
  }

  void runLoadPerturbedMapTest()
  {
    Settings testSettings = conf();
    testSettings.set("random.seed", QString::number(1));
    testSettings.set("perty.systematic.error.x", QString::number(1));
    testSettings.set("perty.systematic.error.y", QString::number(1));
    PertyMatchScorer matchScorer;
    matchScorer.setConfiguration(testSettings);
    matchScorer.setSearchDistance(15.0);
    matchScorer._loadPerturbedMap(
      _inputPath + "PertyMatchScorerTest-reference-out-1.osm",
      _outputPath + "PertyMatchScorerTest-perturbed-out-1.osm");
    HOOT_FILE_EQUALS(
      _inputPath + "PertyMatchScorerTest-perturbed-out-1.osm",
      _outputPath + "PertyMatchScorerTest-perturbed-out-1.osm");
  }

  void runCombineMapTest()
  {
    OsmXmlReader reader;
    OsmMapPtr referenceMap(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.setUseDataSourceIds(true);
    reader.read(
      _inputPath + "PertyMatchScorerTest-reference-out-1.osm", referenceMap);
    CPPUNIT_ASSERT_EQUAL(44, (int)referenceMap->getElementCount());

    PertyMatchScorer matchScorer;
    matchScorer.setSearchDistance(15.0);
    OsmMapPtr combinedMap =
      matchScorer._combineMapsAndPrepareForConflation(
        referenceMap,
        _inputPath + "PertyMatchScorerTest-perturbed-out-1.osm");

    //can't do a file comparison on the output here since the UUID's added to the file will be
    //different with each run
    CPPUNIT_ASSERT_EQUAL(99, (int)combinedMap->getElementCount());
    std::shared_ptr<TagKeyCountVisitor> tagKeyCountVisitorRef1(new TagKeyCountVisitor(MetadataTags::Ref1()));
    combinedMap->visitRo(*tagKeyCountVisitorRef1);
    CPPUNIT_ASSERT_EQUAL(8, (int)tagKeyCountVisitorRef1->getStat());
    std::shared_ptr<TagKeyCountVisitor> tagKeyCountVisitorRef2(new TagKeyCountVisitor(MetadataTags::Ref2()));
    combinedMap->visitRo(*tagKeyCountVisitorRef2);
    CPPUNIT_ASSERT_EQUAL(10, (int)tagKeyCountVisitorRef2->getStat());
  }

  void runConflateTest()
  {
    Settings testSettings = conf();
    testSettings.set("conflate.enable.old.roads", "false");

    OsmXmlReader reader;
    OsmMapPtr combinedMap(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.setUseDataSourceIds(true);
    reader.read(
      _inputPath + "PertyMatchScorerTest-combined-out-1.osm", combinedMap);

    PertyMatchScorer matchScorer;
    matchScorer.setConfiguration(testSettings);
    matchScorer.setSearchDistance(15.0);
    /*std::shared_ptr<MatchComparator> result =*/
    matchScorer._conflateAndScoreMatches(
      combinedMap,
      _outputPath + "PertyMatchScorerTest-conflated-out-1.osm");
    //const double score = result->getPertyScore();

    //MatchComparator test already covers testing the scoring, so not doing that here.
    HOOT_FILE_EQUALS(
      _inputPath + "PertyMatchScorerTest-conflated-out-1.osm",
      _outputPath + "PertyMatchScorerTest-conflated-out-1.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PertyMatchScorerTest, "slow");

}
