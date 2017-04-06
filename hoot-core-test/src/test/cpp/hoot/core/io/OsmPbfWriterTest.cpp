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
 * @copyright Copyright (C) 2012, 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmPbfWriter.h>

// Tgs
#include <tgs/Statistics/Random.h>

using namespace hoot;
using namespace hoot::pb;

// Standard
#include <sstream>
using namespace std;

#include "../TestUtils.h"

namespace hoot
{

class OsmPbfWriterTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(OsmPbfWriterTest);
  CPPUNIT_TEST(runToyTest);
  CPPUNIT_TEST(runWriteNodeTest);
  CPPUNIT_TEST(runWriteRelationTest);
  CPPUNIT_TEST(runWriteWayTest);
  CPPUNIT_TEST(runWriteIncrementalTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runToyTest()
  {
    OsmXmlReader reader;

    boost::shared_ptr<OsmMap> map(new OsmMap());
    reader.read("test-files/ToyTestA.osm", map);

    OsmPbfWriter writer;
    writer.write(map, "test-output/io/OsmPbfWriterTest.pbf");

    HOOT_FILE_EQUALS("test-files/io/OsmPbfWriterTest.pbf",
                     "test-output/io/OsmPbfWriterTest.pbf");

  }

  void _dumpString(const string& str)
  {
    cout << "const unsigned char data[] = {";
    for (size_t i = 0; i < str.size(); i++)
    {
      if (i != 0)
      {
        cout << ", ";
      }
      if (i % 18 == 0)
      {
        cout << endl << "  ";
      }
      //printf("%3d%c", (unsigned char)str.at(i),
      //  str.at(i) >= 32 && str.at(i) <= 125 ? (char)(str.at(i)) : '#');
      printf("%3d", (unsigned char)str.at(i));
    }
    cout << "};" << endl;
    cout << "size_t dataSize = " << str.size() << ";" << endl;
  }

  void runWriteNodeTest()
  {
    stringstream ss(stringstream::out);

    OsmPbfWriter writer;

    boost::shared_ptr<Node> n(new Node(Status::Unknown1, 72, 42.0, 3.14159, 7.1));
    n->setTag("hello", "world");
    n->setTag("note", "test tag");
    writer.writePb(n, &ss);

    //_dumpString(ss.str());

    const unsigned char data[] = {
        0,   0,   0, 101,  10,  69,  10,   0,  10,   4, 110, 111, 116, 101,  10,   8, 116, 101,
      115, 116,  32, 116,  97, 103,  10,   5, 104, 101, 108, 108, 111,  10,   5, 119, 111, 114,
      108, 100,  10,  14, 101, 114, 114, 111, 114,  58,  99, 105, 114,  99, 117, 108,  97, 114,
       10,   3,  55,  46,  49,  10,  11, 104, 111, 111, 116,  58, 115, 116,  97, 116, 117, 115,
       10,   1,  49,  18,  28,  10,  26,   8, 144,   1,  18,   4,   1,   3,   5,   7,  26,   4,
        2,   4,   6,   8,  64, 184, 249, 250,  29,  72, 128, 196, 197, 144,   3};
    size_t dataSize = 105;

    CPPUNIT_ASSERT_EQUAL(dataSize, ss.str().size());
    for (size_t i = 0; i < dataSize; i++)
    {
      CPPUNIT_ASSERT_EQUAL(data[i], (unsigned char)ss.str().at(i));
    }
  }

  void runWriteRelationTest()
  {
    stringstream ss(stringstream::out);

    OsmPbfWriter writer;

    boost::shared_ptr<OsmMap> map(new OsmMap());
    boost::shared_ptr<Relation> r(new Relation(Status::Unknown1, 42, 1.7, "foo"));
    r->addElement("s", ElementId::node(1));
    r->addElement("t", ElementId::node(2));
    r->addElement("u", ElementId::node(3));
    r->addElement("f", ElementId::way(1));
    r->addElement("f", ElementId::relation(1));
    r->setTag("hello", "world");
    r->setTag("highway", "road");
    r->setTag("note", "test tag");
    writer.writePb(r, &ss);

    //_dumpString(ss.str());

    const unsigned char data[] = {
        0,   0,   0, 139,  10,  96,  10,   0,  10,   1, 115,  10,   1, 116,  10,   1, 117,  10,
        1, 102,  10,   4, 110, 111, 116, 101,  10,   8, 116, 101, 115, 116,  32, 116,  97, 103,
       10,   5, 104, 101, 108, 108, 111,  10,   5, 119, 111, 114, 108, 100,  10,   7, 104, 105,
      103, 104, 119,  97, 121,  10,   4, 114, 111,  97, 100,  10,  14, 101, 114, 114, 111, 114,
       58,  99, 105, 114,  99, 117, 108,  97, 114,  10,   3,  49,  46,  55,  10,  11, 104, 111,
      111, 116,  58, 115, 116,  97, 116, 117, 115,  10,   1,  49,  18,  39,  34,  37,   8,  42,
       18,   5,   5,   7,   9,  11,  13,  26,   5,   6,   8,  10,  12,  14,  66,   5,   1,   2,
        3,   4,   4,  74,   5,   2,   2,   2,   3,   0,  82,   5,   0,   0,   0,   1,   2};
    size_t dataSize = 143;

    CPPUNIT_ASSERT_EQUAL(dataSize, ss.str().size());
    for (size_t i = 0; i < dataSize; i++)
    {
      CPPUNIT_ASSERT_EQUAL(data[i], (unsigned char)ss.str().at(i));
    }
  }

  void runWriteWayTest()
  {
    stringstream ss(stringstream::out);

    OsmPbfWriter writer;

    boost::shared_ptr<OsmMap> map(new OsmMap());
    boost::shared_ptr<Way> w(new Way(Status::Unknown1, 42, 1.7));
    vector<long> nodes;
    nodes.push_back(1);
    nodes.push_back(3);
    nodes.push_back(5);
    nodes.push_back(7);
    nodes.push_back(11);
    w->addNodes(nodes);
    w->setTag("hello", "world");
    w->setTag("highway", "road");
    w->setTag("note", "test tag");
    writer.writePb(w, &ss);

    //_dumpString(ss.str());

    const unsigned char data[] = {
        0,   0,   0, 113,  10,  84,  10,   0,  10,   4, 110, 111, 116, 101,  10,   8, 116, 101,
      115, 116,  32, 116,  97, 103,  10,   5, 104, 101, 108, 108, 111,  10,   5, 119, 111, 114,
      108, 100,  10,   7, 104, 105, 103, 104, 119,  97, 121,  10,   4, 114, 111,  97, 100,  10,
       14, 101, 114, 114, 111, 114,  58,  99, 105, 114,  99, 117, 108,  97, 114,  10,   3,  49,
       46,  55,  10,  11, 104, 111, 111, 116,  58, 115, 116,  97, 116, 117, 115,  10,   1,  49,
       18,  25,  26,  23,   8,  42,  18,   5,   1,   3,   5,   7,   9,  26,   5,   2,   4,   6,
        8,  10,  66,   5,   2,   4,   4,   4,   8};
    size_t dataSize = 117;

    CPPUNIT_ASSERT_EQUAL(dataSize, ss.str().size());
    for (size_t i = 0; i < dataSize; i++)
    {
      CPPUNIT_ASSERT_EQUAL(data[i], (unsigned char)ss.str().at(i));
    }
  }

  void runWriteIncrementalTest()
  {
    stringstream ss(stringstream::out);

    OsmPbfWriter writer;
    // makes for a consistent output.
    writer.setCompressionLevel(0);
    writer.includVersion(false);

    OsmMap::resetCounters();
    Tgs::Random::instance()->seed(0);

    boost::shared_ptr<OsmMap> map(new OsmMap());
    boost::shared_ptr<Node> n(new Node(Status::Unknown1, 72, 42.0, 3.14159, 7.1));
    n->setTag("hello", "world");
    n->setTag("note", "test tag");
    map->addNode(n);
    boost::shared_ptr<Way> w(new Way(Status::Unknown1, 42, 1.7));
    vector<long> nodes;
    nodes.push_back(1);
    nodes.push_back(3);
    nodes.push_back(5);
    nodes.push_back(7);
    nodes.push_back(11);
    w->addNodes(nodes);
    w->setTag("hello", "world");
    w->setTag("highway", "road");
    w->setTag("note", "test tag");
    map->addWay(w);
    writer.intializePartial(&ss);
    writer.writePartial(map);

    map->clear();

    n.reset(new Node(Status::Unknown1, 73, 20, 30, 15));
    map->addNode(n);
    w.reset(new Way(Status::Unknown1, 43, 1.7));
    nodes.clear();
    nodes.push_back(73);
    w->addNodes(nodes);
    map->addWay(w);

    writer.writePartial(map);

    writer.finalizePartial();

    //_dumpString(ss.str());

    const unsigned char data[] = {
        0,   0,   0,  13,  10,   9,  79,  83,  77,  72, 101,  97, 100, 101, 114,  24,  56,  16,
       41,  26,  52, 120,   1,   1,  41,   0, 214, 255,  34,  14,  79, 115, 109,  83,  99, 104,
      101, 109,  97,  45,  86,  48,  46,  54,  34,  10,  68, 101, 110, 115, 101,  78, 111, 100,
      101, 115, 130,   1,  10,  72, 111, 111, 116, 101, 110,  97, 110, 110, 121,   9, 172,  13,
      140,   0,   0,   0,  12,  10,   7,  79,  83,  77,  68,  97, 116,  97,  24, 221,   1,  16,
      204,   1,  26, 215,   1, 120,   1,   1, 204,   0,  51, 255,  10,  89,  10,   0,  10,   4,
      110, 111, 116, 101,  10,   8, 116, 101, 115, 116,  32, 116,  97, 103,  10,   5, 104, 101,
      108, 108, 111,  10,   5, 119, 111, 114, 108, 100,  10,  14, 101, 114, 114, 111, 114,  58,
       99, 105, 114,  99, 117, 108,  97, 114,  10,   3,  55,  46,  49,  10,  11, 104, 111, 111,
      116,  58, 115, 116,  97, 116, 117, 115,  10,   1,  49,  10,   7, 104, 105, 103, 104, 119,
       97, 121,  10,   4, 114, 111,  97, 100,  10,   3,  49,  46,  55,  18,  42,  18,  40,  10,
        3, 144,   1,   2,  66,   9, 184, 249, 250,  29, 200, 146, 146, 128,   2,  74,  10, 128,
      196, 197, 144,   3, 255, 187, 231, 209,   1,  82,  10,   1,   2,   3,   4,   5,   6,   7,
        8,   0,   0,  18,  67,  26,  23,   8,  42,  18,   5,   1,   3,   9,   5,   7,  26,   5,
        2,   4,  10,  11,   8,  66,   5,   2,   4,   4,   4,   8,  26,  40,   8,  43,  18,   2,
        5,   7,  26,   2,  11,   8,  66,   2, 146,   1,  74,  24,   8, 128, 136, 222, 190,   1,
       16, 128, 136, 222, 190,   1,  24, 128, 140, 141, 158,   2,  32, 128, 140, 141, 158,   2,
      109,  93,  53, 122};
    size_t dataSize = 310;

    CPPUNIT_ASSERT_EQUAL(dataSize, ss.str().size());
    for (size_t i = 0; i < dataSize; i++)
    {
      CPPUNIT_ASSERT_EQUAL((unsigned int)data[i], (unsigned int)(unsigned char)ss.str().at(i));
    }
  }

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmPbfWriterTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmPbfWriterTest, "quick");

}
