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
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/util/Log.h>
#include <hoot/hadoop/pbf/PbfInputFormat.h>
#include <hoot/hadoop/pbf/PbfRecordReader.h>
#include <hoot/hadoop/conflate/ConflateDriver.h>

#include "../MapReduceTestFixture.h"

using namespace geos::geom;
using namespace std;

namespace hoot
{

class ConflateDriverTest : public MapReduceTestFixture
{
  CPPUNIT_TEST_SUITE(ConflateDriverTest);
  CPPUNIT_TEST(testJob);
  CPPUNIT_TEST_SUITE_END();

public:

  void testJob()
  {
    string outDir = "test-output/hadoop/conflate/ConflateDriverTest/";

    Hdfs fs;

    if (fs.exists(outDir))
    {
      fs.deletePath(outDir, true);
    }

    if (fs.exists(outDir + "input.pbf/input.pbf") == false)
    {
      fs.mkdirs(outDir + "input.pbf");
      //fs.copyFromLocal("test-files/hadoop/conflate/ConflateDriverTest/input.pbf",
      //                 outDir + "input.pbf/input.pbf");
      fs.copyFromLocal("test-files/ToyTestCombined.osm.pbf", outDir + "input.pbf/ToyTestCombined.osm.pbf");
      fs.copyFromLocal("test-files/hadoop/conflate/ConflateDriverTest/input1.stats",
                       outDir + "input.pbf/input1.stats");
      fs.copyFromLocal("test-files/hadoop/conflate/ConflateDriverTest/input2.stats",
                       outDir + "input.pbf/input2.stats");
    }

    vector<Envelope> e;
    e.push_back(Envelope(-180,-104.898,-90,90));

    LOG_INFO("Starting the first conflation.");
    ConflateDriver uut;
    uut.conflate(QString::fromStdString(outDir) + "input.pbf", e, 0.002,
                 QString::fromStdString(outDir) + "step1.pbf");

    LOG_INFO("Starting the second conflation.");
    e.clear();
    e.push_back(Envelope(-104.900,180,-90,90));
    uut.conflate(QString::fromStdString(outDir) + "step1.pbf", e, 0.002,
                 QString::fromStdString(outDir) + "step2.pbf");

    LOG_INFO("Starting the third conflation.");
    e.clear();
    uut.conflate(QString::fromStdString(outDir) + "step2.pbf", e, 0.002,
                 QString::fromStdString(outDir) + "result.pbf");

    OsmMapPtr map(new OsmMap);
    OsmPbfReader reader(true);
    reader.setUseFileStatus(true);
    std::vector<FileStatus> status = fs.listStatus(outDir + "result.pbf");
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

    HOOT_FILE_EQUALS(
          QString::fromUtf8("test-files/hadoop/conflate/ConflateDriverTest/result.osm"),
          QString::fromStdString(outDir + "/result.osm"));
  }
};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::ConflateDriverTest, "glacial");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::ConflateDriverTest, "current");

