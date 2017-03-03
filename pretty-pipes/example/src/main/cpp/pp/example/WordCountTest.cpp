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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <pp/Hdfs.h>
#include <pp/mapreduce/LocalJobRunner.h>
#include <pp/mapreduce/Job.h>
using namespace pp;

// Standard
#include <iostream>
#include <stdlib.h>

#include "WordCountMapper.h"
#include "WordCountReducer.h"

class WordCountTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(WordCountTest);
  CPPUNIT_TEST(testJob);
  CPPUNIT_TEST_SUITE_END();

public:

  void testJob()
  {
    Hdfs fs;
    if (fs.exists("constitution.txt") == false)
    {
      fs.copyFromLocal("test-files/constitution.txt", "constitution.txt");
    }
    if (fs.exists("testJobOutput"))
    {
      fs.deletePath("testJobOutput", true);
    }

    // create a job
    Job job;

    // set the name
    job.setName("My Word Count Test");

    // set the input/output
    job.setInput("constitution.txt");
    job.setOutput("testJobOutput");

    // setup the mapper and reducer classes.
    job.setMapperClass(WordCountMapper::className());
    job.setReducerClass(WordCountReducer::className());

    // Adds all libraries in this directory to the job.
    job.addLibraryDir(getenv("HADOOP_HOME") + string("/c++/Linux-amd64-64/lib/"));
    job.addLibraryDir(getenv("HOOT_HOME") + string("/pretty-pipes/lib/"));

    // This library will be used to provide mapper/reducer classes and anything else referenced
    // by the factory.
    job.addPlugin(
      "/home/jason.surratt/geoeye/src/hootenanny/pretty-pipes/lib/libPrettyPipesExample.so");

    // run the job locally
    job.setJobTracker("local");
    job.run();

    long size = 0;
    vector<FileStatus> status = fs.listStatus("testJobOutput");
    for (size_t i = 0; i < status.size(); i++)
    {
      if (QString::fromStdString(status[i].getPath()).contains("/part-"))
      {
        size += status[i].getLen();
      }
    }
    CPPUNIT_ASSERT_EQUAL(17266l, size);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WordCountTest, "glacial");

