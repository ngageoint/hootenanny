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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/ops/ManualMatchValidator.h>

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
  CPPUNIT_TEST(runStatusTest);
  CPPUNIT_TEST(runEmptyIdTest);
  CPPUNIT_TEST(runInvalidIdComboTest);
  CPPUNIT_TEST(runMissingRef1Test);
  CPPUNIT_TEST(runInvalidIdTest);
  CPPUNIT_TEST(runRepeatedIdTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ManualMatchValidatorTest() :
  HootTestFixture("test-files/ops/ManualMatchValidatorTest/", UNUSED_PATH)
  {
    setResetType(ResetBasic);
  }

  void runEmptyIdTest()
  {
    // empty manual match ids aren't allowed

    OsmMapPtr map(new OsmMap());

    Tags tags1;
    tags1.set(MetadataTags::Ref1(), "");
    ConstNodePtr node = TestUtils::createNode(map, 0.0, 0.0, 15.0, tags1);

    Tags tags2;
    tags2.set(MetadataTags::Ref2(), "");
    geos::geom::Coordinate c1[] = {
      geos::geom::Coordinate(0.0, 0.0), geos::geom::Coordinate(1.0, 1.0),
      geos::geom::Coordinate::getNull() };
    ConstWayPtr way = TestUtils::createWay(map, c1, Status::Unknown1, 15.0, tags2);

    Tags tags3;
    tags3.set(MetadataTags::Review(), "");
    QList<ElementPtr> relationMembers;
    relationMembers.append(node);
    relationMembers.append(way);
    ConstRelationPtr relation =
      TestUtils::createRelation(map, relationMembers, Status::Unknown1, 15.0, tags3);

    ManualMatchValidator uut;
    uut.apply(map);

    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 3);
    QMap<ElementId, QString>::const_iterator errorItr =
      uut.getErrors().find(node->getElementId());
    CPPUNIT_ASSERT_EQUAL("", errorItr.value());
    errorItr = uut.getErrors().find(way->getElementId());
    CPPUNIT_ASSERT_EQUAL("", errorItr.value());
    errorItr = uut.getErrors().find(relation->getElementId());
    CPPUNIT_ASSERT_EQUAL("", errorItr.value());
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
    ConstNodePtr node = TestUtils::createNode(map, 0.0, 0.0, 15.0, tags);
    uut.apply(map);
    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(node->getElementId());
    CPPUNIT_ASSERT_EQUAL("", errorItr.value());

    node->getTags().clear();
    node->getTags().set(MetadataTags::Ref1(), "002c75");
    node->getTags().set(MetadataTags::Review(), "002da0");
    uut.apply(map);
    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(node->getElementId());
    CPPUNIT_ASSERT_EQUAL("", errorItr.value());
  }

  void runMissingRef1Test()
  {
    // all ref2/review manual match ids have to have a corresponding ref1 id

    OsmMapPtr map(new OsmMap());
    Tags tags;
    ManualMatchValidator uut;
    QMap<ElementId, QString>::const_iterator errorItr;

    tags.set(MetadataTags::Ref1(), "002c75");
    ConstNodePtr ref1 = TestUtils::createNode(map, 0.0, 0.0, 15.0, tags);
    ref1->setStatus(Status::Unknown1);

    tags.clear();
    tags.set(MetadataTags::Ref2(), "002da0");
    ConstNodePtr ref2 = TestUtils::createNode(map, 0.0, 0.0, 15.0, tags);
    ref2->setStatus(Status::Unknown2);

    uut.apply(map);
    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(ref2->getElementId());
    CPPUNIT_ASSERT_EQUAL("", errorItr.value());

    tags.clear();
    tags.set(MetadataTags::Review(), "002da0");
    ConstNodePtr review = TestUtils::createNode(map, 0.0, 0.0, 15.0, tags);
    review->setStatus(Status::Unknown2);

    uut.apply(map);
    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(review->getElementId());
    CPPUNIT_ASSERT_EQUAL("", errorItr.value());
  }

  void runInvalidIdTest()
  {
    OsmMapPtr map(new OsmMap());
    Tags tags;
    ManualMatchValidator uut;
    QMap<ElementId, QString>::const_iterator errorItr;

    // ref1 ids all must be hex
    tags.set(MetadataTags::Ref1(), "todo");
    ConstNodePtr ref1 = TestUtils::createNode(map, 0.0, 0.0, 15.0, tags);
    ref1->setStatus(Status::Unknown1);

    uut.apply(map);
    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(ref1->getElementId());
    CPPUNIT_ASSERT_EQUAL("", errorItr.value());

    tags.clear();
    tags.set(MetadataTags::Ref1(), "none");
    ConstNodePtr ref1 = TestUtils::createNode(map, 0.0, 0.0, 15.0, tags);
    ref1->setStatus(Status::Unknown1);

    uut.apply(map);
    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(ref1->getElementId());
    CPPUNIT_ASSERT_EQUAL("", errorItr.value());

    tags.clear();
    tags.set(MetadataTags::Ref1(), "-");
    ConstNodePtr ref1 = TestUtils::createNode(map, 0.0, 0.0, 15.0, tags);
    ref1->setStatus(Status::Unknown1);

    uut.apply(map);
    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(ref1->getElementId());
    CPPUNIT_ASSERT_EQUAL("", errorItr.value());

    // ref2/review ids must all be either hex, 'todo', or 'none'

    tags.clear();
    tags.set(MetadataTags::Ref2(), "-");
    ConstNodePtr ref1 = TestUtils::createNode(map, 0.0, 0.0, 15.0, tags);
    ref2->setStatus(Status::Unknown2);

    uut.apply(map);
    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(ref2->getElementId());
    CPPUNIT_ASSERT_EQUAL("", errorItr.value());
  }

  void runRepeatedIdTest()
  {
    // a ref2 and a review can't have the same id on the same element

    OsmMapPtr map(new OsmMap());
    Tags tags;
    ManualMatchValidator uut;
    QMap<ElementId, QString>::const_iterator errorItr;

    tags.set(MetadataTags::Ref2(), "002da0");
    tags.set(MetadataTags::Review(), "002da0");
    ConstNodePtr ref = TestUtils::createNode(map, 0.0, 0.0, 15.0, tags);
    ref->setStatus(Status::Unknown2);

    uut.apply(map);
    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    errorItr = uut.getErrors().find(ref->getElementId());
    CPPUNIT_ASSERT_EQUAL("", errorItr.value());
  }

  void runStatusTest()
  {
    // This test makes sure data loaded with an Unknown1 status doesn't have any REF2 tags in it and
    // that data loaded with an Unknown2 status doesn't have any REF1 tags in it.

    OsmXmlReader reader;
    ManualMatchValidator uut;

    OsmMapPtr map1(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "has-ref1-tags-only.osm", map1);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(_inputPath + "has-ref2-tags-only.osm", map1);
    uut.apply(map1);
    CPPUNIT_ASSERT(!uut.hasErrors());

    OsmMapPtr map2(new OsmMap());
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(_inputPath + "has-ref1-tags-only.osm", map2);
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "has-ref2-tags-only.osm", map2);
    uut.apply(map2);
    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    QMap<ElementId, QString>::const_iterator errorItr =
      uut.getErrors().find(ElementId(ElementType::Way, 1));
    CPPUNIT_ASSERT_EQUAL("", errorItr.value());

    OsmMapPtr map3(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "has-ref1-tags-only.osm", map3);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(_inputPath + "has-ref1-tags-only.osm", map3);
    uut.apply(map3);
    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    QMap<ElementId, QString>::const_iterator errorItr =
      uut.getErrors().find(ElementId(ElementType::Way, 1));
    CPPUNIT_ASSERT_EQUAL("", errorItr.value());

    OsmMapPtr map4(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "has-ref2-tags-only.osm", map4);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(_inputPath + "has-ref2-tags-only.osm", map4);
    uut.apply(map4);
    CPPUNIT_ASSERT(uut.hasErrors());
    CPPUNIT_ASSERT(uut.getErrors().size() == 1);
    QMap<ElementId, QString>::const_iterator errorItr =
      uut.getErrors().find(ElementId(ElementType::Way, 1));
    CPPUNIT_ASSERT_EQUAL("", errorItr.value());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ManualMatchValidatorTest, "quick");

}
