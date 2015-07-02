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
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
using namespace hoot;

#include "../TestUtils.h"

namespace hoot
{

class ReviewMarkerTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ReviewMarkerTest);
  CPPUNIT_TEST(runSimpleTest);
  CPPUNIT_TEST(runMultipleScoresTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runSimpleTest()
  {
    OsmMapPtr map(new OsmMap());
    ElementPtr n1(new Node(Status::Unknown1, 1, 0, 0, 0));
    ElementPtr n2(new Node(Status::Unknown2, 2, 0, 0, 0));

    // set the uuids so they don't change with each test
    n1->getTags().set("uuid", "n1");
    n2->getTags().set("uuid", "n2");

    ReviewMarker uut;

    uut.mark(map, n1, n2, "a note");

    HOOT_STR_EQUALS("hoot:review:needs = yes\n"
                    "hoot:review:source = 1\n"
                    "hoot:review:uuid = n2\n"
                    "hoot:review:note = a note\n"
                    "uuid = n1\n"
                    "", n1->getTags());

    HOOT_STR_EQUALS("hoot:review:needs = yes\n"
                    "hoot:review:source = 2\n"
                    "hoot:review:uuid = n1\n"
                    "hoot:review:note = a note\n"
                    "uuid = n2\n"
                    "", n2->getTags());
  }

  /**
   * This can happen with some of the old data. The new code shouldn't generate these values.
   */
  void runMultipleScoresTest()
  {
    OsmMapPtr map(new OsmMap());
    DisableLog dl;

    ElementPtr n1(new Node(Status::Unknown1, 1, 0, 0, 0));
    ElementPtr n2(new Node(Status::Unknown2, 2, 0, 0, 0));

    // set the uuids so they don't change with each test
    n1->getTags().set("uuid", "n1");
    n2->getTags().set("uuid", "n2");
    n2->getTags().set("hoot:review:score", "0.1;0.2");

    ReviewMarker uut;

    uut.mark(map, n1, n2, "a note", 0.15);

    HOOT_STR_EQUALS("hoot:review:needs = yes\n"
                    "hoot:review:source = 1\n"
                    "hoot:review:score = 0.15\n"
                    "hoot:review:uuid = n2\n"
                    "hoot:review:note = a note\n"
                    "uuid = n1\n"
                    "", n1->getTags());

    HOOT_STR_EQUALS("hoot:review:needs = yes\n"
                    "hoot:review:source = 2\n"
                    "hoot:review:score = 0.2\n"
                    "hoot:review:uuid = n1\n"
                    "hoot:review:note = a note\n"
                    "uuid = n2\n"
                    "", n2->getTags());
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ReviewMarkerTest, "quick");

}
