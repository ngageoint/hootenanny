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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmPgCsvWriter.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/util/Log.h>

//  Geos
#include <geos/geom/Coordinate.h>

using namespace geos::geom;

namespace hoot
{

class OsmPgCsvWriterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmPgCsvWriterTest);
  CPPUNIT_TEST(runBasic);
  CPPUNIT_TEST(runString);
  CPPUNIT_TEST(runEscapeTags);
  CPPUNIT_TEST_SUITE_END();

public:

  OsmPgCsvWriterTest()
    : HootTestFixture("test-files/io/PgCsvWriter/",
                      "test-output/io/PgCsvWriter/")
  {
    setResetType(ResetBasic);
  }

  void runBasic()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ScoreMatchRef1.osm", map);

    OsmPgCsvWriter writer;
    writer.open(_outputPath + "runBasic.pgcsv");
    writer.write(map);
    writer.close();

    //  Check the results
    HOOT_FILE_EQUALS(_inputPath + "runBasic-nodes.pgcsv",
                     _outputPath + "runBasic-nodes.pgcsv");
    HOOT_FILE_EQUALS(_inputPath + "runBasic-ways.pgcsv",
                     _outputPath + "runBasic-ways.pgcsv");
    HOOT_FILE_EQUALS(_inputPath + "runBasic-waynodes.pgcsv",
                     _outputPath + "runBasic-waynodes.pgcsv");
    HOOT_FILE_EQUALS(_inputPath + "runBasic-relations.pgcsv",
                     _outputPath + "runBasic-relations.pgcsv");
    HOOT_FILE_EQUALS(_inputPath + "runBasic-relationmembers.pgcsv",
                     _outputPath + "runBasic-relationmembers.pgcsv");
  }

  void runString()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/UndividedHighwayPreSplit.osm", map);

    QString output = OsmPgCsvWriter::toString(map);
    //  Compare the results
    HOOT_STR_EQUALS(expected_runString, output);
  }

  void runEscapeTags()
  {
    OsmMapPtr map(new OsmMap());
    Coordinate coords[] = { Coordinate(0, 0), Coordinate(0, 1), Coordinate(1, 1), Coordinate(1, 0), Coordinate::getNull() };
    Tags tags;
    tags.set("note", "<2>");
    tags.set("aerialway", "t-bar");
    tags.set("first name", "first name goes here");
    tags.set("full_name", "\"Hacksaw\" Jim Duggan");
    WayPtr way = TestUtils::createWay(map, Status::Unknown1, coords);
    way->setTags(tags);

    QList<ElementPtr> nodes;
    NodePtr node1(new Node(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.1), 15));
    node1->getTags().appendValue("name", "test1");
    nodes.append(node1);

    NodePtr node2(new Node(Status::Unknown1, map->createNextNodeId(), Coordinate(0.1, 0.0), 15));
    node2->getTags().appendValue("name", "test2");
    nodes.append(node2);

    RelationPtr relation = TestUtils::createRelation(map, nodes);
    relation->setType("review");
    relation->getTags().appendValue("name", "Test Review");
    std::vector<RelationData::Entry> members = relation->getMembers();
    members[0].role = "reviewee";
    members[1].role = "reviewee";
    relation->setMembers(members);

    QString output = OsmPgCsvWriter::toString(map);
    //  Compare the results
    HOOT_STR_EQUALS(expected_runEscapeTags, output);
  }

  QString expected_runString =
      "node_id,latitude,longitude,changeset_id,visible,timestamp,version,tags\n"
      "-24,38.8922829731610307,-104.7571242107038501,0,t,,0,\n"
      "-23,38.8921263906740933,-104.7570871002226340,0,t,,0,\n"
      "-22,38.8909086840196778,-104.7569933474280219,0,t,,0,\n"
      "-21,38.8907414567564302,-104.7569953006112513,0,t,,0,\n"
      "-20,38.8905100588100012,-104.7552970957371627,0,t,,0,\n"
      "-19,38.8906218568842945,-104.7554296822108739,0,t,,0,\n"
      "-18,38.8904004105277679,-104.7559406925781644,0,t,,0,\n"
      "-17,38.8902778622473306,-104.7566367715649847,0,t,,0,\n"
      "-16,38.8902198465279980,-104.7570020119788126,0,t,,0,\n"
      "-15,38.8901811134555260,-104.7572472217875656,0,t,,0,\n"
      "-14,38.8900306150730160,-104.7580095940112273,0,t,,0,\n"
      "-13,38.8908004699918735,-104.7586007087063962,0,t,,0,\n"
      "-12,38.8908255427122072,-104.7573050338473735,0,t,,0,\n"
      "-11,38.8908255427121929,-104.7569936423757184,0,t,,0,\n"
      "-10,38.8908311144266534,-104.7565891913838385,0,t,,0,\n"
      "-9,38.8908450437108044,-104.7560916808716485,0,t,,0,\n"
      "-8,38.8908951891112054,-104.7555619574486201,0,t,,0,\n"
      "-7,38.8918145151778489,-104.7570437513481778,0,t,,0,\n"
      "-6,38.8914495720119433,-104.7570008008003555,0,t,,0,\n"
      "-5,38.8911188362054361,-104.7569942966839562,0,t,,0,\n"
      "-4,38.8904717379506977,-104.7570008008003555,0,t,,0,\n"
      "-3,38.8898142693043027,-104.7570115384373111,0,t,,0,\n"
      "-2,38.8908278806533190,-104.7568782149722182,0,t,,0,\n"
      "-1,38.8908235807431026,-104.7571019546465294,0,t,,0,\n"
      "\n"
      "way_id,changeset_id,timestamp,version,visible,tags\n"
      "-7,0,,0,t,highway=>primary,note=>2\n"
      "-6,0,,0,t,highway=>unclassified\n"
      "-5,0,,0,t,highway=>primary\n"
      "-4,0,,0,t,highway=>unclassified\n"
      "-3,0,,0,t,highway=>unclassified\n"
      "-2,0,,0,t,highway=>primary\n"
      "-1,0,,0,t,highway=>primary\n"
      "\n"
      "way_id,node_id,version,sequence_id\n"
      "-7,-7,0,1\n"
      "-7,-6,0,2\n"
      "-6,-8,0,1\n"
      "-6,-19,0,2\n"
      "-6,-20,0,3\n"
      "-5,-6,0,1\n"
      "-5,-5,0,2\n"
      "-5,-22,0,3\n"
      "-5,-11,0,4\n"
      "-5,-21,0,5\n"
      "-5,-4,0,6\n"
      "-5,-16,0,7\n"
      "-5,-3,0,8\n"
      "-4,-16,0,1\n"
      "-4,-17,0,2\n"
      "-4,-18,0,3\n"
      "-3,-14,0,1\n"
      "-3,-15,0,2\n"
      "-3,-16,0,3\n"
      "-2,-13,0,1\n"
      "-2,-12,0,2\n"
      "-2,-1,0,3\n"
      "-2,-11,0,4\n"
      "-2,-2,0,5\n"
      "-2,-10,0,6\n"
      "-2,-9,0,7\n"
      "-2,-8,0,8\n"
      "-1,-24,0,1\n"
      "-1,-23,0,2\n"
      "-1,-7,0,3\n"
      "\n"
      "relation_id,changeset_id,timestamp,version,visible,tags\n"
      "\n"
      "relation_id,member_type,member_id,member_role,version,sequence_id\n";

  QString expected_runEscapeTags =
      "node_id,latitude,longitude,changeset_id,visible,timestamp,version,tags\n"
      "-6,0.0000000000000000,0.1000000000000000,0,t,,0,name=>test2\n"
      "-5,0.1000000000000000,0.0000000000000000,0,t,,0,name=>test1\n"
      "-4,0.0000000000000000,1.0000000000000000,0,t,,0,\n"
      "-3,1.0000000000000000,1.0000000000000000,0,t,,0,\n"
      "-2,1.0000000000000000,0.0000000000000000,0,t,,0,\n"
      "-1,0.0000000000000000,0.0000000000000000,0,t,,0,\n"
      "\n"
      "way_id,changeset_id,timestamp,version,visible,tags\n"
      "-1,0,,0,t,note=>\"<2>\",\"first name\"=>\"first name goes here\",aerialway=>\"t-bar\",full_name=>\"\\\"Hacksaw\\\" Jim Duggan\"\n"
      "\n"
      "way_id,node_id,version,sequence_id\n"
      "-1,-1,0,1\n"
      "-1,-2,0,2\n"
      "-1,-3,0,3\n"
      "-1,-4,0,4\n"
      "\n"
      "relation_id,changeset_id,timestamp,version,visible,tags\n"
      "-1,0,,0,t,name=>\"Test Review\"\n"
      "\n"
      "relation_id,member_type,member_id,member_role,version,sequence_id\n"
      "-1,Node,-5,reviewee,0,1\n"
      "-1,Node,-6,reviewee,0,2\n";
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmPgCsvWriterTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmPgCsvWriterTest, "current");

}
