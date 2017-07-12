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

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/TileConflator.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/util/Log.h>
#include <hoot/hadoop/pbf/PbfInputFormat.h>
#include <hoot/hadoop/pbf/PbfRecordReader.h>
#include <hoot/hadoop/conflate/ConflateDriver.h>
#include <hoot/hadoop/HadoopTileWorker.h>

// Tgs
#include <tgs/Statistics/Random.h>

#include "MapReduceTestFixture.h"

using namespace std;

namespace hoot
{

class HadoopTileWorkerTest : public MapReduceTestFixture
{
  CPPUNIT_TEST_SUITE(HadoopTileWorkerTest);
  CPPUNIT_TEST(testAll);
  CPPUNIT_TEST_SUITE_END();

public:

  void testAll()
  {
    Tgs::Random::instance()->seed(0);
    OsmMap::resetCounters();
    Settings::getInstance().clear();
    conf().set(ConfigOptions().getUuidHelperRepeatableKey(), true);
    conf().set(ConfigOptions().getUnifyOptimizerTimeLimitKey(), -1);

    string outDir = "test-output/hadoop/HadoopTileWorkerTest/";
    Hdfs fs;
    if (fs.exists(outDir))
    {
      fs.deletePath(outDir, true);
    }
    fs.copyFromLocal("test-files/DcTigerRoads.pbf", outDir + "in1.pbf/DcTigerRoads.pbf");
    fs.copyFromLocal("test-files/DcGisRoads.pbf", outDir + "in2.pbf/DcGisRoads.pbf");

    boost::shared_ptr<TileWorker> worker(new HadoopTileWorker());
    TileConflator uut(worker);
    // ~240m
    uut.setBuffer(8.0 / 3600.0);
    uut.setMaxNodesPerBox(5000);

    uut.setSources(QString::fromStdString(outDir) + "in1.pbf",
                   QString::fromStdString(outDir) + "in2.pbf");

    uut.conflate(QString::fromStdString(outDir) + "HadoopTileWorkerTest.pbf");

    OsmMapPtr map(new OsmMap);
    OsmPbfReader reader(true);
    reader.setUseFileStatus(true);
    std::vector<FileStatus> status = fs.listStatus(outDir + "HadoopTileWorkerTest.pbf", true);
    for (size_t i = 0; i < status.size(); i++)
    {
      const string& path = status[i].getPath();
      LOG_INFO(path);
      if (QString::fromStdString(path).endsWith(".pbf"))
      {
        boost::shared_ptr<istream> is(fs.open(path));
        reader.parse(is.get(), map);
      }
    }

    QDir().mkpath(QString::fromStdString(outDir));

    OsmXmlWriter writer;
    writer.setIncludeHootInfo(true);
    writer.write(map, QString::fromStdString(outDir + "/result.osm"));

    HOOT_FILE_EQUALS("test-files/hadoop/HadoopTileWorkerTest/result.osm",
                     "test-output/hadoop/HadoopTileWorkerTest/result.osm");
  }

  virtual void tearDown()
  {
    Settings::getInstance().clear();
  }
};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::HadoopTileWorkerTest, "glacial");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::HadoopTileWorkerTest, "current");

