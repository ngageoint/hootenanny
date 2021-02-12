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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/ops/ManualMatchValidator.h>
#include <hoot/core/ops/RemoveNodeByEid.h>

// CPPUnit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class ManualMatchValidatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ManualMatchValidatorTest);
  CPPUNIT_TEST(runEmptyIdTest);
  CPPUNIT_TEST(runInvalidIdComboTest);
  CPPUNIT_TEST(runMissingRef1Test);
  CPPUNIT_TEST(runMissingRef1NotRequiredTest);
  CPPUNIT_TEST(runInvalidIdTest);
  CPPUNIT_TEST(runUuidTest);
  CPPUNIT_TEST(runRepeatedIdTest);
  CPPUNIT_TEST(runStatusTest);
  CPPUNIT_TEST(runMultipleRef1Test);
  CPPUNIT_TEST(runInvalidMultipleIdTest);
  CPPUNIT_TEST(runDuplicateIdTest);
  CPPUNIT_TEST(runFullDebugOutputTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ManualMatchValidatorTest() : HootTestFixture(UNUSED_PATH, UNUSED_PATH)
  {
  }

  void runEmptyIdTest()
  {
    // empty manual match ids aren't allowed

    OsmMapPtr map(new OsmMap());
    Tags tags;

    tags.set(MetadataTags::Ref1(), "");
    NodePtr node = TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    tags.clear();
    tags.set(MetadataTags::Ref2(), "");
    geos::geom::Coordinate c1[] = {
      geos::geom::Coordinate(0.0, 0.0), geos::geom::Coordinate(1.0, 1.0),
      geos::geom::Coordinate::getNull() };
    WayPtr way = TestUtils::createWay(map, c1, Status::Unknown1, 15.0, tags);

    tags.clear();
    tags.set(MetadataTags::Review(), "");
    QList<ElementPtr> relationMembers;
    relationMembers.append(node);
    relationMembers.append(way);
    ConstRelationPtr relation =
      TestUtils::createRelation(map, relationMembers, Status::Unknown1, 15.0, tags);

    ManualMatchValidator uut;
    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
//    LOG_VARW(uut.getErrors().size());
//    QMap<ElementId, QString> errors = uut.getErrors();
//    for (QMap<ElementId, QString>::const_iterator itr = errors.begin();
//         itr != errors.end(); ++itr)
//    {
//      LOG_WARN(itr.key().toString() << ";" << itr.value());
//    }
    CPPUNIT_ASSERT(uut.getErrors().size() == 3);
    QMap<ElementId, QString>::const_iterator errorItr =
      uut.getErrors().find(node->getElementId());
    HOOT_STR_EQUALS("Empty REF1 tag", errorItr.value().toStdString());
    errorItr = uut.getErrors().find(way->getElementId());
    HOOT_STR_EQUALS("Empty REF2 tag", errorItr.value());
    errorItr = uut.getErrors().find(relation->getElementId());
    HOOT_STR_EQUALS("Empty REVIEW tag", errorItr.value().toStdString());
  }

  void runInvalidIdComboTest()
  {
    // can't have ref1 and ref2/review ids on the same element

    OsmMapPtr map(new OsmMap());
    Tags tags;
    ManualMatchValidator uut;
    QMap<ElementId, QString>::const_iterator errorItr;

    tags.set(MetadataTags::Ref1(), "002c75");
    tags.set(MetadataTags::Ref2(), "002da0");
    NodePtr node = TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(node->getElementId());
    HOOT_STR_EQUALS(
      "Element has both REF1 and either a REF2 or REVIEW tag", errorItr.value().toStdString());

    tags.clear();
    tags.set(MetadataTags::Ref1(), "002c75");
    tags.set(MetadataTags::Review(), "002da0");
    node->setTags(tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(node->getElementId());
    HOOT_STR_EQUALS(
      "Element has both REF1 and either a REF2 or REVIEW tag", errorItr.value().toStdString());
  }

  void runMissingRef1Test()
  {
    // all ref2/review manual match ids, excluding todo's and none, have to have a corresponding
    // and matching ref1 id

    OsmMapPtr map(new OsmMap());
    Tags tags;
    ManualMatchValidator uut;
    QMap<ElementId, QString>::const_iterator errorItr;

    tags.set(MetadataTags::Ref1(), "002c75");
    TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    tags.clear();
    tags.set(MetadataTags::Ref2(), "002da0");
    ConstNodePtr ref2 = TestUtils::createNode(map, Status::Unknown2, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(ref2->getElementId());
    HOOT_STR_EQUALS("No REF1 exists for REF2=002da0", errorItr.value().toStdString());

    tags.clear();
    RemoveNodeByEid::removeNode(map, ref2->getId());
    tags.set(MetadataTags::Review(), "002da0");
    ConstNodePtr review = TestUtils::createNode(map, Status::Unknown2, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    //LOG_VARW(uut.getErrors().size());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(review->getElementId());
    HOOT_STR_EQUALS("No REF1 exists for REVIEW=002da0", errorItr.value().toStdString());

    tags.clear();
    RemoveNodeByEid::removeNode(map, review->getId());
    tags.set(MetadataTags::Ref2(), "NONE");
    ref2 = TestUtils::createNode(map, Status::Unknown2, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(!uut.hasErrors());

    tags.clear();
    RemoveNodeByEid::removeNode(map, ref2->getId());
    tags.set(MetadataTags::Review(), "todo");
    review = TestUtils::createNode(map, Status::Unknown2, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(!uut.hasErrors());
  }

  void runMissingRef1NotRequiredTest()
  {
    // This makes ref1's corresponding to ref2/review effectively optional by logging warnings
    // instead of errors.

    OsmMapPtr map(new OsmMap());
    Tags tags;
    ManualMatchValidator uut;
    QMap<ElementId, QString>::const_iterator warningItr;

    uut.setRequireRef1(false);

    tags.set(MetadataTags::Ref1(), "002c75");
    TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    tags.clear();
    tags.set(MetadataTags::Ref2(), "002da0");
    ConstNodePtr ref2 = TestUtils::createNode(map, Status::Unknown2, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(!uut.hasErrors());
    CPPUNIT_ASSERT(uut.hasWarnings());
    CPPUNIT_ASSERT(uut.getWarnings().size() == 1);
    warningItr = uut.getWarnings().find(ref2->getElementId());
    HOOT_STR_EQUALS("No REF1 exists for REF2=002da0", warningItr.value().toStdString());
  }

  void runUuidTest()
  {
    // in backward compatibility mode, uuid's are allowed for all types of manual match ids

    OsmMapPtr map(new OsmMap());
    Tags tags;
    ManualMatchValidator uut;
    QMap<ElementId, QString>::const_iterator errorItr;
    QMap<ElementId, QString> errors;

    uut.setAllowUuidManualMatchIds(true);

    tags.set(MetadataTags::Ref1(), "{1c745d1e-39e5-4926-a2d3-8f87af39e037}");
    NodePtr node = TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(!uut.hasErrors());

    map->clear();
    node->getTags().clear();
    const QString badUuid = "{1c745d1e-3e5-4926-a2d3-8f87af39e037}";
    tags.set(MetadataTags::Ref1(), badUuid);
    node = TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errors = uut.getErrors();
    errorItr = errors.find(node->getElementId());
    const QString expectedErrorMsg = "Invalid REF1=" + badUuid;
    HOOT_STR_EQUALS(expectedErrorMsg.toStdString(), errorItr.value().toStdString());

    // regular ids should be allowed in compatibility mode as well

    map->clear();
    node->getTags().clear();
    tags.set(MetadataTags::Ref1(), "002c75");
    node = TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(!uut.hasErrors());
  }

  void runInvalidIdTest()
  {
    OsmMapPtr map(new OsmMap());
    Tags tags;
    ManualMatchValidator uut;
    QMap<ElementId, QString>::const_iterator errorItr;
    QMap<ElementId, QString> errors;
    QString expectedErrorMsg;

    // ref1 ids all must all be as defined by AddRef1Visitor

    tags.set(MetadataTags::Ref1(), "todo");
    ConstNodePtr ref1 = TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(ref1->getElementId());
    HOOT_STR_EQUALS("Invalid REF1=todo", errorItr.value().toStdString());

    map->clear();
    tags.clear();
    tags.set(MetadataTags::Ref1(), "none");
    ref1 = TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(ref1->getElementId());
    HOOT_STR_EQUALS("Invalid REF1=none", errorItr.value().toStdString());

    const QString idWrongSize = "df1dd"; // ids are of size >= 6, since a prefix is allowed

    map->clear();
    tags.clear();
    tags.set(MetadataTags::Ref1(), idWrongSize);
    ref1 = TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errors = uut.getErrors();
    errorItr = errors.find(ref1->getElementId());
    expectedErrorMsg = "Invalid REF1=" + idWrongSize;
    HOOT_STR_EQUALS(expectedErrorMsg.toStdString(), errorItr.value().toStdString());

    // ref2/review ids must all be either in the format specified by AddRef1Visitor, 'todo', or
    // 'none'

    map->clear();
    tags.clear();
    tags.set(MetadataTags::Ref2(), idWrongSize);
    ConstNodePtr ref2 = TestUtils::createNode(map, Status::Unknown2, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errors = uut.getErrors();
    errorItr = errors.find(ref2->getElementId());
    expectedErrorMsg = "Invalid REF2=" + idWrongSize;
    HOOT_STR_EQUALS(expectedErrorMsg.toStdString(), errorItr.value().toStdString());

    map->clear();
    tags.clear();
    tags.set(MetadataTags::Review(), idWrongSize);
    ConstNodePtr review = TestUtils::createNode(map, Status::Unknown2, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errors = uut.getErrors();
    errorItr = errors.find(review->getElementId());
    expectedErrorMsg = "Invalid REVIEW=" + idWrongSize;
    HOOT_STR_EQUALS(expectedErrorMsg.toStdString(), errorItr.value().toStdString());

    const QString idWrongEnd = "aas32df132a-df1d"; // last six must be alphanumeric

    map->clear();
    tags.clear();
    tags.set(MetadataTags::Ref1(), idWrongEnd);
    ref1 = TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errors = uut.getErrors();
    errorItr = errors.find(ref1->getElementId());
    expectedErrorMsg = "Invalid REF1=" + idWrongEnd;
    HOOT_STR_EQUALS(expectedErrorMsg.toStdString(), errorItr.value().toStdString());

    map->clear();
    tags.clear();
    tags.set(MetadataTags::Ref2(), idWrongEnd);
    ref2 = TestUtils::createNode(map, Status::Unknown2, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errors = uut.getErrors();
    errorItr = errors.find(ref2->getElementId());
    expectedErrorMsg = "Invalid REF2=" + idWrongEnd;
    HOOT_STR_EQUALS(expectedErrorMsg.toStdString(), errorItr.value().toStdString());

    map->clear();
    tags.clear();
    tags.set(MetadataTags::Review(), idWrongEnd);
    review = TestUtils::createNode(map, Status::Unknown2, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errors = uut.getErrors();
    errorItr = errors.find(review->getElementId());
    expectedErrorMsg = "Invalid REVIEW=" + idWrongEnd;
    HOOT_STR_EQUALS(expectedErrorMsg.toStdString(), errorItr.value().toStdString());
  }

  void runRepeatedIdTest()
  {
    // a ref2 and a review can't have the same id on the same element

    OsmMapPtr map(new OsmMap());
    Tags tags;
    ManualMatchValidator uut;
    QMap<ElementId, QString>::const_iterator errorItr;

    tags.set(MetadataTags::Ref1(), "002da0");
    TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    tags.clear();
    tags.set(MetadataTags::Ref2(), "002da0");
    tags.set(MetadataTags::Review(), "002da0");
    ConstNodePtr invalidRef = TestUtils::createNode(map, Status::Unknown2, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    //LOG_VARW(uut.getErrors().size());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(invalidRef->getElementId());
    HOOT_STR_EQUALS(
      "Invalid repeated ID: REF2=002da0, REVIEW=002da0", errorItr.value().toStdString());
  }

  void runStatusTest()
  {
    // data loaded with an Unknown1 status can't have any REF2 tags in it and data loaded with an
    // Unknown2 status can't have any REF1 tags in it

    OsmMapPtr map(new OsmMap());
    Tags tags;
    ManualMatchValidator uut;
    QMap<ElementId, QString>::const_iterator errorItr;

    tags.set(MetadataTags::Ref1(), "002da0");
    ConstNodePtr invalidRef1 = TestUtils::createNode(map, Status::Unknown2, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(invalidRef1->getElementId());
    HOOT_STR_EQUALS("Unknown2 element with REF1 tag", errorItr.value().toStdString());

    map->clear();
    tags.clear();
    tags.set(MetadataTags::Ref2(), "002da0");
    ConstNodePtr invalidRef2 = TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(invalidRef2->getElementId());
    HOOT_STR_EQUALS("Unknown1 element with REF2 or REVIEW tag", errorItr.value().toStdString());

    map->clear();
    tags.clear();
    tags.set(MetadataTags::Review(), "002da0");
    ConstNodePtr invalidReview = TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(invalidReview->getElementId());
    HOOT_STR_EQUALS("Unknown1 element with REF2 or REVIEW tag", errorItr.value().toStdString());
  }

  void runMultipleRef1Test()
  {
    // ref1 is always a single id

    OsmMapPtr map(new OsmMap());
    Tags tags;
    ManualMatchValidator uut;
    QMap<ElementId, QString>::const_iterator errorItr;

    tags.set(MetadataTags::Ref1(), "002da0;002e0f");
    ConstNodePtr invalidRef1 = TestUtils::createNode(map, Status::Unknown2, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(invalidRef1->getElementId());
    HOOT_STR_EQUALS("REF1 ID must be singular. REF1=002da0;002e0f", errorItr.value().toStdString());
  }

  void runInvalidMultipleIdTest()
  {
    // if a ref2/review has multiple ids, 'todo' and 'none' aren't valid entries

    OsmMapPtr map(new OsmMap());
    Tags tags;
    ManualMatchValidator uut;
    QMap<ElementId, QString>::const_iterator errorItr;

    tags.set(MetadataTags::Ref2(), "002da0;todo");
    ConstNodePtr invalidRef2 = TestUtils::createNode(map, Status::Unknown2, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(invalidRef2->getElementId());
    HOOT_STR_EQUALS("Invalid many to one REF2=002da0;todo", errorItr.value().toStdString());

    tags.clear();
    map->clear();
    tags.set(MetadataTags::Ref2(), "none;002da0");
    invalidRef2 = TestUtils::createNode(map, Status::Unknown2, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(invalidRef2->getElementId());
    HOOT_STR_EQUALS("Invalid many to one REF2=none;002da0", errorItr.value().toStdString());

    tags.clear();
    map->clear();
    tags.set(MetadataTags::Review(), "002da0;todo");
    ConstNodePtr invalidReview = TestUtils::createNode(map, Status::Unknown2, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(invalidReview->getElementId());
    HOOT_STR_EQUALS("Invalid many to one REVIEW=002da0;todo", errorItr.value().toStdString());

    tags.clear();
    map->clear();
    tags.set(MetadataTags::Review(), "none;002da0");
    invalidReview = TestUtils::createNode(map, Status::Unknown2, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(invalidReview->getElementId());
    HOOT_STR_EQUALS("Invalid many to one REVIEW=none;002da0", errorItr.value().toStdString());
  }

  void runDuplicateIdTest()
  {
    // ref2/review can't have the same id duplicated within the same tag value

    OsmMapPtr map(new OsmMap());
    Tags tags;
    ManualMatchValidator uut;
    QMap<ElementId, QString>::const_iterator errorItr;

    tags.set(MetadataTags::Ref2(), "002da0;002da0");
    ConstNodePtr invalidRef2 = TestUtils::createNode(map, Status::Unknown2, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(invalidRef2->getElementId());
    HOOT_STR_EQUALS("Duplicate IDs found in REF2: 002da0", errorItr.value().toStdString());

    tags.clear();
    map->clear();
    tags.set(MetadataTags::Review(), "002da0;002da0");
    ConstNodePtr invalidReview = TestUtils::createNode(map, Status::Unknown2, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(invalidReview->getElementId());
    HOOT_STR_EQUALS("Duplicate IDs found in REVIEW: 002da0", errorItr.value().toStdString());
  }

  void runFullDebugOutputTest()
  {
    OsmMapPtr map(new OsmMap());
    Tags tags;
    ManualMatchValidator uut;
    QMap<ElementId, QString>::const_iterator errorItr;

    uut.setFullDebugOutput(true);

    tags.set(MetadataTags::Ref2(), "002da0;002da0");
    tags.set("blah", "bleh");
    ConstNodePtr invalidRef2 = TestUtils::createNode(map, Status::Unknown2, 0.0, 0.0, 15.0, tags);

    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(invalidRef2->getElementId());
    HOOT_STR_EQUALS(
      "Duplicate IDs found in REF2: 002da0; tags: blah = bleh\n",
      errorItr.value().toStdString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ManualMatchValidatorTest, "quick");

}
