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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */


// Hoot
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/rnd/scoring/multiary/MultiaryMatchComparator.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/TestUtils.h>

using namespace hoot;

namespace hoot
{

class MultiaryMatchComparatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MultiaryMatchComparatorTest);
  CPPUNIT_TEST(runEmptyTest);
  CPPUNIT_TEST(runOneMatchTest);
  CPPUNIT_TEST(runMatchTest1);
  CPPUNIT_TEST(runMatchTest2);
  CPPUNIT_TEST(runMatchTest3);
  CPPUNIT_TEST(runReviewTest1);
  CPPUNIT_TEST(runReviewTest2);
  CPPUNIT_TEST(runReviewTest3);
  CPPUNIT_TEST(runReviewTest4);
  CPPUNIT_TEST(runMatchMissTest1);
  CPPUNIT_TEST_SUITE_END();

public:

  /**
   * Run an empty test to verify that we get reasonable results with the simplest case.
   */
  void runEmptyTest()
  {
    QString inputJson =
      "{'version': 0.6,'generator': 'Overpass API','elements':[\n"
      "{'type':'node','id': -1,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'Input001', 'ID':'1-000000', 'MATCH':'none', 'poi':'yes'}},\n"
      "{'type':'node','id': -2,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'Input002', 'ID':'2-000000', 'MATCH':'none', 'poi':'yes'}}\n"
      "]}\n";

    OsmMapPtr input = std::make_shared<OsmMap>();
    OsmJsonReader().loadFromString(inputJson, input);
    OsmMapPtr conflated = std::make_shared<OsmMap>();
    OsmJsonReader().loadFromString(inputJson, conflated);

    MultiaryMatchComparator uut;
    double correct = uut.evaluateMatches(input, conflated);
    HOOT_STR_EQUALS(true, isnan(correct));
    HOOT_STR_EQUALS(0, uut.getTp());
    HOOT_STR_EQUALS(0, uut.getFp());
    HOOT_STR_EQUALS(0, uut.getFn());
    // this doesn't include True Negatives so the count is zero.
    HOOT_STR_EQUALS(0, uut.getTotalCount());
  }

  /**
   * Run a simple match test.
   */
  void runOneMatchTest()
  {
    QString inputJson =
      "{'version': 0.6,'generator': 'Overpass API','elements':[\n"
      "{'type':'node','id': -1,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000000', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type':'node','id': -2,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000001', 'MATCH':'1-000000', 'poi':'yes'}}\n"
      "]}\n";

    QString conflatedJson =
      "{'version': 0.6,'generator': 'Overpass API','elements':[\n"
      "{'type':'node','id': -1,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'Conflated', 'ID':'1-000000;1-000001', 'MATCH':'1-000000', 'poi':'yes'}}\n"
      "]}\n";

    OsmJsonReader reader;
    reader.setDefaultStatus(Status::Unknown1);

    OsmMapPtr input = std::make_shared<OsmMap>();
    reader.loadFromString(inputJson, input);
    OsmMapPtr conflated = std::make_shared<OsmMap>();
    reader.loadFromString(conflatedJson, conflated);

    MultiaryMatchComparator uut;
    double correct = uut.evaluateMatches(input, conflated);
    LOG_VAR(uut.toString());
    HOOT_STR_EQUALS(1, correct);
    HOOT_STR_EQUALS(1, uut.getTp());
    HOOT_STR_EQUALS(0, uut.getFp());
    HOOT_STR_EQUALS(0, uut.getFn());
    // this doesn't include True Negatives
    HOOT_STR_EQUALS(1, uut.getTotalCount());
  }

  /**
   * Expected 1 cluster of 3
   * Actual, 1 cluster of 2 and a cluster of 1.
   */
  void runMatchTest1()
  {
    QString inputJson =
      "{'version': 0.6,'generator': 'Overpass API','elements':[\n"
      "{'type':'node','id': -1,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000000', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type':'node','id': -2,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000001', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type':'node','id': -3,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'2', 'ID':'2-000000', 'MATCH':'1-000000', 'poi':'yes'}}\n"
      "]}\n";

    QString conflatedJson =
      "{'version': 0.6,'generator': 'Overpass API','elements':[\n"
      "{'type':'node','id': -1,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'Conflated', 'ID':'1-000000;1-000001', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type':'node','id': -3,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'2', 'ID':'2-000000', 'MATCH':'1-000000', 'poi':'yes'}}\n"
      "]}\n";

    OsmJsonReader reader;
    reader.setDefaultStatus(Status::Unknown1);

    OsmMapPtr input = std::make_shared<OsmMap>();
    reader.loadFromString(inputJson, input);
    input->getNode(-3)->setStatus(Status::Unknown2);

    OsmMapPtr conflated = std::make_shared<OsmMap>();
    reader.loadFromString(conflatedJson, conflated);
    conflated->getNode(-3)->setStatus(Status::Unknown2);

    MultiaryMatchComparator uut;
    double correct = uut.evaluateMatches(input, conflated);
    LOG_VAR(uut.toString());
    HOOT_STR_EQUALS(0.333333, correct);
    HOOT_STR_EQUALS(1, uut.getTp());
    HOOT_STR_EQUALS(0, uut.getFp());
    HOOT_STR_EQUALS(2, uut.getFn());
    // this doesn't include True Negatives
    HOOT_STR_EQUALS(3, uut.getTotalCount());
  }


  /**
   * Expected 2 clusters. size 4 and 3.
   * Actual, 2 clusters. size 4 and 3, but 1 node swapped between them.
   */
  void runMatchTest2()
  {
    QString inputJson =
      "{'version': 0.6,'generator': 'Overpass API','elements':[\n"
      "{'type':'node','id': -1,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000000', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type':'node','id': -2,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000001', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type':'node','id': -3,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'2', 'ID':'2-000000', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type':'node','id': -4,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000002', 'MATCH':'1-000002', 'poi':'yes'}},\n"
      "{'type':'node','id': -5,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000003', 'MATCH':'1-000002', 'poi':'yes'}},\n"
      "{'type':'node','id': -6,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'2', 'ID':'2-000001', 'MATCH':'1-000002', 'poi':'yes'}},\n"
      "{'type':'node','id': -7,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'2', 'ID':'2-000002', 'MATCH':'1-000002', 'poi':'yes'}}\n"
      "]}\n";

    QString conflatedJson =
      "{'version': 0.6,'generator': 'Overpass API','elements':[\n"
      "{'type':'node','id': -1,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'Conflated', 'ID':'1-000000;1-000001;1-000002', 'MATCH':'1-000000;1-000001;1-000002', 'poi':'yes'}},\n"
      "{'type':'node','id': -3,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'2', 'ID':'1-000003;2-000000;2-000001;2-000002', 'MATCH':'1-000002;1-000000', 'poi':'yes'}}\n"
      "]}\n";

    OsmJsonReader reader;
    reader.setDefaultStatus(Status::Unknown1);

    OsmMapPtr input = std::make_shared<OsmMap>();
    reader.loadFromString(inputJson, input);
    input->getNode(-3)->setStatus(Status::Unknown2);

    OsmMapPtr conflated = std::make_shared<OsmMap>();
    reader.loadFromString(conflatedJson, conflated);
    conflated->getNode(-3)->setStatus(Status::Unknown2);

    MultiaryMatchComparator uut;
    double correct = uut.evaluateMatches(input, conflated);
    HOOT_STR_EQUALS(0.285714, correct);
    HOOT_STR_EQUALS(4, uut.getTp());
    HOOT_STR_EQUALS(5, uut.getFp());
    HOOT_STR_EQUALS(5, uut.getFn());
    // this doesn't include True Negatives
    HOOT_STR_EQUALS(14, uut.getTotalCount());
  }

  /**
   * Expected 1 cluster of 3
   * Actual 1 cluster of 3
   */
  void runMatchTest3()
  {
    QString inputJson =
      "{'version': 0.6,'generator': 'Overpass API','elements':[\n"
      "{'type':'node','id': -1,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000000', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type':'node','id': -2,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000001', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type':'node','id': -3,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'2', 'ID':'2-000000', 'MATCH':'1-000000', 'poi':'yes'}}\n"
      "]}\n";

    QString conflatedJson =
      "{'version': 0.6,'generator': 'Overpass API','elements':[\n"
      "{'type':'node','id': -1,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'Conflated', 'ID':'1-000000;1-000001;2-000000', 'MATCH':'1-000000', 'poi':'yes'}}\n"
      "]}\n";

    OsmJsonReader reader;
    reader.setDefaultStatus(Status::Unknown1);

    OsmMapPtr input = std::make_shared<OsmMap>();
    reader.loadFromString(inputJson, input);

    OsmMapPtr conflated = std::make_shared<OsmMap>();
    reader.loadFromString(conflatedJson, conflated);

    MultiaryMatchComparator uut;
    double correct = uut.evaluateMatches(input, conflated);
    HOOT_STR_EQUALS(false, conflated->getNode(-1)->getTags().contains(MetadataTags::HootWrong()));
    LOG_VAR(OsmJsonWriter().toString(conflated));
    LOG_VAR(uut.toString());
    HOOT_STR_EQUALS(1, correct);
    HOOT_STR_EQUALS(3, uut.getTp());
    HOOT_STR_EQUALS(0, uut.getFp());
    HOOT_STR_EQUALS(0, uut.getFn());
    // this doesn't include True Negatives
    HOOT_STR_EQUALS(3, uut.getTotalCount());
  }

  /**
   * Expected 1 clusters of 2 and two reviews.
   * Actual 1 clusters of 2 and two reviews.
   */
  void runReviewTest1()
  {
    QString inputJson =
      "{'version': 0.6,'generator': 'Overpass API','elements':[\n"
      "{'type':'node','id': -1,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000000', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type':'node','id': -2,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000001', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type':'node','id': -3,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000002', 'MATCH':'none', 'poi':'yes'}},\n"
      "{'type':'node','id': -4,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'2', 'ID':'2-000000', 'MATCH':'none', 'REVIEW':'1-000000;1-000002', 'poi':'yes'}}\n"
      "]}\n";

    QString conflatedJson =
      "{'version': 0.6,'generator': 'Overpass API','elements':[\n"
      "{'type':'node','id': -1,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'Conflated', 'ID':'1-000000;1-000001', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type':'node','id': -3,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000002', 'MATCH':'none', 'poi':'yes'}},\n"
      "{'type':'node','id': -4,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'2', 'ID':'2-000000', 'REVIEW':'1-000000;1-000002', 'poi':'yes'}},\n"
      "{'type': 'relation','id': -1,'members': [\n"
      "  {'type': 'node', 'ref': -1, 'role': 'reviewee'},\n"
      "  {'type': 'node', 'ref': -4, 'role': 'reviewee'}\n"
      "],\n"
      "  'tags':{'hoot:review:needs':'yes', 'hoot:review:members':'2', 'hoot:review:score':'1', 'type':'review'}},\n"
      "{'type': 'relation','id': -2,'members': [\n"
      "  {'type': 'node', 'ref': -3, 'role': 'reviewee'},\n"
      "  {'type': 'node', 'ref': -4, 'role': 'reviewee'}\n"
      "],\n"
      "  'tags':{'hoot:review:needs':'yes', 'hoot:review:members':'2', 'hoot:review:score':'1', 'type':'review'}}\n"
      "]}\n";

    OsmJsonReader reader;
    reader.setDefaultStatus(Status::Unknown1);

    OsmMapPtr input = std::make_shared<OsmMap>();
    reader.loadFromString(inputJson, input);
    input->getNode(-3)->setStatus(Status::Unknown2);

    OsmMapPtr conflated = std::make_shared<OsmMap>();
    reader.loadFromString(conflatedJson, conflated);
    conflated->getNode(-3)->setStatus(Status::Unknown2);

    MultiaryMatchComparator uut;
    double correct = uut.evaluateMatches(input, conflated);
    LOG_VAR(uut.toString());
    LOG_VAR(uut.getFScore());
    // this is arguable. We aren't explicitly stating that 2-000000 should be reviewed against
    // 1-000001, but it is being implicitly reviewed because of the 1-000000 vs 1-000001 match.
    // this leads to one unexpected review and an error.
    HOOT_STR_EQUALS(.75, correct);
    HOOT_STR_EQUALS(3, uut.getTp());
    HOOT_STR_EQUALS(1, uut.getFp());
    HOOT_STR_EQUALS(0, uut.getFn());
    // this doesn't include True Negatives
    HOOT_STR_EQUALS(4, uut.getTotalCount());
  }

  /**
   * Expected 1 clusters of 2 and one review.
   * Actual 1 clusters of 2 and one incorrect reviews.
   */
  void runReviewTest2()
  {
    QString inputJson =
      "{'version': 0.6,'generator': 'Overpass API','elements':[\n"
      "{'type':'node','id': -1,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000000', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type':'node','id': -2,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000001', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type':'node','id': -3,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000002', 'MATCH':'none', 'poi':'yes'}},\n"
      "{'type':'node','id': -4,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'2', 'ID':'2-000000', 'MATCH':'none', 'REVIEW':'1-000002', 'poi':'yes'}}\n"
      "]}\n";

    QString conflatedJson =
      "{'version': 0.6,'generator': 'Overpass API','elements':[\n"
      "{'type':'node','id': -1,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'Conflated', 'ID':'1-000000;1-000001', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type':'node','id': -3,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000002', 'MATCH':'none', 'poi':'yes'}},\n"
      "{'type':'node','id': -4,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'2', 'ID':'2-000000', 'REVIEW':'1-000002', 'poi':'yes'}},\n"
      "{'type': 'relation','id': -1,'members': [\n"
      "  {'type': 'node', 'ref': -1, 'role': 'reviewee'},\n"
      "  {'type': 'node', 'ref': -4, 'role': 'reviewee'}\n"
      "],\n"
      "  'tags':{'hoot:review:needs':'yes', 'hoot:review:members':'2', 'hoot:review:score':'1', 'type':'review'}}\n"
      "]}\n";

    OsmJsonReader reader;
    reader.setDefaultStatus(Status::Unknown1);

    OsmMapPtr input = std::make_shared<OsmMap>();
    reader.loadFromString(inputJson, input);
    input->getNode(-3)->setStatus(Status::Unknown2);

    OsmMapPtr conflated = std::make_shared<OsmMap>();
    reader.loadFromString(conflatedJson, conflated);
    conflated->getNode(-3)->setStatus(Status::Unknown2);

    MultiaryMatchComparator uut;
    double correct = uut.evaluateMatches(input, conflated);
    LOG_VAR(uut.toString());
    LOG_VAR(uut.getFScore());
    HOOT_STR_EQUALS(0.25, correct);
    HOOT_STR_EQUALS(0.4, uut.getFScore());
    HOOT_STR_EQUALS(1, uut.getTp());
    HOOT_STR_EQUALS(2, uut.getFp());
    HOOT_STR_EQUALS(1, uut.getFn());
    // this doesn't include True Negatives
    HOOT_STR_EQUALS(4, uut.getTotalCount());
  }


  /**
   * Expected 1 clusters of 2.
   * Actual 1 review.
   */
  void runReviewTest3()
  {
    QString inputJson =
      "{'version': 0.6,'generator': 'Overpass API','elements':[\n"
      "{'type':'node','id': -1,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000000', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type':'node','id': -2,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000001', 'MATCH':'1-000000', 'poi':'yes'}}\n"
      "]}\n";

    QString conflatedJson =
      "{'version': 0.6,'generator': 'Overpass API','elements':[\n"
      "{'type':'node','id': -1,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'Conflated', 'ID':'1-000000', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type':'node','id': -2,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000001', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type': 'relation','id': -1,'members': [\n"
      "  {'type': 'node', 'ref': -1, 'role': 'reviewee'},\n"
      "  {'type': 'node', 'ref': -2, 'role': 'reviewee'}\n"
      "],\n"
      "  'tags':{'hoot:review:needs':'yes', 'hoot:review:members':'2', 'hoot:review:score':'1', 'type':'review'}}\n"
      "]}\n";

    OsmJsonReader reader;
    reader.setDefaultStatus(Status::Unknown1);

    OsmMapPtr input = std::make_shared<OsmMap>();
    reader.loadFromString(inputJson, input);
    input->getNode(-2)->setStatus(Status::Unknown2);

    OsmMapPtr conflated = std::make_shared<OsmMap>();
    reader.loadFromString(conflatedJson, conflated);
    conflated->getNode(-2)->setStatus(Status::Unknown2);

    MultiaryMatchComparator uut;
    double correct = uut.evaluateMatches(input, conflated);

    LOG_VAR(OsmJsonWriter().toString(conflated));
    HOOT_STR_EQUALS(0, correct);
    HOOT_STR_EQUALS(0, uut.getFScore());
    HOOT_STR_EQUALS(0, uut.getTp());
    HOOT_STR_EQUALS(0, uut.getFp());
    HOOT_STR_EQUALS(1, uut.getFn());
    // this doesn't include True Negatives
    HOOT_STR_EQUALS(1, uut.getTotalCount());
  }

  /**
   * Expected 1 clusters of 2.
   * Actual 1 review.
   */
  void runReviewTest4()
  {
    QString translationScript = ConfPath::getHootHome() + "/translation-local/HGISv20.js";

    if (QFile::exists(translationScript) == false)
    {
      LOG_INFO("HGIS translation script doesn't exist, skipping "
        "MultiaryMatchComparatorTest::runReviewTest4.");
      return;
    }

    QString inputJson =
      "{'version': 0.6,'generator': 'Overpass API','elements':[\n"
      "{'type':'node','id': -1,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000000', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type':'node','id': -2,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000001', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type':'node','id': -3,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000002', 'MATCH':'1-000000', 'poi':'yes'}}\n"
      "]}\n";

    QString conflatedJson =
      "{'version': 0.6,'generator': 'Overpass API','elements':[\n"
      "{'type':'node','id': -1,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'Conflated', 'ID':'1-000000;1-000002', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type':'node','id': -2,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000001', 'MATCH':'1-000000', 'poi':'yes'}},\n"
      "{'type': 'relation','id': -1,'members': [\n"
      "  {'type': 'node', 'ref': -1, 'role': 'reviewee'},\n"
      "  {'type': 'node', 'ref': -2, 'role': 'reviewee'}\n"
      "],\n"
      "  'tags':{'hoot:review:needs':'yes', 'hoot:review:members':'2', 'hoot:review:score':'1', 'type':'review'}}\n"
      "]}\n";

    OsmJsonReader reader;
    reader.setDefaultStatus(Status::Unknown1);

    OsmMapPtr input = std::make_shared<OsmMap>();
    reader.loadFromString(inputJson, input);
    input->getNode(-2)->setStatus(Status::Unknown2);

    OsmMapPtr conflated = std::make_shared<OsmMap>();
    reader.loadFromString(conflatedJson, conflated);
    conflated->getNode(-2)->setStatus(Status::Unknown2);

    MultiaryMatchComparator uut;

    uut.setTranslationScript(translationScript);
    double correct = uut.evaluateMatches(input, conflated);

    LOG_VAR(OsmJsonWriter().toString(conflated));
    LOG_VAR(uut.toString());
    HOOT_STR_EQUALS(1.0 / 3.0, correct);
    HOOT_STR_EQUALS(.5, uut.getFScore());
    HOOT_STR_EQUALS(1, uut.getTp());
    HOOT_STR_EQUALS(0, uut.getFp());
    HOOT_STR_EQUALS(2, uut.getFn());
    // this doesn't include True Negatives
    HOOT_STR_EQUALS(3, uut.getTotalCount());
  }

  /**
   * Expected no matches.
   * Actual 1 cluster of 5.
   *
   * There are 10 reported false postives here for this reason:
   *
   * 1. -1 to -2
   * 2. -1 to -3
   * 3. -1 to -4
   * 4. -1 to -5
   * 5. -2 to -3
   * 6. -2 to -4
   * 7. -2 to -5
   * 8. -3 to -4
   * 9. -3 to -5
   * 10. -4 to -5
   */
  void runMatchMissTest1()
  {
    QString inputJson =
      "{'version': 0.6,'generator': 'Overpass API','elements':[\n"
      "{'type':'node','id': -1,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000000', 'MATCH':'none', 'poi':'yes'}},\n"
      "{'type':'node','id': -2,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000001', 'MATCH':'none', 'poi':'yes'}},\n"
      "{'type':'node','id': -3,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000002', 'MATCH':'none', 'poi':'yes'}},\n"
      "{'type':'node','id': -4,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000003', 'MATCH':'none', 'poi':'yes'}},\n"
      "{'type':'node','id': -5,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'1', 'ID':'1-000004', 'MATCH':'none', 'poi':'yes'}}\n"
      "]}\n";

    QString conflatedJson =
      "{'version': 0.6,'generator': 'Overpass API','elements':[\n"
      "{'type':'node','id': -1,'lat': 2.0,'lon': -3.0,\n"
      "  'tags':{'hoot:status':'Conflated', 'ID':'1-000000;1-000001;1-000002;1-000003;1-000004', 'MATCH':'none', 'poi':'yes'}}\n"
      "]}\n";

    OsmJsonReader reader;
    reader.setDefaultStatus(Status::Unknown1);
    OsmMapPtr input = std::make_shared<OsmMap>();
    reader.loadFromString(inputJson, input);

    OsmMapPtr conflated = std::make_shared<OsmMap>();
    reader.loadFromString(conflatedJson, conflated);

    MultiaryMatchComparator uut;
    double correct = uut.evaluateMatches(input, conflated);
    LOG_VAR(uut.toString());
    LOG_VAR(uut.getFScore());
    HOOT_STR_EQUALS(0, correct);
    HOOT_STR_EQUALS(0, uut.getFScore());
    HOOT_STR_EQUALS(0, uut.getTp());
    HOOT_STR_EQUALS(10, uut.getFp());
    HOOT_STR_EQUALS(0, uut.getFn());
    // this doesn't include True Negatives
    HOOT_STR_EQUALS(10, uut.getTotalCount());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MultiaryMatchComparatorTest, "glacial");

}
