/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Pretty Pipes
#include <pp/HadoopPipesUtils.h>
#include <pp/Hdfs.h>
#include <pp/mapreduce/Job.h>
using namespace pp;

// Standard
#include <iostream>
#include <stdlib.h>

#include <hoot/hadoop/pbf/PbfRecordWriter.h>

using namespace std;

namespace hoot
{

class PbfRecordWriterTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PbfRecordWriterTest);
  CPPUNIT_TEST(testBasics);
  CPPUNIT_TEST_SUITE_END();

public:

  void testBasics()
  {
    PbfRecordWriter uut;
    // makes for a consistent output.
    uut.setCompressionLevel(0);
    uut.includeVersion(false);

    Hdfs fs;
    if (fs.exists("test-output/PbfRecordWriterTest"))
    {
      fs.deletePath("test-output/PbfRecordWriterTest", true);
    }

    uut.setReduceContext(2, "test-output/PbfRecordWriterTest");
    OsmMapPtr map(new OsmMap());
    NodePtr n(new Node(Status::Unknown1, 72, 42.0, 3.14159, 7.1));
    n->setTag("hello", "world");
    n->setTag("note", "test tag");
    map->addNode(n);
    WayPtr w(new Way(Status::Unknown1, 42, 1.7));
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

    uut.emitRecord(map);

    map->clear();

    n.reset(new Node(Status::Unknown1, 73, 20, 30, 15));
    map->addNode(n);
    w.reset(new Way(Status::Unknown1, 43, 1.7));
    nodes.clear();
    nodes.push_back(73);
    w->addNodes(nodes);
    map->addWay(w);

    uut.emitRecord(map);

    uut.close();

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

    boost::shared_ptr<istream> strm(fs.open("test-output/PbfRecordWriterTest/part-00002r.pbf"));
    unsigned char* buffer = new unsigned char[dataSize * 2];
    strm->read((char*)buffer, dataSize * 2);
    CPPUNIT_ASSERT_EQUAL(dataSize, (size_t)strm->gcount());

    for (size_t i = 0; i < dataSize; i++)
    {
      CPPUNIT_ASSERT_EQUAL((int)(unsigned char)data[i], (int)(unsigned char)buffer[i]);
    }

  }
};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::PbfRecordWriterTest, "glacial");

