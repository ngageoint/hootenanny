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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

//  Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmApiMatchFailure.h>
#include <hoot/core/util/FileUtils.h>

namespace hoot
{

class OsmApiMatchFailureTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmApiMatchFailureTest);
  CPPUNIT_TEST(runXmlChangesetFailureMatchesTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OsmApiMatchFailureTest() = default;

  void runXmlChangesetFailureMatchesTest()
  {
    OsmApiMatchFailure failureCheck;

    bool found = false;
    long id = 0;
    long id2 = 0;
    std::vector<long> ids;
    ElementType::Type type = ElementType::Unknown;
    ElementType::Type type2 = ElementType::Unknown;
    QString hint;

    //  Test Placeholder failure with node in way
    id = 0;
    id2 = 0;
    type = ElementType::Unknown;
    type2 = ElementType::Unknown;
    hint = "Placeholder node not found for reference -145213 in way -5687";
    found = failureCheck.matchesPlaceholderFailure(hint, id, type, id2, type2);
    CPPUNIT_ASSERT_EQUAL(true, found);
    CPPUNIT_ASSERT_EQUAL(-145213L, id);
    CPPUNIT_ASSERT_EQUAL(ElementType::Node, type);
    CPPUNIT_ASSERT_EQUAL(-5687L, id2);
    CPPUNIT_ASSERT_EQUAL(ElementType::Way, type2);

    //  Test Placeholder failure with way in relation
    id = 0;
    id2 = 0;
    type = ElementType::Unknown;
    type2 = ElementType::Unknown;
    hint = "Placeholder Way not found for reference -12257 in Relation -51";
    found = failureCheck.matchesPlaceholderFailure(hint, id, type, id2, type2);
    CPPUNIT_ASSERT_EQUAL(true, found);
    CPPUNIT_ASSERT_EQUAL(-12257L, id);
    CPPUNIT_ASSERT_EQUAL(ElementType::Way, type);
    CPPUNIT_ASSERT_EQUAL(-51L, id2);
    CPPUNIT_ASSERT_EQUAL(ElementType::Relation, type2);

    //  Test Placeholder failure with empty string
    id = 0;
    id2 = 0;
    type = ElementType::Unknown;
    type2 = ElementType::Unknown;
    hint = "";
    found = failureCheck.matchesPlaceholderFailure(hint, id, type, id2, type2);
    CPPUNIT_ASSERT_EQUAL(false, found);
    CPPUNIT_ASSERT_EQUAL(0L, id);
    CPPUNIT_ASSERT_EQUAL(ElementType::Unknown, type);
    CPPUNIT_ASSERT_EQUAL(0L, id2);
    CPPUNIT_ASSERT_EQUAL(ElementType::Unknown, type2);

    //  Test relation failure with no relation ID
    id = 0;
    id2 = 0;
    type = ElementType::Unknown;
    hint = "Relation with id  cannot be saved due to Relation with id 1707699";
    found = failureCheck.matchesRelationFailure(hint, id, id2, type);
    CPPUNIT_ASSERT_EQUAL(true, found);
    CPPUNIT_ASSERT_EQUAL(0L, id);
    CPPUNIT_ASSERT_EQUAL(1707699L, id2);
    CPPUNIT_ASSERT_EQUAL(ElementType::Relation, type);

    //  Test relation failure with relation ID
    id = 0;
    id2 = 0;
    type = ElementType::Unknown;
    hint = "Relation with id 122 cannot be saved due to Way with id 7699";
    found = failureCheck.matchesRelationFailure(hint, id, id2, type);
    CPPUNIT_ASSERT_EQUAL(true, found);
    CPPUNIT_ASSERT_EQUAL(122L, id);
    CPPUNIT_ASSERT_EQUAL(7699L, id2);
    CPPUNIT_ASSERT_EQUAL(ElementType::Way, type);

    //  Test multi relation failure with relation ID
    id = 0;
    ids.clear();
    type = ElementType::Unknown;
    type2 = ElementType::Unknown;
    hint = "Relation -2 requires the relations with id in 1707148,1707249,1707264,1707653,1707654,1707655,1707656,1707657,1707658,1707699,1707700, which either do not exist, or are not visible.";
    found = failureCheck.matchesMultiElementFailure(hint, id, type, ids, type2);
    std::vector<long> expected_relation_ids { 1707148, 1707249, 1707264, 1707653, 1707654, 1707655, 1707656, 1707657, 1707658, 1707699, 1707700 };
    CPPUNIT_ASSERT_EQUAL(true, found);
    CPPUNIT_ASSERT_EQUAL(-2L, id);
    CPPUNIT_ASSERT_EQUAL(ElementType::Relation, type);
    CPPUNIT_ASSERT_EQUAL(expected_relation_ids.size(), ids.size());
    for (int i = 0; i < (int)ids.size(); ++i)
      CPPUNIT_ASSERT_EQUAL(expected_relation_ids[i], ids[i]);
    CPPUNIT_ASSERT_EQUAL(ElementType::Relation, type2);

    //  Test multi node failure with way ID
    id = 0;
    ids.clear();
    type = ElementType::Unknown;
    type2 = ElementType::Unknown;
    hint = "Way -1 requires the nodes with id in 11,20, which either do not exist, or are not visible.";
    found = failureCheck.matchesMultiElementFailure(hint, id, type, ids, type2);
    std::vector<long> expected_node_ids { 11, 20 };
    CPPUNIT_ASSERT_EQUAL(true, found);
    CPPUNIT_ASSERT_EQUAL(-1L, id);
    CPPUNIT_ASSERT_EQUAL(ElementType::Way, type);
    CPPUNIT_ASSERT_EQUAL(expected_node_ids.size(), ids.size());
    for (int i = 0; i < (int)ids.size(); ++i)
      CPPUNIT_ASSERT_EQUAL(expected_node_ids[i], ids[i]);
    CPPUNIT_ASSERT_EQUAL(ElementType::Node, type2);

    //  Test multi way failure with node ID
    id = 0;
    ids.clear();
    type = ElementType::Unknown;
    type2 = ElementType::Unknown;
    hint = "Precondition failed: Node 1671 is still used by ways 308,880,884,1071.";
    found = failureCheck.matchesChangesetDeletePreconditionFailure(hint, id, type, ids, type2);
    std::vector<long> expected_way_ids { 308, 880, 884, 1071 };
    CPPUNIT_ASSERT_EQUAL(true, found);
    CPPUNIT_ASSERT_EQUAL(1671L, id);
    CPPUNIT_ASSERT_EQUAL(ElementType::Node, type);
    CPPUNIT_ASSERT_EQUAL(expected_way_ids.size(), ids.size());
    for (int i = 0; i < (int)ids.size(); ++i)
      CPPUNIT_ASSERT_EQUAL(expected_way_ids[i], ids[i]);
    CPPUNIT_ASSERT_EQUAL(ElementType::Way, type2);

    //  Test multi relation failure with relation ID
    id = 0;
    ids.clear();
    type = ElementType::Unknown;
    type2 = ElementType::Unknown;
    hint = "Precondition failed: The relation 220 is used in relations 159,189";
    found = failureCheck.matchesChangesetDeletePreconditionFailure(hint, id, type, ids, type2);
    std::vector<long> expected_deleted_relation_ids { 159, 189 };
    CPPUNIT_ASSERT_EQUAL(true, found);
    CPPUNIT_ASSERT_EQUAL(220L, id);
    CPPUNIT_ASSERT_EQUAL(ElementType::Relation, type);
    CPPUNIT_ASSERT_EQUAL(expected_deleted_relation_ids.size(), ids.size());
    for (int i = 0; i < (int)ids.size(); ++i)
      CPPUNIT_ASSERT_EQUAL(expected_deleted_relation_ids[i], ids[i]);
    CPPUNIT_ASSERT_EQUAL(ElementType::Relation, type2);

    //  Test relation failure with empty string
    id = 0;
    id2 = 0;
    type = ElementType::Unknown;
    hint = "";
    found = failureCheck.matchesRelationFailure(hint, id, id2, type);
    CPPUNIT_ASSERT_EQUAL(false, found);
    CPPUNIT_ASSERT_EQUAL(0L, id);
    CPPUNIT_ASSERT_EQUAL(0L, id2);
    CPPUNIT_ASSERT_EQUAL(ElementType::Unknown, type);

    //  Test changeset closed
    hint = "Changeset conflict: The changeset 49514404 was closed at 2020-01-23 20:11:10 UTC";
    found = failureCheck.matchesChangesetClosedFailure(hint);
    CPPUNIT_ASSERT_EQUAL(true, found);

    //  Test element gone deleted
    id = 0;
    type = ElementType::Unknown;
    hint = "Gone: The node with the id 12345 has already been deleted";
    found = failureCheck.matchesElementGoneDeletedFailure(hint, id, type);
    CPPUNIT_ASSERT_EQUAL(true, found);
    CPPUNIT_ASSERT_EQUAL(12345L, id);
    CPPUNIT_ASSERT_EQUAL(ElementType::Node, type);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiMatchFailureTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiMatchFailureTest, "current");

}
