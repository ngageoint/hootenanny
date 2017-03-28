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
#include <pp/Hdfs.h>
#include <pp/mapreduce/Job.h>
using namespace pp;

// Qt
#include <QDir>

// Standard
#include <iostream>
#include <stdlib.h>

#include <hoot/hadoop/pbf/PbfInputFormat.h>
#include <hoot/hadoop/pbf/PbfRecordReader.h>
#include <hoot/hadoop/paint-nodes/PaintNodesMapper.h>
#include <hoot/hadoop/paint-nodes/PaintNodesReducer.h>

#include "../MapReduceTestFixture.h"

namespace hoot
{

class PbfInputFormatTest : public MapReduceTestFixture
{
  CPPUNIT_TEST_SUITE(PbfInputFormatTest);
  CPPUNIT_TEST(testJob);
  CPPUNIT_TEST_SUITE_END();

public:

  void testJob()
  {
    Hdfs fs;
    if (fs.exists("SmallSplits.pbf") == false)
    {
      fs.copyFromLocal("test-files/io/SmallSplits.pbf", "SmallSplits.pbf");
    }

    if (fs.exists("test-output/PbfInputFormatTest"))
    {
      fs.deletePath("test-output/PbfInputFormatTest", true);
    }
    fs.mkdirs("test-output/PbfInputFormatTest");

    // create a job
    Job job;

    // set the name
    job.setName("PbfInputFormatTest");

    // set the input/output
    job.setInput("SmallSplits.pbf");
    job.setOutput("test-output/PbfInputFormatTest");

    job.setNumReduceTask(0);

    job.getConfiguration().set("hoot.envelope", "Env[-180:180,-90:90]");
    job.getConfiguration().setDouble("hoot.pixel.size", 0.1);

    // setup the mapper and reducer classes.
    job.setMapperClass(PaintNodesMapper::className());
    job.setInputFormatClass(PbfInputFormat::className());
    job.setRecordReaderClass(PbfRecordReader::className());

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
    vector<FileStatus> status = fs.listStatus("test-output/PbfInputFormatTest");
    for (size_t i = 0; i < status.size(); i++)
    {
      if (QString::fromStdString(status[i].getPath()).contains("/part-"))
      {
        size += status[i].getLen();
      }
    }
    CPPUNIT_ASSERT_EQUAL(20l, size);
  }
};

}

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::PbfInputFormatTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::PbfInputFormatTest, "quick");
