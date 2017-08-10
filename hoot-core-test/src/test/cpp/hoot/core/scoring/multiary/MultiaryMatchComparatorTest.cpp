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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */


// Hoot
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/scoring/multiary/MultiaryMatchComparator.h>
#include <hoot/core/util/Log.h>

using namespace hoot;

#include "../../TestUtils.h"

namespace hoot
{

class MultiaryMatchComparatorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(MultiaryMatchComparatorTest);
  CPPUNIT_TEST(runEmptyTest);
  CPPUNIT_TEST(runOneMatchTest);
  CPPUNIT_TEST(runMatchTest1);
  CPPUNIT_TEST(runMatchTest2);
  CPPUNIT_TEST(runReviewTest1);
  CPPUNIT_TEST(runReviewTest2);
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

    OsmMapPtr input = OsmJsonReader().loadFromString(inputJson);
    OsmMapPtr conflated = OsmJsonReader().loadFromString(inputJson);

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
    OsmMapPtr input = reader.loadFromString(inputJson);
    OsmMapPtr conflated = reader.loadFromString(conflatedJson);

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
    OsmMapPtr input = reader.loadFromString(inputJson);
    input->getNode(-3)->setStatus(Status::Unknown2);
    OsmMapPtr conflated = reader.loadFromString(conflatedJson);
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
    OsmMapPtr input = reader.loadFromString(inputJson);
    input->getNode(-3)->setStatus(Status::Unknown2);
    OsmMapPtr conflated = reader.loadFromString(conflatedJson);
    conflated->getNode(-3)->setStatus(Status::Unknown2);

    MultiaryMatchComparator uut;
    double correct = uut.evaluateMatches(input, conflated);
    LOG_VAR(uut.toString());
    LOG_VAR(uut.getFScore());
    HOOT_STR_EQUALS(0.285714, correct);
    HOOT_STR_EQUALS(4, uut.getTp());
    HOOT_STR_EQUALS(5, uut.getFp());
    HOOT_STR_EQUALS(5, uut.getFn());
    // this doesn't include True Negatives
    HOOT_STR_EQUALS(14, uut.getTotalCount());
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
      "  'tags':{'hoot:status':'2', 'ID':'2-000000', 'REVIEW':'1-000000;1-000002', 'poi':'yes'}}\n"
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
    OsmMapPtr input = reader.loadFromString(inputJson);
    input->getNode(-3)->setStatus(Status::Unknown2);
    OsmMapPtr conflated = reader.loadFromString(conflatedJson);
    conflated->getNode(-3)->setStatus(Status::Unknown2);

    MultiaryMatchComparator uut;
    double correct = uut.evaluateMatches(input, conflated);
    LOG_VAR(uut.toString());
    LOG_VAR(uut.getFScore());
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
      "  'tags':{'hoot:status':'2', 'ID':'2-000000', 'REVIEW':'1-000002', 'poi':'yes'}}\n"
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
      "  'tags':{'hoot:review:needs':'yes', 'hoot:review:members':'2', 'hoot:review:score':'1', 'type':'review'}}\n"
      "]}\n";

    OsmJsonReader reader;
    reader.setDefaultStatus(Status::Unknown1);
    OsmMapPtr input = reader.loadFromString(inputJson);
    input->getNode(-3)->setStatus(Status::Unknown2);
    OsmMapPtr conflated = reader.loadFromString(conflatedJson);
    conflated->getNode(-3)->setStatus(Status::Unknown2);

    MultiaryMatchComparator uut;
    double correct = uut.evaluateMatches(input, conflated);
    HOOT_STR_EQUALS(0.33333333, correct);
    HOOT_STR_EQUALS(0.5, uut.getFScore());
    HOOT_STR_EQUALS(1, uut.getTp());
    HOOT_STR_EQUALS(1, uut.getFp());
    HOOT_STR_EQUALS(1, uut.getFn());
    // this doesn't include True Negatives
    HOOT_STR_EQUALS(3, uut.getTotalCount());
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(MultiaryMatchComparatorTest);

}
