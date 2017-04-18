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

// Pretty Pipes
#include <pp/io/CppSeqFileRecordWriter.h>
#include <pp/Hdfs.h>
#include <pp/mapreduce/Job.h>
using namespace pp;

// Qt
#include <QDir>

// Standard
#include <iostream>
#include <stdlib.h>

#include <hoot/core/TestUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/hadoop/stats/MapStats.h>
#include <hoot/hadoop/stats/MapStatsDriver.h>

#include "../MapReduceTestFixture.h"

namespace hoot
{

class MapStatsDriverTest : public MapReduceTestFixture
{
  CPPUNIT_TEST_SUITE(MapStatsDriverTest);
  CPPUNIT_TEST(testJob);
  CPPUNIT_TEST_SUITE_END();

public:

  void testJob()
  {
    string outDir = "test-output/hadoop/stats/MapStatsDriverTest/";

    Hdfs fs;
    if (fs.exists(outDir + "SmallSplits.pbf") == false)
    {
      fs.copyFromLocal("test-files/io/SmallSplits.pbf", outDir + "input.pbf/SmallSplits.pbf");
    }

    MapStatsDriver uut;
    const MapStats& s = uut.calculateStats(QString::fromStdString(outDir + "input.pbf"));

    CPPUNIT_ASSERT_EQUAL(string("minNode: -36 maxNode: -1 minWay: -4 maxWay: -1 minRelation: 9223372036854775807 maxRelation: -9223372036854775807 env: Env[-104.9024316:-104.8961823,38.8532424:38.8549614]"),
                         s.toString().toStdString());
  }
};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::MapStatsDriverTest, "glacial");

