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

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/ops/RemoveElementOp.h>
#include <hoot/core/visitors/RemoveInvalidReviewRelationsVisitor.h>
#include <hoot/core/conflate/ReviewMarker.h>

#include "../TestUtils.h"

namespace hoot
{
using namespace Tgs;

class RemoveInvalidReviewRelationsVisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(RemoveInvalidReviewRelationsVisitorTest);
  CPPUNIT_TEST(runInvalidMemberCountTest);
  CPPUNIT_TEST(runEmptyRelationNoMemberCountTagTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runInvalidMemberCountTest()
  {
    //add some nodes to a map
    OsmMap::resetCounters();
    shared_ptr<OsmMap> map(new OsmMap());
    ElementPtr n1(new Node(Status::Unknown1, 1, 0, 0, 0));
    ElementPtr n2(new Node(Status::Unknown2, 2, 0, 0, 0));
    ElementPtr n3(new Node(Status::Unknown1, 3, 0, 0, 0));
    ElementPtr n4(new Node(Status::Unknown2, 4, 0, 0, 0));
    map->addElement(n1);
    map->addElement(n2);
    map->addElement(n3);
    map->addElement(n4);

    //create two reviews involving the two pairs of nodes
    ReviewMarker reviewMarker;
    reviewMarker.mark(map, n1, n2, "note 1", "test 1");
    reviewMarker.mark(map, n3, n4, "note 2", "test 2");
    CPPUNIT_ASSERT_EQUAL((size_t)2, map->getRelationMap().size());
    CPPUNIT_ASSERT(reviewMarker.isNeedsReview(map, n1, n2));
    CPPUNIT_ASSERT(reviewMarker.isNeedsReview(map, n3, n4));

    //get the review relation id's
    set<ElementId> review1 = reviewMarker._getReviewRelations(map, n1->getElementId());
    CPPUNIT_ASSERT_EQUAL((size_t)1, review1.size());
    const ElementId r1Id = *review1.begin()++;
    set<ElementId> review2 = reviewMarker._getReviewRelations(map, n3->getElementId());
    CPPUNIT_ASSERT_EQUAL((size_t)1, review2.size());
    const ElementId r2Id = *review2.begin()++;
    //RelationPtr relation1 = map->getRelation(r1Id.getId());
    RelationPtr relation2 = map->getRelation(r2Id.getId());

    //remove all of one of the review relation's members
    RemoveElementOp::removeElement(map, n3->getElementId());
    RemoveElementOp::removeElement(map, n4->getElementId());
    relation2->removeElement(n3->getElementId());
    relation2->removeElement(n4->getElementId());

    //run the visitor
    RemoveInvalidReviewRelationsVisitor v;
    map->visitRw(v);

    //the empty review relation should have been removed
    CPPUNIT_ASSERT_EQUAL((size_t)1, map->getRelationMap().size());
    CPPUNIT_ASSERT(map->containsElement(r1Id));
    CPPUNIT_ASSERT(!map->containsElement(r2Id));
  }

  void runEmptyRelationNoMemberCountTagTest()
  {
    //add some nodes to a map
    OsmMap::resetCounters();
    shared_ptr<OsmMap> map(new OsmMap());
    ElementPtr n1(new Node(Status::Unknown1, 1, 0, 0, 0));
    ElementPtr n2(new Node(Status::Unknown2, 2, 0, 0, 0));
    ElementPtr n3(new Node(Status::Unknown1, 3, 0, 0, 0));
    ElementPtr n4(new Node(Status::Unknown2, 4, 0, 0, 0));
    map->addElement(n1);
    map->addElement(n2);
    map->addElement(n3);
    map->addElement(n4);

    //create two reviews involving the two pairs of nodes
    ReviewMarker reviewMarker;
    reviewMarker.mark(map, n1, n2, "note 1", "test 1");
    reviewMarker.mark(map, n3, n4, "note 2", "test 2");
    CPPUNIT_ASSERT_EQUAL((size_t)2, map->getRelationMap().size());
    CPPUNIT_ASSERT(reviewMarker.isNeedsReview(map, n1, n2));
    CPPUNIT_ASSERT(reviewMarker.isNeedsReview(map, n3, n4));

    //get the review relations
    set<ElementId> review1 = reviewMarker._getReviewRelations(map, n1->getElementId());
    CPPUNIT_ASSERT_EQUAL((size_t)1, review1.size());
    const ElementId r1Id = *review1.begin()++;
    set<ElementId> review2 = reviewMarker._getReviewRelations(map, n3->getElementId());
    CPPUNIT_ASSERT_EQUAL((size_t)1, review2.size());
    const ElementId r2Id = *review2.begin()++;
    RelationPtr relation1 = map->getRelation(r1Id.getId());
    RelationPtr relation2 = map->getRelation(r2Id.getId());

    //go ahead and remove their review member count tags
    relation1->getTags().remove(ReviewMarker::reviewMemberCountKey);
    relation2->getTags().remove(ReviewMarker::reviewMemberCountKey);

    //remove all of one of the review relation's members
    RemoveElementOp::removeElement(map, n3->getElementId());
    RemoveElementOp::removeElement(map, n4->getElementId());
    relation2->removeElement(n3->getElementId());
    relation2->removeElement(n4->getElementId());

    //run the visitor
    RemoveInvalidReviewRelationsVisitor v;
    map->visitRw(v);

    //the empty review relation should have been removed
    CPPUNIT_ASSERT_EQUAL((size_t)1, map->getRelationMap().size());
    CPPUNIT_ASSERT(map->containsElement(r1Id));
    CPPUNIT_ASSERT(!map->containsElement(r2Id));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RemoveInvalidReviewRelationsVisitorTest, "quick");

}


