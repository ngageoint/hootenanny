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

// Standard
#include <iostream>
#include <stdlib.h>

#include <hoot/hadoop/pbf/PbfInputFormat.h>
#include <hoot/hadoop/pbf/PbfRecordReader.h>
#include <hoot/hadoop/way-join/WayJoin1Mapper.h>
#include <hoot/hadoop/way-join/WayJoin1Reducer.h>

#include "../MapReduceTestFixture.h"

namespace hoot
{

class WayJoin1Test : public MapReduceTestFixture
{
  CPPUNIT_TEST_SUITE(WayJoin1Test);
  CPPUNIT_TEST(testJob);
  CPPUNIT_TEST_SUITE_END();

public:

  void testJob()
  {
    Hdfs fs;
    if (fs.exists("ToyTestA.osm.pbf") == false)
    {
      fs.copyFromLocal("test-files/ToyTestA.osm.pbf", "ToyTestA.osm.pbf");
    }

    // create a job
    Job job;

    job.setVerbose(Log::getInstance().getLevel() <= Log::Debug);
    // set the name
    job.setName("WayJoin1Test");

    // set the input/output
    job.setInput("ToyTestA.osm.pbf");
    job.setOutput("test-output/WayJoin1Test");

    // setup the mapper and reducer classes.
    job.setMapperClass(WayJoin1Mapper::className());
    job.setReducerClass(WayJoin1Reducer::className());
    job.setInputFormatClass(PbfInputFormat::className());
    job.setRecordReaderClass(PbfRecordReader::className());
    job.setRecordWriterClass(CppSeqFileRecordWriter::className());
    job.setNumReduceTask(2);

    // Adds all libraries in this directory to the job.
    job.addLibraryDir(getenv("HADOOP_HOME") + string("/c++/Linux-amd64-64/lib/"));
    job.addLibraryDir(getenv("HOOT_HOME") + string("/pretty-pipes/lib/"));
    job.addLibraryDir(getenv("HOOT_HOME") + string("/lib/"));

    // This library will be used to provide mapper/reducer classes and anything else referenced
    // by the factory.
    job.addPlugin(getenv("HOOT_HOME") + string("/lib/libHootHadoop.so.1"));

    // run the job.
    job.run();

    long size = 0;
    vector<FileStatus> status = fs.listStatus("test-output/WayJoin1Test");
    for (size_t i = 0; i < status.size(); i++)
    {
      if (QString::fromStdString(status[i].getPath()).contains("/part-"))
      {
        size += status[i].getLen();
      }
    }
    CPPUNIT_ASSERT_EQUAL(1800l, size);
  }
};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::WayJoin1Test, "glacial");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::WayJoin1Test, "current");

