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
 * @copyright Copyright (C) 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/util/Log.h>
#include <hoot/rnd/conflate/multiary/MultiaryReviewCommand.h>

using namespace std;

namespace hoot
{

class MultiaryReviewCommandTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MultiaryReviewCommandTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST(hashTest);
  CPPUNIT_TEST_SUITE_END();

public:

  MultiaryReviewCommandTest()
  {
    setResetType(ResetAll);
  }

  OsmMapPtr getTestMap()
  {
    QString testJsonStr =
      "{                                      \n"
      " 'elements': [                         \n"
      " { 'type': 'node', 'id': -1, 'lat': 2.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'James Bar' } },\n"
      " { 'type': 'node', 'id': -2, 'lat': 2.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'James\\'s Bar' } },\n"
      " { 'type': 'node', 'id': -3, 'lat': 2.0000001, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'studio', 'name': 'Jamie\\'s Barre' } },\n"
      " { 'type': 'node', 'id': -4, 'lat': 2.00000101, 'lon': -3.0, \n"
      "   'tags': { 'place': 'locality', 'name': 'James Town' } },\n"
      " { 'type': 'node', 'id': -5, 'lat': 2.0, 'lon': -3.0, \n"
      "   'tags': { 'military': 'yes', 'name': 'James Base' } }\n"
      "]                                      \n"
      "}                                      \n";

    OsmMapPtr pMap(new OsmMap());
    OsmJsonReader().loadFromString(testJsonStr, pMap);
    return pMap;
  }

  /**
   * @brief basicTest creates a few review commands, exports them to strings, then recreates them
   * from those strings again.
   */
  void basicTest()
  {
    OsmMapPtr pMap = getTestMap();

    MultiaryReviewCommand missCmd, matchCmd, modifyCmd, deleteCmd;

    // Match
    matchCmd.setOp(MultiaryReviewCommand::Match);
    matchCmd.addElementHash(pMap->getElement(ElementType::Node, -1));
    matchCmd.addElementHash(pMap->getElement(ElementType::Node, -2));
    NodePtr pMatchNode(new Node(Status::Unknown1, -1, geos::geom::Coordinate(-3.0, 2.0), 15.0));
    pMatchNode->getTags().appendValue("name", "James Bar");
    pMatchNode->getTags().appendValue("amenity", "pub");
    matchCmd.setFinalElement(pMatchNode);

    // Miss
    missCmd.setOp(MultiaryReviewCommand::Miss);
    missCmd.addElementHash(pMap->getElement(ElementType::Node, -2));
    missCmd.addElementHash(pMap->getElement(ElementType::Node, -3));

    // Modify
    modifyCmd.setOp(MultiaryReviewCommand::Modify);
    modifyCmd.addElementHash(pMap->getElement(ElementType::Node, -4));
    NodePtr pModNode(new Node(Status::Unknown1, -4, geos::geom::Coordinate(-3.0, 2.1), 15.0));
    pModNode->getTags().appendValue("name", "James\\'s Town");
    pModNode->getTags().appendValue("place", "locality");
    modifyCmd.setFinalElement(pModNode);

    // Delete
    deleteCmd.setOp(MultiaryReviewCommand::Delete);
    deleteCmd.addElementHash(pMap->getElement(ElementType::Node, -5));\

    // Convert to JSON strings and back
    QString jsonStr = matchCmd.toJsonString();
    MultiaryReviewCommand testMatch = MultiaryReviewCommand::fromJsonString(jsonStr);
    CPPUNIT_ASSERT(matchCmd == testMatch);

    jsonStr = missCmd.toJsonString();
    MultiaryReviewCommand testMiss = MultiaryReviewCommand::fromJsonString(jsonStr);
    CPPUNIT_ASSERT(missCmd == testMiss);

    jsonStr = modifyCmd.toJsonString();
    MultiaryReviewCommand testModify = MultiaryReviewCommand::fromJsonString(jsonStr);
    CPPUNIT_ASSERT(modifyCmd == testModify);

    jsonStr = deleteCmd.toJsonString();
    MultiaryReviewCommand testDelete = MultiaryReviewCommand::fromJsonString(jsonStr);
    CPPUNIT_ASSERT(deleteCmd == testDelete);
  }

  /**
   * @brief hashTest checks to make sure we are handling byte-array-hashes and hashes-as-strings
   * and converting to/from hex representations appropriately.
   */
  void hashTest()
  {
    OsmMapPtr pMap = getTestMap();

    MultiaryReviewCommand matchCmd;

    matchCmd.setOp(MultiaryReviewCommand::Match);
    matchCmd.addElementHash(pMap->getElement(ElementType::Node, -1));
    matchCmd.addElementHash(pMap->getElement(ElementType::Node, -2));

    QByteArray hashBytes = *(matchCmd.getElementHashes().begin());
    QString hashString = pMap->getElement(ElementType::Node, -1)->getTags()[MetadataTags::HootHash()];
    hashString.replace("sha1sum:", "");

    CPPUNIT_ASSERT(hashString == QString(hashBytes.toHex()));
  }

  // TODO: Create a test that applies commands to a map

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MultiaryReviewCommandTest, "glacial");

}
