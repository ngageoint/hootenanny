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

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/hadoop/way-join/WayJoin2InputFormat.h>
#include <hoot/hadoop/way-join/WayJoin1Reducer.h>

// Pretty Pipes
#include <pp/Hdfs.h>
#include <pp/mapreduce/Job.h>
#include <pp/io/CppSeqFile.h>
using namespace pp;

// Standard
#include <iostream>
#include <stdlib.h>

using namespace std;

namespace hoot
{

class WayJoin2InputFormatTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(WayJoin2InputFormatTest);
  CPPUNIT_TEST(testBasics);
  CPPUNIT_TEST_SUITE_END();

public:

  void testBasics()
  {
    string outDir = "test-output/WayJoin2InputFormat";
    string pbfIn = outDir + "/SmallSplits.pbf";
    string csqIn = outDir + "/sample.csq";
    Hdfs fs;
    if (fs.exists(pbfIn) == false)
    {
      fs.copyFromLocal("test-files/io/SmallSplits.pbf", pbfIn);
    }

    // create a sequence file that looks like the output of WayJoin1Reducer
    WayJoin1Reducer::Value v;
    int64_t wid;

    boost::shared_ptr<ostream> sample(fs.create(csqIn));

    pp::CppSeqFile::Writer writer(*sample);

    v.nodeId = 1;
    v.x = -104;
    v.y = 38;
    wid = 2;
    writer.appendFixed<int64_t, WayJoin1Reducer::Value>(wid, v);

    v.nodeId = 3;
    v.x = -104;
    v.y = 38;
    wid = 4;
    writer.appendFixed<int64_t, WayJoin1Reducer::Value>(wid, v);

    writer.close();
    sample.reset();

    // create an input format
    WayJoin2InputFormat uut;
    uut.setPath(pbfIn + "," + csqIn);

    // verify that the input splits look as expected.
    CPPUNIT_ASSERT_EQUAL(2, uut.getSplitCount());

    CPPUNIT_ASSERT_EQUAL(pbfIn, uut.getSplit(0).getPath());
    CPPUNIT_ASSERT_EQUAL((long long)1125, uut.getSplit(0).getStart());
    CPPUNIT_ASSERT_EQUAL((long long)13490, uut.getSplit(0).getLength());

    CPPUNIT_ASSERT_EQUAL(csqIn, uut.getSplit(1).getPath());
    CPPUNIT_ASSERT_EQUAL((long long)0, uut.getSplit(1).getStart());
    CPPUNIT_ASSERT_EQUAL((long long)100, uut.getSplit(1).getLength());
  }
};

}

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::WayJoin2InputFormatTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::WayJoin2InputFormatTest, "glacial");
