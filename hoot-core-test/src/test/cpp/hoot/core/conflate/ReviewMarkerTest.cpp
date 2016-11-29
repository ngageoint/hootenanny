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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/conflate/ReviewMarker.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
using namespace hoot;

#include "../TestUtils.h"

namespace hoot
{

class ReviewMarkerTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ReviewMarkerTest);
  CPPUNIT_TEST(runNeedsReviewTest);
  CPPUNIT_TEST(runSimpleTest);
  CPPUNIT_TEST(runMultipleScoresTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runNeedsReviewTest()
  {
    TestUtils::resetEnvironment();

    OsmMapPtr map(new OsmMap());
    ElementPtr n1(new Node(Status::Unknown1, 1, 0, 0, 0));
    ElementPtr n2(new Node(Status::Unknown2, 2, 0, 0, 0));
    ElementPtr n3(new Node(Status::Unknown2, 3, 0, 0, 0));

    // set the uuids so they don't change with each test
    n1->getTags().set("uuid", "n1");
    n2->getTags().set("uuid", "n2");
    n3->getTags().set("uuid", "n2");
    map->addElement(n1);
    map->addElement(n2);
    map->addElement(n3);

    ReviewMarker uut;

    uut.mark(map, n1, n2, "a note", "test");
    LOG_VAR(uut.isNeedsReview(map, n1, n2));
    LOG_VAR(uut.isNeedsReview(map, n2, n3));
    LOG_VAR(uut.isNeedsReview(map, n3, n1));

    HOOT_STR_EQUALS(true, uut.isNeedsReview(map, n1, n2));
    HOOT_STR_EQUALS(true, uut.isNeedsReview(map, n2, n1));
    HOOT_STR_EQUALS(false, uut.isNeedsReview(map, n2, n3));
    HOOT_STR_EQUALS(false, uut.isNeedsReview(map, n3, n1));
  }

  void runSimpleTest()
  {
    TestUtils::resetEnvironment();

    OsmMapPtr map(new OsmMap());
    ElementPtr n1(new Node(Status::Unknown1, 1, 0, 0, 0));
    ElementPtr n2(new Node(Status::Unknown2, 2, 0, 0, 0));

    // set the uuids so they don't change with each test
    n1->getTags().set("uuid", "n1");
    n2->getTags().set("uuid", "n2");
    map->addElement(n1);
    map->addElement(n2);

    ReviewMarker uut;

    uut.mark(map, n1, n2, "a note", "test");

    HOOT_STR_EQUALS("{\"version\": 0.6,\"generator\": \"Hootenanny\",\"elements\": [\n"
      "{\"type\":\"node\",\"id\":2,\"lat\":0,\"lon\":0,\"tags\":{\"uuid\":\"n2\"}},\n"
      "{\"type\":\"node\",\"id\":1,\"lat\":0,\"lon\":0,\"tags\":{\"uuid\":\"n1\"}},\n"
      "{\"type\":\"relation\",\"id\":-1,\"members\":[\n"
      "{\"type\":\"node\",\"ref\":1,\"role\":\"reviewee\"},\n"
      "{\"type\":\"node\",\"ref\":2,\"role\":\"reviewee\"}],\"tags\":{\"hoot:review:needs\":\"yes\",\"hoot:review:type\":\"test\",\"hoot:review:members\":\"2\",\"hoot:review:score\":\"-1\",\"hoot:review:note\":\"a note\",\"error:circular\":\"15\"}]\n"
      "}\n",
      OsmJsonWriter().toString(map));
  }

  /**
   * This can happen with some of the old data. The new code shouldn't generate these values.
   */
  void runMultipleScoresTest()
  {
    TestUtils::resetEnvironment();

    OsmMapPtr map(new OsmMap());
    DisableLog dl;

    ElementPtr n1(new Node(Status::Unknown1, 1, 0, 0, 0));
    ElementPtr n2(new Node(Status::Unknown2, 2, 0, 0, 0));

    // set the uuids so they don't change with each test
    n1->getTags().set("uuid", "n1");
    n2->getTags().set("uuid", "n2");
    map->addElement(n1);
    map->addElement(n2);

    ReviewMarker uut;

    uut.mark(map, n1, n2, "a note", "test", 0.15);
    uut.mark(map, n1, n2, "a note 2", "test", 0.5);

    HOOT_STR_EQUALS("{\"version\": 0.6,\"generator\": \"Hootenanny\",\"elements\": [\n"
      "{\"type\":\"node\",\"id\":2,\"lat\":0,\"lon\":0,\"tags\":{\"uuid\":\"n2\"}},\n"
      "{\"type\":\"node\",\"id\":1,\"lat\":0,\"lon\":0,\"tags\":{\"uuid\":\"n1\"}},\n"
      "{\"type\":\"relation\",\"id\":-2,\"members\":[\n"
      "{\"type\":\"node\",\"ref\":1,\"role\":\"reviewee\"},\n"
      "{\"type\":\"node\",\"ref\":2,\"role\":\"reviewee\"}],\"tags\":{\"hoot:review:needs\":\"yes\",\"hoot:review:type\":\"test\",\"hoot:review:members\":\"2\",\"hoot:review:score\":\"0.5\",\"hoot:review:note\":\"a note 2\",\"error:circular\":\"15\"},\n"
      "{\"type\":\"relation\",\"id\":-1,\"members\":[\n"
      "{\"type\":\"node\",\"ref\":1,\"role\":\"reviewee\"},\n"
      "{\"type\":\"node\",\"ref\":2,\"role\":\"reviewee\"}],\"tags\":{\"hoot:review:needs\":\"yes\",\"hoot:review:type\":\"test\",\"hoot:review:members\":\"2\",\"hoot:review:score\":\"0.15\",\"hoot:review:note\":\"a note\",\"error:circular\":\"15\"}]\n"
      "}\n",
      OsmJsonWriter().toString(map));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ReviewMarkerTest, "quick");

}
